#include "debug.h"
#include "codec.h"
#include "string.h"

#define CODEC_SPI SPID1
#define SPIDX SPID2
#define SPIDY SPID3
#define SPIDZ SPID4

#define CODEC_RESET_DELAY 30
#define CODEC_SEND_DELAY 1
#define STM32_SPI_IRQ_PRIORITY 10
#define STM32_SPI_DMA_PRIORITY 1
#define NUMBER_CODECS 3

BSEMAPHORE_DECL(sem_codec_X,0);
BSEMAPHORE_DECL(sem_codec_Y,0);
BSEMAPHORE_DECL(sem_codec_Z,0);

static WORKING_AREA(waSetAmpThread, 512);
static WORKING_AREA(waSetAmp,512);
static MAILBOX_DECL(set_amp_mb,&waSetAmp,16);

static void codec_struct_init(void);
static void codec_spi_init(void);
static void codec_i2s_init(void);
static void codec_sai_init(void);
static void codec_dma_init(void);
static void cb_dma(void *args, uint32_t flags);
static msg_t set_amp_thread(void* arg);
static void send_to_codecs(const uint8_t * const buf,const uint32_t size, const uint8_t mask);
static void send_to_codec(const uint8_t * const buf,const uint32_t size, const uint8_t codec);

extern const uint8_t _binary_conf_binary_start;
extern const uint8_t _binary_conf_binary_size;
static const uint8_t *config_file = (uint8_t*) &_binary_conf_binary_start;
static const int config_size = (int) &_binary_conf_binary_size;

typedef struct SAID_t{ //Like a Chibios Driver type but simpler (no state machine)
    SAI_TypeDef *sai;
    SAI_Block_TypeDef *sai_block;
    const stm32_dma_stream_t *dmarx;
    uint32_t rxdmamode;
}SAID_t;

static const SAID_t SAID1={
    SAI1,
    SAI1_Block_A,
    STM32_DMA2_STREAM1,
    0x0
};

static const SPIConfig spi_config={
    NULL,
    GPIOA,              /*To configure properly*/
    0,              /*To configure properly*/
    SPI_CR1_BR     /*Serial clock baud rate*/
    |SPI_CR1_CPHA    /*Bit is valid on trailing edge*/
};


codec_config_t codec_config[3]={
    {
        NULL, //The NULLs are set in struct init;
        GPIOA,
        GPIOA_CODEC_CS_1,
        GPIOC,
        GPIOC_CODEC_nRST_1,
        NULL,
        .is_sai = true,
        .buffer = {{0}},
        &sem_codec_X
    },
    {
        NULL,
        GPIOD,
        GPIOD_CODEC_CS_3,
        GPIOD,
        GPIOD_CODEC_nRST_3,
        NULL,
        .is_sai = false,
        .buffer = {{0}},
        &sem_codec_Y
    },
    {
        NULL,
        GPIOC,
        GPIOC_CODEC_CS_2,
        GPIOB,
        GPIOB_CODEC_nRST_2,
        NULL,
        .is_sai = false,
        .buffer = {{0}},
        &sem_codec_Z
    },
};

void codec_init(){
    /*Init struct*/
    codec_struct_init();

    /*Set SPI1*/
    codec_spi_init();

    /*Set up codec*/
    codec_send_config();
    debug("Codecs config done\r\n");

    /*Set up dma*/
    codec_dma_init();
    codec_start_all_dmas();

    /*Then set I2S and SAI*/
    codec_sai_init();
    asm volatile("dsb");

    codec_i2s_init();
    asm volatile("dsb");

    /*Launch amplification setting mb reader thread*/
    chThdCreateStatic(waSetAmpThread, sizeof(waSetAmpThread), HIGHPRIO, set_amp_thread, NULL);
}

static void codec_struct_init(){
    for(uint32_t i=0;i<NUMBER_CODECS;i++)
        codec_config[i].spi=&CODEC_SPI;
    codec_config[0].sai2s=(void*)&SAID1;
    codec_config[1].sai2s=(void*)&SPID3;
    codec_config[2].sai2s=(void*)&SPID2;
}

static void codec_spi_init(){
    spiStart(&CODEC_SPI,&spi_config);
}

