#ifndef MEASURE_CST_H
#define MEASURE_CST_H

/*PWM configuration constants */
#define PWM_SIZE 1<<15
#define SAMPLING_FREQUENCY 96000
#define EMITTER_FREQUENCY 18000 // frequency of the signal /!\ different from the emitting_frequency used in measure.c
#define MEASURE_FREQUENCY_MIN 50
#define STABILISATION_TIME 156 // time before the signal is stable, in µs
#define STABILISATION_OFFSET 100 //((SAMPLING_FREQUENCY*STABILISATION_TIME)/1000000) // in number of samples, corresponds to the stabilisation time
#define AMP2_OFFSET ((SAMPLING_FREQUENCY*STABILISATION_TIME)/1000000+1) // in number of samples, corresponds to the stabilisation time+1
#define AMP_TIME 2 // number of samples before the amplification settings is complete

#define CAUTION_OFFSET 50 //number of samples to leave at the end to be sure to have them valid
#define FIRST_DETECTION 8 // first sample to be non zero after the cycle starts /!\ check number from simulation, depends on the SAMPLING_FREQUENCY

#define PHASE_PER_SAMPLE ((EMITTER_FREQUENCY/SAMPLING_FREQUENCY)*2*M_PI)// 

#define REC_COIL_X    0
#define REC_COIL_Y    1
#define REC_COIL_Z    2

#define EMIT_COIL_X    0
#define EMIT_COIL_Y    1
#define EMIT_COIL_Z    2
#define EMIT_COIL_NONE 3

#define AMP1_BIAS .5 // bias of the measure during the first amplification setting, can be due to the processing of the signal or the stabilisation time of the signal
#define MAX_PGA 95 // maximum amplification, in 0.5 dB
// structure : {without_pre_amplifier_X, with_pre_amplifier_X, without_pre_amplifier_Y, with_pre_amplifier_Y, without_pre_amplifier_Z, with_pre_amplifier_Z}
#define CALIB_DEFAULT {1,21,1,21,1,21}

#define ZERO_VALUE 5 // value under which we consider having no coil emitting, corresponds to the noise value

//DMA buffer base address and offsets, defined as follows :
/*
 * [buf_l&r_x_0]---[buf_l&r_x_1]---[buf_l&r_y_0]---[buf_l&r_y_1]---[buf_l&r_z_0]---[buf_l&r_z_1]
 * |<OFFSET_BUFFER>|               |
 * |<--------OFFSET_COIL---------->|
 * ^
 *BUFFER
 */

//All the values are in int32_t
#define MEASURE_BUFFER_SIZE ((SAMPLING_FREQUENCY*2*3)/MEASURE_FREQUENCY_MIN)
#define COIL_BUFFER_SIZE  ((SAMPLING_FREQUENCY*2)/MEASURE_FREQUENCY_MIN)
#define OFFSET_COIL     COIL_BUFFER_SIZE
#define MAX_SAMPLE_SIZE COIL_BUFFER_SIZE/2

#endif //MEASURE_CST_H
