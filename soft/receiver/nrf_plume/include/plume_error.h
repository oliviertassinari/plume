#ifndef PLUME_ERROR_H__
#define PLUME_ERROR_H__

void app_error_handler(uint32_t error_code, uint32_t line_num, 
		       const uint8_t * p_file_name);

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);

#define BUSY_WAIT_MS(n)						\
    for (volatile int i__ = 0; i__ < (160000 * n); i__++);
#endif