static void codec_i2s_init(void){
    SPI_TypeDef *spi_base;
    for(uint32_t i=0;i<NUMBER_CODECS;i++){

        if(!codec_config[i].is_sai){
            spi_base=((SPIDriver*)codec_config[i].sai2s)->spi;

            /*Enable I2S PLL*/
            if(codec_config[i].sai2s==&SPID2){
                rccEnableSPI2(TRUE);
            }else if(codec_config[i].sai2s==&SPID3){
                rccEnableSPI3(TRUE);
            }

            /*PLL Settings: Launch PLL if not already launched*/
            if(!(RCC->CR&RCC_CR_PLLI2SON)){
                debug("Starting I2S PLL\r\n");
                RCC->PLLI2SCFGR |= (192<<6)|(0x2<<28);
                RCC->CR |= RCC_CR_PLLI2SON;
                while(!(RCC->CR&RCC_CR_PLLI2SRDY));
            }

            /*Set spi to i2s*/
            spi_base->I2SCFGR|=SPI_I2SCFGR_I2SMOD;
            /*Set as master receive*/
            spi_base->I2SCFGR|=SPI_I2SCFGR_I2SCFG&(0x3<<8);
            /*Set data length to 32bits */
            spi_base->I2SCFGR|=SPI_I2SCFGR_DATLEN&(0x2<<1);
            /*Clock polarity*/
            spi_base->I2SCFGR|=SPI_I2SCFGR_CKPOL;

            /*Prescaler setting*/
            spi_base->I2SPR |= (2<<0);
            /*A divider setting*/
            spi_base->I2SPR |= SPI_I2SPR_ODD;
            /* Master clock output enable*/
            spi_base->I2SPR |= SPI_I2SPR_MCKOE;

            /*Set dma*/
            spi_base->CR2|=SPI_CR2_RXDMAEN&(0x1<<0);
            /*Enable I2S*/
            spi_base->I2SCFGR |= SPI_I2SCFGR_I2SE;
        }
    }
}

