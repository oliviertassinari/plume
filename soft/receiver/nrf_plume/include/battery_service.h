#ifndef BATTERIE_SERVICE_H__
#define BATTERIE_SERVICE_H__

extern app_timer_id_t m_battery_timer_id;
extern ble_bas_t m_bas;

void adc_start(void);
void bas_init(void);
void battery_level_meas_timeout_handler(void * p_context);

#endif // BATTERIE_SERVICE_H__
