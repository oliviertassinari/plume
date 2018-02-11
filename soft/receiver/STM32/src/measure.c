/* the functions written in this file take care of all the measures
 * to work correctly, the samples recorded on the 3 inputs have to be brought back on baseband through multiplication with a cosinus and a sinus on respectively left and right inputs
 * to sum up : | A |       /----[*cos(20kHz*t)]-----L| I |
 *             | D |------<                          | 2 |
 *             | C |       \----[*sin(20kHz*t)]-----R| S |
 *
 *
 */
#include "hal.h"
#include "debug.h"
#include "measure.h"
#include "measure_cst.h"
#include "codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "communication.h"

MUTEX_DECL(mtx_nb_samples);
MUTEX_DECL(mtx_ext_measures);
BSEMAPHORE_DECL(bsem_measure_run,1);
static uint32_t nb_samples=0;
static uint32_t sync_offset=0;
static uint32_t measure_run=0;

//this event is notified once all the measures are valid and ready to be sent

// contains the measure to be exported
uint32_t ext_measure[9];
// contains the current total amplification of each receiving coil
static int amplification[3]={65,65,65};

// contains the calibration values
// structure : {without_pre_amplifier_X, with_pre_amplifier_X, without_pre_amplifier_Y, with_pre_amplifier_Y, without_pre_amplifier_Z, with_pre_amplifier_Z}
static float calibration[6] = {0,45,0,45,0,45};

//indicates if we are at the first cycle of emission, then we need to set up the first phase setting
static int first_cycle = 1;

//Returns the buffer currently used by double buffering DMA
static uint32_t current_buffer(const uint32_t codec);

void measure_set_calib(float* new_calib){
    for(int ind =0;ind<6;ind++)
        calibration[ind]=new_calib[ind];
}

//Improved method for synchronisation offset determination
static void conv_sync(void){
    // alpha is in thousandths
    unsigned int alpha = 990;
    // 3/4 of the buffer 
    uint32_t limit=(nb_samples)/4;
    volatile    uint64_t sum=0;
    volatile    uint64_t min;
    uint32_t min_index = limit;
    uint32_t i;
    uint32_t a;
    int buf[3] = {1-current_buffer(REC_COIL_X),1-current_buffer(REC_COIL_Y),1-current_buffer(REC_COIL_Z)};
    //Computes the first scalar product
    for (i = 0; i < limit; i++){
	for (int coil_index = REC_COIL_X;coil_index<=REC_COIL_Z;coil_index++){
	    sum += (uint64_t)abs(convert_from_buffer(coil_index,buf[coil_index],2*i));
	    sum += (uint64_t)abs(convert_from_buffer(coil_index,buf[coil_index],2*i+1));
	}
    }
    min = sum;
    for (i = 0;i < nb_samples-1; i++){
        a = (i+limit)%nb_samples;
        //Add values on the right
	for (int coil_index = REC_COIL_X;coil_index<=REC_COIL_Z;coil_index++){
	    sum += (uint64_t)abs(convert_from_buffer(coil_index,buf[coil_index],2*a));
	    sum += (uint64_t)abs(convert_from_buffer(coil_index,buf[coil_index],2*a+1));
	}
        //Sub values on the left
	for (int coil_index = REC_COIL_X;coil_index<=REC_COIL_Z;coil_index++){
	    sum -= (uint64_t)abs(convert_from_buffer(coil_index,buf[coil_index],2*i));
	    sum -= (uint64_t)abs(convert_from_buffer(coil_index,buf[coil_index],2*i+1));
	}
        if (sum < min){
            min = sum;
            min_index = a;
        }
    }

    //recursive filter, we have to distinct three possibilities to take into account the circular aspect of the offset
    if(first_cycle){
	sync_offset = min_index;
    }
    else{
	if((abs(min_index-sync_offset) < abs(min_index-sync_offset+nb_samples)) && (abs(min_index-sync_offset) < abs(min_index-sync_offset-nb_samples)))
	    sync_offset = (alpha*sync_offset + (1000-alpha)*min_index)/1000;
	else
	    if((abs(min_index-sync_offset-nb_samples) < abs(min_index-sync_offset+nb_samples)) && (abs(min_index-sync_offset-nb_samples) < abs(min_index-sync_offset)))
		sync_offset = ((alpha*(sync_offset+nb_samples) + (1000-alpha)*min_index)/1000)%nb_samples;
	    else
		sync_offset = ((alpha*sync_offset + (1000-alpha)*(min_index+nb_samples))/1000)%nb_samples;
    }
}

static uint32_t current_buffer(const uint32_t codec){
    return (codec_dma_stream(codec)->stream->CR>>19)&0x1;
}