void codec_sai_init(){
    /*Enable SAI PLL*/   //TODO : Use the same PLL as I2S
    RCC->APB2ENR |= RCC_APB2ENR_SAI1EN;
    /*PLL Settings: Launch PLL if not already launched*/
    if(!(RCC->CR&RCC_CR_PLLSAION)){
        debug("Starting SAI PLLi\r\n");
        RCC->PLLSAICFGR |=
            (192 << 6)|
            (5 << 24);
        RCC->CR |= RCC_CR_PLLSAION;
        while(!(RCC->CR&RCC_CR_PLLSAIRDY));
    }

    /*!< SAI block x configuration register 1,     Address offset: 0x04 */
    SAI1_Block_A->CR1 =
        (SAI_xCR1_MCKDIV     &   (1 << 20)       ) |       /*!<MCKDIV[3:0] (Master ClocK Divider)  */
        (SAI_xCR1_NODIV      &   (0 << 19)       ) |       /*!<No Divider Configuration   */
        (SAI_xCR1_DMAEN      &   (0 << 17)       ) |       /*!<DMA enable                 */
        (SAI_xCR1_SAIEN      &   (0 << 16)       ) |       /*!<Audio Block enable         */
        (SAI_xCR1_OUTDRIV    &   (0 << 13)       ) |       /*!<Output Drive               */
        (SAI_xCR1_MONO       &   (0 << 12)       ) |       /*!<Mono mode ? : stereo mode                 */
        (SAI_xCR1_SYNCEN     &   (0 << 10)       ) |       /*!<SYNCEN[1:0](SYNChronization ENable) */
        (SAI_xCR1_CKSTR      &   (1 << 9)        ) |       /*!<ClocK STRobing edge ? : falling edge     */                       //TODO
        (SAI_xCR1_LSBFIRST   &   (0 << 8)        ) |       /*!<LSB First Configuration  */
        (SAI_xCR1_DS         &   (7 << 5)        ) |       /*!<DS[1:0] bits (Data Size) */
        (SAI_xCR1_PRTCFG     &   (0 << 2)        ) |       /*!<PRTCFG[1:0] bits (Protocol Configuration)   */
        (SAI_xCR1_MODE       &    1              );        /*!<MODE[1:0] bits (Audio Block Mode)           */

    /*!< SAI block x configuration register 2,     Address offset: 0x08 */
    SAI1_Block_A->CR2 =
        (SAI_xCR2_COMP       &   (00 << 14)      ) |       /*!<COMP[1:0] (Companding mode) */
        (SAI_xCR2_CPL        &   (0 << 13)       ) |       /*!< Complement Bit             */
        (SAI_xCR2_MUTECNT    &   (0 << 7)        ) |       /*!<MUTECNT[5:0] (MUTE counter) */
        (SAI_xCR2_MUTEVAL    &   (0 << 6)        ) |       /*!<Muate value                      */
        (SAI_xCR2_MUTE       &   (0 << 5)        ) |       /*!<Mute mode                        */
        (SAI_xCR2_TRIS       &   (0 << 4)        ) |       /*!<TRIState Management on data line */
        (SAI_xCR2_FFLUSH     &   (0 << 3)        ) |       /*!<Fifo FLUSH                       */
        (SAI_xCR2_FTH        &   (0 << 0)        );        /*!<FTH[1:0](Fifo THreshold)  */                                      //TODO

    /*!< SAI block x frame configuration register, Address offset: 0x0C */
    SAI1_Block_A->FRCR =
        (SAI_xFRCR_FSOFF     &   (1 << 18)       ) |        /*!<Frame Synchronization OFFset      */
        (SAI_xFRCR_FSPO      &   (0 << 17)       ) |        /*!<Frame Synchronization POLarity    */
        (SAI_xFRCR_FSDEF     &   (0 << 16)       ) |        /*!< Frame Synchronization Definition */
        (SAI_xFRCR_FSALL     &   (31 << 8)       ) |        /*!<FSALL[6:0] (Frame synchronization active level length)  */          //TODO
        (SAI_xFRCR_FRL       &    63             );         /*!<FRL[7:0](Frame length)  */                                        //TODO

    /*!< SAI block x slot register,                Address offset: 0x10 */
    SAI1_Block_A->SLOTR =
        (SAI_xSLOTR_SLOTEN   &   (3 << 16)       ) |        /*!<SLOTEN[15:0] (Slot Enable)  */
        (SAI_xSLOTR_NBSLOT   &   (1 << 8)        ) |        /*!<NBSLOT[3:0] (Number of Slot in audio Frame)  */
        (SAI_xSLOTR_SLOTSZ   &   (2 << 6)        ) |        /*!<SLOTSZ[1:0] (Slot size)  */
        (SAI_xSLOTR_FBOFF    &    0             );         /*!<FRL[4:0](First Bit Offset)  */

    /*!< SAI block x interrupt mask register,      Address offset: 0x14 */
    SAI1_Block_A->IMR =
        (SAI_xIMR_LFSDETIE   &   (0 << 6)        ) |        /*!<Late frame synchronization detection interrupt enable          */
        (SAI_xIMR_AFSDETIE   &   (0 << 5)        ) |        /*!<Anticipated frame synchronization detection interrupt enable   */
        (SAI_xIMR_CNRDYIE    &   (0 << 4)        ) |        /*!<Codec not ready interrupt enable                               */
        (SAI_xIMR_FREQIE     &   (0 << 3)        ) |        /*!<FIFO request interrupt enable                                  */
        (SAI_xIMR_WCKCFGIE   &   (0 << 2)        ) |        /*!<Wrong Clock Configuration interrupt enable                     */
        (SAI_xIMR_MUTEDETIE  &   (0 << 1)        ) |        /*!<Mute detection interrupt enable                                */
        (SAI_xIMR_OVRUDRIE   &    0              );         /*!<Overrun underrun interrupt enable                              */



    SAI1_Block_A->CR1 |=
        (SAI_xCR1_DMAEN      &   (1 << 17)       ) ;       /*!<DMA enable                 */
    /*!<  Activate the audio block */
    SAI1_Block_A->CR1 |= SAI_xCR1_SAIEN;                  /*!<Audio Block enable         */


}

