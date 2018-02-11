#include "conf.h"

ble_gap_sec_params_t             m_sec_params;
uint16_t                         m_conn_handle;

static void on_ble_evt(ble_evt_t * p_ble_evt) {
    uint32_t                         err_code;
    static ble_gap_evt_auth_status_t m_auth_status;
    ble_gap_enc_info_t *             p_enc_info;

    switch (p_ble_evt->header.evt_id) {
    case BLE_GAP_EVT_CONNECTED:
#ifdef DEBUG_NRF_USER
	nrf_gpio_pin_set(CONNECTED_LED_PIN_NO);
#endif
	m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

	break;

    case BLE_GAP_EVT_DISCONNECTED:
#ifdef DEBUG_NRF_USER
	nrf_gpio_pin_clear(CONNECTED_LED_PIN_NO);
#endif
	m_conn_handle = BLE_CONN_HANDLE_INVALID;

	advertising_start();
	break;

    case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
	err_code = sd_ble_gap_sec_params_reply(m_conn_handle,
					       BLE_GAP_SEC_STATUS_SUCCESS,
					       &m_sec_params);
	APP_ERROR_CHECK(err_code);
	break;

    case BLE_GATTS_EVT_SYS_ATTR_MISSING:
	err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0);
	APP_ERROR_CHECK(err_code);
	break;

    case BLE_GAP_EVT_AUTH_STATUS:
	m_auth_status = p_ble_evt->evt.gap_evt.params.auth_status;
	break;

    case BLE_GAP_EVT_SEC_INFO_REQUEST:
	p_enc_info = &m_auth_status.periph_keys.enc_info;
	if (p_enc_info->div == p_ble_evt->evt.gap_evt.params.sec_info_request.div) {
	    err_code = sd_ble_gap_sec_info_reply(m_conn_handle, p_enc_info, NULL);
	    APP_ERROR_CHECK(err_code);
	}
	else {
	    // No keys found for this device
	    err_code = sd_ble_gap_sec_info_reply(m_conn_handle, NULL, NULL);
	    APP_ERROR_CHECK(err_code);
	}
	break;

    case BLE_GAP_EVT_TIMEOUT:
	if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISEMENT) {
	    // Go to system-off mode (this function will not return; wakeup will cause a reset)                
	    err_code = sd_power_system_off();
	    APP_ERROR_CHECK(err_code);
	}
	break;

    default:
	// No implementation needed.
	break;
    }
}

void ble_evt_dispatch(ble_evt_t * p_ble_evt) {
    on_ble_evt(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);
    
    ble_plume_on_ble_evt(&m_plume, p_ble_evt);
    ble_bas_on_ble_evt(&m_bas, p_ble_evt);
}
