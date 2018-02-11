#include "codec.h"

#define CODEC_BUFFER_SIZE 1024

CH_IRQ_HANDLER(Vector78);
WORKING_AREA(waCodec0,CODEC_BUFFER_SIZE);
WORKING_AREA(waCodec1,CODEC_BUFFER_SIZE);

int32_t *codec_buffer;

static void codec_spi_init(void);
static void codec_i2s_init(void);
static void codec_dma_init(void);
static void cb_dma(void *args, uint32_t flags);
static void codec_send_config(const uint8_t * const buf, int size);

extern const uint8_t _binary_codec_cfg_binary_start;
extern const uint8_t _binary_codec_cfg_binary_size;
static const uint8_t *config_file = (uint8_t*) &_binary_codec_cfg_binary_start;
static const int config_size = (int) &_binary_codec_cfg_binary_size;

static const SPIConfig spi1_config={
    NULL,
    GPIOA,
    GPIOA_LRCK,
    SPI_CR1_BR_2|SPI_CR1_BR_1|SPI_CR1_BR_0|   /*Serial clock baud rate*/
    (SPI_CR1_DFF        &0x0)|   /*Data format : 8-bit*/
    (SPI_CR1_LSBFIRST   &0x0)|   /*Endianness : MSB first*/
    (SPI_CR1_MSTR       &0x0)|   /*Config as master*/
    (SPI_CR1_CPHA)               /*Bit is valid on trailing edge*/
};

void codec_init(){

    /*First Set SPI1*/
    codec_spi_init();        
    /*Then set I2S2*/
    codec_i2s_init();
    /*Set up DMA on I2S2*/
    codec_dma_init();
    /*Set up codec*/
    codec_send_config(config_file,config_size);
}

static void codec_spi_init(){
    spiObjectInit(&SPID1);
    spiStart(&SPID1,&spi1_config);
}

static void codec_i2s_init(){
    /*Enable I2S PLL*/
    RCC->APB1ENR|=RCC_APB1ENR_SPI2EN;
    /*PLL Settings*/
    RCC->PLLI2SCFGR |= (258<<6)|(0x3<<28);
    RCC->CR |= RCC_CR_PLLI2SON;
    while(!(RCC->CR&RCC_CR_PLLI2SRDY));
    /*Prescaler setting*/
    SPI2->I2SPR |= (3<<0);
    /*A divider setting*/
    SPI2->I2SPR |= SPI_I2SPR_ODD;
    /* Master clock output enable*/
    SPI2->I2SPR |= SPI_I2SPR_MCKOE;
    /*Set spi to i2s*/
    SPI2->I2SCFGR|=SPI_I2SCFGR_I2SMOD;
    /*Set i2s to phillips standard*/
    SPI2->I2SCFGR|=SPI_I2SCFGR_I2SSTD&(0x0<<4);
    /*Set as master receive*/
    SPI2->I2SCFGR|=SPI_I2SCFGR_I2SCFG&(0x3<<8);
    /*Set data length to 32bits */
    SPI2->I2SCFGR|=SPI_I2SCFGR_DATLEN&(0x2<<1);
    /*Set dma*/
    SPI2->CR2|=SPI_CR2_RXDMAEN&(0x1<<0);
    /*Enable I2S*/
    SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;
}

static void codec_dma_init(){
    /*I2S2 is on channel 3 stream 3 of DMA1*/
    dmaStreamAllocate(STM32_DMA1_STREAM3,STM32_SPI_SPI2_IRQ_PRIORITY,cb_dma,NULL);
    dmaStreamSetPeripheral(STM32_DMA1_STREAM3,&SPI2->DR);
    dmaStreamSetMemory0(STM32_DMA1_STREAM3,waCodec0);
    dmaStreamSetMemory1(STM32_DMA1_STREAM3,waCodec1);
    dmaStreamSetTransactionSize(STM32_DMA1_STREAM3,sizeof(waCodec0)/sizeof(uint16_t));
    dmaStreamSetMode(
            STM32_DMA1_STREAM3,
            STM32_DMA_CR_PL(STM32_SPI_SPI2_DMA_PRIORITY)|         /*low priority*/
            STM32_DMA_CR_CIRC|          /*Circular mode*/
            STM32_DMA_CR_DBM|           /*Double buffering*/
            STM32_DMA_CR_MSIZE_HWORD|    /*16-bit words in memory*/
            STM32_DMA_CR_PSIZE_HWORD|    /*16-bit word in peripherial*/
            STM32_DMA_CR_MINC|          /*Increment memory pointer according to MSIZE*/
            STM32_DMA_CR_TCIE);         /*Transfer complete interrupt*/
    dmaStreamEnable(STM32_DMA1_STREAM3);
}

static void cb_dma(void *args, uint32_t flags){
        (void) args;
        (void) flags;
        palTogglePad(GPIOD, GPIOD_LED3);
}

static void codec_send_config(const uint8_t *const buf, const int size){
        static const uint8_t codec_reset[]={0,0,2,1};
        //send reset
        spiSelect(&SPID1);
        spiSend(&SPID1,sizeof(codec_reset),codec_reset);
        spiUnselect(&SPID1);
        chThdSleepMilliseconds(20);
	const uint8_t * b = buf;
        //send conf
	const uint8_t* endbuf=b+size;
	for(;b<endbuf;b+=2){
	  spiSelect(&SPID1);
	  spiSend(&SPID1,2,b);
	  spiUnselect(&SPID1);
	  chThdSleepMilliseconds(1);
	}
}
