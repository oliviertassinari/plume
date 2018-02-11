/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "conf.h"

/* global declarations */
/**< Structure used to identify the battery service. */
ble_bas_t  m_bas;

/**< Reference voltage (in milli volts) used by ADC while doing conversion. */
#define ADC_REF_VOLTAGE_IN_MILLIVOLTS     1200
/**< The ADC is configured to use VDD with 1/3 prescaling as input.
 * And hence the result of conversion is to be multiplied by 3 to get the actual value of the battery voltage.*/
#define ADC_PRE_SCALING_COMPENSATION      4
/**< Typical forward voltage drop of the diode (Part no: SD103ATW-7-F) that is connected in series with the voltage supply. This is the voltage drop when the forward current is 1mA. Source: Data sheet of 'SURFACE MOUNT SCHOTTKY BARRIER DIODE ARRAY' available at www.diodes.com. */
#define DIODE_FWD_VOLT_DROP_MILLIVOLTS    0

#define BATTERY_LEVEL_MEAS_INTERVAL 10000


/**@brief Macro to convert the result of ADC conversion in millivolts.
 *
 * @param[in]  ADC_VALUE   ADC result.
 * @retval     Result converted to millivolts.
 */
#define ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE)\
        ((((ADC_VALUE) * ADC_REF_VOLTAGE_IN_MILLIVOLTS) / 255) * ADC_PRE_SCALING_COMPENSATION)


/**@brief Function for handling the Battery Service events.
 *
 * @details This function will be called for all Battery Service events which are passed to the
 |          application.
 *
 * @param[in]   p_bas  Battery Service structure.
 * @param[in]   p_evt  Event received from the Battery Service.
 */
static void on_bas_evt(ble_bas_t * p_bas, ble_bas_evt_t *p_evt)
{
    //    uint32_t err_code;

    switch (p_evt->evt_type)
    {
        case BLE_BAS_EVT_NOTIFICATION_ENABLED:
            // Start battery timer
	    adc_start();
	    break;

        case BLE_BAS_EVT_NOTIFICATION_DISABLED:
            //err_code = app_timer_stop(m_battery_timer_id);
            //APP_ERROR_CHECK(err_code);
            adc_start();
	    break;

        default:
            // No implementation needed.
            break;
    }
}


static uint8_t my_battery_level_in_percent(const uint16_t mvolts) {
    if (mvolts >= 4200)
        return 100;
    else if (mvolts >= 4100)
	return 90;
    else if (mvolts >= 3970)
	return 80;
    else if (mvolts >= 3920)
	return 70;
    else if (mvolts >= 3870)
	return 60;
    else if (mvolts >= 3830)
	return 50;
    else if (mvolts >= 3790)
	return 40;
    else if (mvolts >= 3750)
	return 30;
    else if (mvolts >= 3700)
	return 20;
    else if (mvolts >= 3600)
	return 10;
    else if (mvolts >= 3300)
	return 5;
    else
	return 0;

}

/**@brief Function for handling the ADC interrupt.
 * @details  This function will fetch the conversion result from the ADC, convert the value into
 *           percentage and send it to peer.
 */
void ADC_IRQHandler(void)
{
    if (NRF_ADC->EVENTS_END != 0)
    {
	uint8_t adc_result;
        uint16_t batt_lvl_in_milli_volts;
	uint8_t battery_level_in_percent;

        uint32_t    err_code;

        NRF_ADC->EVENTS_END     = 0;
        adc_result              = NRF_ADC->RESULT;
        NRF_ADC->TASKS_STOP     = 1;

	batt_lvl_in_milli_volts = ADC_RESULT_IN_MILLI_VOLTS(adc_result);

        battery_level_in_percent = my_battery_level_in_percent(batt_lvl_in_milli_volts);
        uint16_t len = sizeof(uint8_t);
        //battery_level_in_percent = adc_result;
        // Save new battery value
        m_bas.battery_level_last = battery_level_in_percent;
        
        // Update database
        err_code = sd_ble_gatts_value_set(m_bas.battery_level_handles.value_handle,
                                          0,
                                          &len,
                                          &battery_level_in_percent);

	//asm volatile("b .");
	if (
            (err_code != NRF_SUCCESS)
            &&
            (err_code != NRF_ERROR_INVALID_STATE)
            &&
            (err_code != BLE_ERROR_NO_TX_BUFFERS)
            &&
            (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
        )
        {
            APP_ERROR_HANDLER(err_code);
        }

    }
}

/**@brief Function for making the ADC start a battery level conversion.
 */
void adc_start(void)
{
    uint32_t err_code;

    // Configure ADC
    NRF_ADC->INTENSET   = ADC_INTENSET_END_Msk;
    NRF_ADC->CONFIG     = (ADC_CONFIG_RES_8bit                        << ADC_CONFIG_RES_Pos)     |
                          (ADC_CONFIG_INPSEL_AnalogInputNoPrescaling  << ADC_CONFIG_INPSEL_Pos)  |
                          (ADC_CONFIG_REFSEL_VBG                      << ADC_CONFIG_REFSEL_Pos)  |
                          (ADC_CONFIG_PSEL_AnalogInput4               << ADC_CONFIG_PSEL_Pos)    |
                          (ADC_CONFIG_EXTREFSEL_None                  << ADC_CONFIG_EXTREFSEL_Pos);
    NRF_ADC->EVENTS_END = 0;
    NRF_ADC->ENABLE     = ADC_ENABLE_ENABLE_Enabled;

    // Enable ADC interrupt
    err_code = sd_nvic_ClearPendingIRQ(ADC_IRQn);
    APP_ERROR_CHECK(err_code);

    err_code = sd_nvic_SetPriority(ADC_IRQn, NRF_APP_PRIORITY_LOW);
    APP_ERROR_CHECK(err_code);

    err_code = sd_nvic_EnableIRQ(ADC_IRQn);
    APP_ERROR_CHECK(err_code);

    NRF_ADC->EVENTS_END  = 0;    // Stop any running conversions.
    NRF_ADC->TASKS_START = 1;
}

/**@brief Function for initializing the Battery Service.
 */
void bas_init(void)
{
    uint32_t       err_code;
    ble_bas_init_t bas_init_obj;

    memset(&bas_init_obj, 0, sizeof(bas_init_obj));

    bas_init_obj.evt_handler          = on_bas_evt;
    bas_init_obj.support_notification = true;
    bas_init_obj.p_report_ref         = NULL;
    bas_init_obj.initial_batt_level   = 100;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init_obj.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_report_read_perm);

    err_code = ble_bas_init(&m_bas, &bas_init_obj);
    APP_ERROR_CHECK(err_code);
    adc_start();
}