static void codec_dma_init(){
    const stm32_dma_stream_t *dmarx;
    uint32_t rxdmamode;
    volatile void *peripheral_register;

    for (uint32_t i=0;i<NUMBER_CODECS;i++){
        //Retrieve the values in both cases (SAI or I2S)
        dmarx = codec_dma_stream(i);

        dmaStreamDisable(dmarx);
        dmaStreamAllocate(dmarx, STM32_SPI_IRQ_PRIORITY, cb_dma, codec_config[i].sem_dma);
        switch (i) {
            case 0:
                rxdmamode=((SAID_t*)codec_config[i].sai2s)->rxdmamode|
                    STM32_DMA_CR_MSIZE_WORD |   /*32-bit words in memory*/
                    STM32_DMA_CR_PSIZE_WORD;    /*32-bit word in peripherial*/
                peripheral_register = &SAI1_Block_A->DR;
                dmaStreamSetTransactionSize(dmarx,COIL_BUFFER_SIZE);
                break;
            case 1:
            case 2:
                rxdmamode=((SPIDriver*)codec_config[i].sai2s)->rxdmamode|
                    STM32_DMA_CR_MSIZE_HWORD |   /*16-bit words in memory*/
                    STM32_DMA_CR_PSIZE_HWORD;    /*16-bit word in peripherial*/
                peripheral_register=&((SPIDriver*)codec_config[i].sai2s)->spi->DR;
                dmaStreamSetTransactionSize(dmarx, 2 * COIL_BUFFER_SIZE);
                break;
        }

        dmaStreamSetMemory0(dmarx,codec_config[i].buffer[0]);
        dmaStreamSetMemory1(dmarx,codec_config[i].buffer[1]);
        dmaStreamSetPeripheral(dmarx,peripheral_register);
        dmaStreamSetMode(dmarx,
                rxdmamode|                  /*Contains the dma channel*/
                STM32_DMA_CR_PL(STM32_SPI_DMA_PRIORITY)|    /*low priority*/
                STM32_DMA_CR_CIRC|          /*Circular mode*/
                STM32_DMA_CR_MINC|          /*Increment memory pointer according to MSIZE*/
                STM32_DMA_CR_DBM |
                STM32_DMA_CR_TCIE);         /*Transfer complete interrupt*/
    }
}

static void cb_dma(void *args, uint32_t flags){
    (void) flags;
    chSysLockFromIsr();
    chBSemSignalI((BinarySemaphore *) args);
    chSysUnlockFromIsr();
}

void codec_send_config(){
    static const uint8_t codec_reset[]={0,0,2,1};
    //Send hard reset
    for(uint32_t i=0;i<3;i++)
        palSetPad(codec_config[i].nrst_port,codec_config[i].nrst_pad);
    chThdSleepMilliseconds(CODEC_RESET_DELAY);
    for(uint32_t i=0;i<3;i++)
        palClearPad(codec_config[i].nrst_port,codec_config[i].nrst_pad);
    chThdSleepMilliseconds(CODEC_RESET_DELAY);
    for(uint32_t i=0;i<3;i++)
        palSetPad(codec_config[i].nrst_port,codec_config[i].nrst_pad);
    chThdSleepMilliseconds(CODEC_RESET_DELAY);

    //Send soft reset
    //Chip select the codecs with 0x7
    send_to_codecs(codec_reset,sizeof(codec_reset),0x7);
    //Give time for codec init
    chThdSleepMilliseconds(CODEC_RESET_DELAY);
    //Send conf
    send_to_codecs(config_file,config_size,0x7);
}

void codec_start_all_dmas(){
    for(uint32_t i=0;i<NUMBER_CODECS;i++){
        dmaStreamEnable(codec_dma_stream(i));
    }
}

void codec_stop_all_dmas(){
    for(uint32_t i=0;i<NUMBER_CODECS;i++){
        dmaStreamDisable(codec_dma_stream(i));
    }
}


void codec_set_ampI(const uint32_t codec, const uint32_t lna, const uint32_t pga){
    chMBPostI(&set_amp_mb,codec<<16|lna<<8|pga);
}

void codec_set_amp(const uint32_t codec, const uint32_t lna, const uint32_t pga){
    chMBPost(&set_amp_mb,codec<<16|lna<<8|pga,TIME_IMMEDIATE);
}

void codec_get_amp(const uint32_t codec, uint32_t *const lna_dst, uint32_t *const pga_dst){
    codec_write(codec,0x0,0x1);
    *lna_dst=(codec_read(codec,0x34)&0x10)>>4;
    *pga_dst=(codec_read(codec,0x3b)&0xff);
}

