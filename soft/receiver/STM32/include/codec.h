#ifndef CODEC_H
#define CODEC_H

#include "ch.h"
#include "hal.h"
#include "measure_cst.h"

typedef struct {
    SPIDriver *spi;      /*SPI driver for codec config*/
    GPIO_TypeDef *cs_port;    /*Chip select port*/
    uint32_t cs_pad;     /*Chip select pad*/
    GPIO_TypeDef *nrst_port;
    uint32_t nrst_pad;
    void *sai2s;     /*I2S driver*/
    uint32_t is_sai;      /*Is SAI used instead of I2S*/
    int32_t buffer[2][COIL_BUFFER_SIZE];       /*First memory buffer for DMA*/
    BinarySemaphore *sem_dma;  /*DMA semaphore*/
} codec_config_t;

extern codec_config_t codec_config[3];
extern BinarySemaphore sem_codec_X;
extern BinarySemaphore sem_codec_Y;
extern BinarySemaphore sem_codec_Z;

void codec_init(void);

void codec_set_dmas(const uint32_t sample_number);
void codec_start_all_dmas(void);
void codec_stop_all_dmas(void);

void codec_set_amp(const uint32_t codec, const uint32_t lna, const uint32_t pga);
void codec_set_ampI(const uint32_t codec,const uint32_t lna, const uint32_t pga);
void codec_get_amp(const uint32_t codec, uint32_t *const lna_dst, uint32_t *const pga_dst);

void codec_wake_up(void);
void codec_sleep(void);

void codec_send_config(void);
uint8_t codec_read(uint32_t codec, uint8_t reg);
void codec_write(uint32_t codec, uint8_t reg, uint8_t value);

int32_t convert_from_buffer(const uint32_t codec,const uint32_t buffer,const uint32_t index);

const stm32_dma_stream_t *codec_dma_stream(const uint32_t codec);
#endif