static int32_t get_value(const uint32_t emitter_coil, const uint32_t codec, const uint32_t index, const uint32_t channel){
    uint32_t n=nb_samples/4;
    if (index+sync_offset+emitter_coil*n < nb_samples)
	return convert_from_buffer(codec,1-current_buffer(codec),2*(index+sync_offset+emitter_coil*n)+channel);
    else
	return convert_from_buffer(codec,current_buffer(codec),2*(index+sync_offset+emitter_coil*n-nb_samples)+channel);
}

//This function should be used also to set amplification
static void measure_mean(uint32_t *const dst_norm, const uint32_t emitter_coil, const uint32_t codec){
    uint32_t limit=nb_samples/4;
    int64_t sum_x=0;
    float norm, mean_x;

    for (uint32_t i=CAUTION_OFFSET+STABILISATION_OFFSET;i<limit-CAUTION_OFFSET;i++){
        sum_x+=get_value(emitter_coil,codec,i,0);
    }
    //Coputes the average norm
    mean_x=((float)sum_x)/((float)(limit-2*CAUTION_OFFSET-STABILISATION_OFFSET));

    //norm=hypotf(mean_x,mean_y);
    norm = sqrtf(mean_x)*10000;

    *dst_norm=(int32_t)(norm/powf(10,amplification[codec]/40));
}

static WORKING_AREA(wa_measure_thread,1024);
static msg_t measure_thread(void *args){
    (void)args;
    
    while(true){
         //Check if has to stop or launch
        if(!measure_run){
	    //we are at the beginning of the zero emission time so we can trigger the amplification change
	    for(int coil_index = REC_COIL_X; coil_index<=REC_COIL_Z; coil_index++){
		if(amplification[coil_index]>75)
		    codec_set_amp(coil_index,1,amplification[coil_index]-calibration[2*coil_index+1]+calibration[2*coil_index]);
		else
		    codec_set_amp(coil_index,0,amplification[coil_index]);
	    }
	    chBSemWait(&bsem_measure_run);
	    chBSemWait(&sem_codec_X);
	    chBSemWait(&sem_codec_Y);
	    chBSemWait(&sem_codec_Z);
            first_cycle = 1;
	}

        //Wait for the three buffers to be filled
        chBSemWait(&sem_codec_X);
        chBSemWait(&sem_codec_Y);
        chBSemWait(&sem_codec_Z);

	//Computes the offset
        conv_sync();
	//we skip the time needed to finish the three emitting pulse
	if(sync_offset>nb_samples/4)
	    chThdSleepMicroseconds(((sync_offset-nb_samples/4)*1000000)/SAMPLING_FREQUENCY);

        //Computes the values to send
        chMtxLock(&mtx_ext_measures);
        for(uint32_t emitter_coil=0;emitter_coil<3;emitter_coil++) 
            for(uint32_t codec=0;codec<3;codec++){
                measure_mean(&ext_measure[3*emitter_coil+codec],emitter_coil,codec);
	    }
        chMtxUnlock();
        //Broadcast event: measure ready
        com_put_event(MEASURES_READY);
        first_cycle=0;
    }
    return 0;
}

//Set the sample size to use for the calculations
static int set_sample_size(const uint32_t sample_size){
    //TODO
    //This function should deal with the DMA Transaction Size 
    if(sample_size>MAX_SAMPLE_SIZE)
        return 2; 
    chMtxLock(&mtx_nb_samples);
    nb_samples=sample_size;
    chMtxUnlock();
    return 0;
}

int set_emit_frequency(const int emit_freq){
    return set_sample_size((MEASURE_FREQUENCY_MIN*MAX_SAMPLE_SIZE)/emit_freq);
}


//Here is the code to stop the measure thread
void measure_stop(){
    measure_run=false;
}

//Here should go the code to start the measure thread
int measure_start(const int emit_freq){
    int ans=set_emit_frequency(emit_freq);
    //Some code to wake the sleeping thread
    measure_run=true;
    chBSemSignal(&bsem_measure_run);
    return ans;
}

//Start the measure thread
//At the moment the measure freq is not used
//that will be changed
int measure_init(const int emit_freq){
    //Initialize codecs
    codec_init();

    if(emit_freq<MEASURE_FREQUENCY_MIN)
        return 2;

    //set emission frequency (It changes the number of samples to use)
    set_emit_frequency(emit_freq);
    //Launch measure thread, It will immediately sleeps
    chThdCreateStatic(wa_measure_thread,sizeof(wa_measure_thread),HIGHPRIO-1,measure_thread,NULL);
    return 0;
}