static msg_t set_amp_thread(void* arg){
    (void) arg;
    msg_t next_amp;
    while(1){
	chMBFetch(&set_amp_mb,&next_amp,TIME_INFINITE);
	//envoyer les valeurs au codec selon :
	// -> codec = next_amp>>16
	// -> lna = (next_amp&0x10)
	// -> pga = (next_amp&0xFF)
	// le LNA amplificateur est routé sur IN1L, le suiveur sur IN1R
	// on utilise dans tous les cas l'ADC gauche, mais on peut router IN1R dessus également

	/* Registres à configurer :
	 * if next_amp&0x10
	 *    page 1 Register 52 = 0x80 // IN1L is routed to left MICPGA with 10k resistance, IN1R is not routed
	 * else
	 *    page 1 Register 52 = 0x01 // IN1R is routed to left MICPGA with 10k resistance, IN1L is not routed
	 * page 1 Register 59 = next_amp&0xFF //left channel adc amp
	 *
	 */
	uint32_t codec = next_amp>>16;
	uint32_t lna = next_amp&0x100;

	codec_write(codec,0,1);
	if(lna)
	    codec_write(codec,52,0x80);
	else
	    codec_write(codec,52,0x01);
	codec_write(codec,59,next_amp&0xff);
	/*	debug("codec: %d, real amp: %d, ",codec,codec_read(codec,59));

	codec_write(codec,0,0);
	debug("gain applied : %d\r\n",codec_read(codec,36)>>7);*/
    }
    return 0;
}


void codec_wake_up(void){
    //wake codec up :
    // page 1 reg 1 = 0x0
    // page 1 reg 2 = 0x1
    static const uint8_t wake_up[]={0x0,0x1,0x2,0x0,0x4,0x1};
    send_to_codecs(wake_up,sizeof(wake_up),0x7);
}

void codec_sleep(void){
    //put codec to sleep :
    // page 1 reg 2 = 0x8
    // page 1 reg 1 = 0x8
    static const uint8_t sleep[]={0x0,0x1,0x4,0x8,0x2,0x8};
    send_to_codecs(sleep,sizeof(sleep),0x7);
}

void codec_set_dmas(const uint32_t sample_number){
    //set dma buffer size to 2*sample_number (2 channels)
    //x4 because 2 channels and 2 uint16_t in a uint32_t
    for(uint32_t i=0;i<NUMBER_CODECS;i++)
        dmaStreamSetTransactionSize(codec_dma_stream(i),sample_number*4);
}

static void send_to_codecs(const uint8_t* const buf, const uint32_t size, const uint8_t cs_mask){
    for(uint8_t i=0;i<3;i++) {
        if(cs_mask&(1<<i)) {
            send_to_codec(buf,size,i);
        }
    }
}

static void send_to_codec(const uint8_t* const buf, const uint32_t size, const uint8_t codec){
    uint32_t j;

    for(j=0;j<size;j+=2){
        palClearPad(codec_config[codec].cs_port,codec_config[codec].cs_pad);
        spiSend(&CODEC_SPI,2,buf+j);
        palSetPad(codec_config[codec].cs_port,codec_config[codec].cs_pad);
        chThdSleepMilliseconds(CODEC_SEND_DELAY);
    }

}

const stm32_dma_stream_t *codec_dma_stream(const uint32_t codec){
    if(codec_config[codec].is_sai){
        return ((SAID_t*)codec_config[codec].sai2s)->dmarx;
    }else{
        return ((SPIDriver*)codec_config[codec].sai2s)->dmarx;
    }
}

uint8_t codec_read(const uint32_t codec, const uint8_t reg){
    uint8_t send_buf[2]={((reg<<1)|1), 0x00};
    uint8_t received_buf[2];
    palClearPad(codec_config[codec].cs_port,codec_config[codec].cs_pad);
    spiExchange(&CODEC_SPI,2,send_buf,received_buf);
    palSetPad(codec_config[codec].cs_port,codec_config[codec].cs_pad);
    chThdSleepMilliseconds(CODEC_SEND_DELAY);
    return received_buf[1];
}

void codec_write(const uint32_t codec, const uint8_t reg, const uint8_t value){
    uint8_t send_buf[2]={((reg<<1)), value};
    palClearPad(codec_config[codec].cs_port,codec_config[codec].cs_pad);
    spiSend(&CODEC_SPI,2,send_buf);
    palSetPad(codec_config[codec].cs_port,codec_config[codec].cs_pad);
    chThdSleepMilliseconds(CODEC_SEND_DELAY);
}

int32_t convert_from_buffer(const uint32_t codec,const uint32_t buffer,const uint32_t index){
    if(codec_config[codec].is_sai)
	return codec_config[codec].buffer[buffer][index];
    else
	return (codec_config[codec].buffer[buffer][index]>>16)+(codec_config[codec].buffer[buffer][index]<<16);
}
