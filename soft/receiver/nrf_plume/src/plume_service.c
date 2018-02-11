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
ble_plume_t m_plume;

/* static declarations */

static bool got_tx_buffer_evt;

void start_send_notif(void) {
    m_plume.notify_ena = true;
    got_tx_buffer_evt = true;
    UART_WRITE_DBG("notify ok\n\r");
}

void stop_send_notif(void) {
    m_plume.notify_ena = false;
}

err_t ble_plume_coils_update(ble_plume_t * const p_plume, 
			     uint8_t * const data, 
			     const uint16_t size) {
    if (size > COILS_DATA_SIZE) {
	return NRF_ERROR_DATA_SIZE;
    }
    err_t err_code;

    static uint16_t hvx_data_len = COILS_DATA_SIZE;
    static uint8_t hvx_data[COILS_DATA_SIZE];
    for (int i = 0; i < COILS_DATA_SIZE; i++) {
	hvx_data[i] = i < size ? data[i] : 0;
    } 
    
    /* set the struct for the notify */
    static ble_gatts_hvx_params_t hvx_params = {
	0,
	BLE_GATT_HVX_NOTIFICATION,
	0,
	&hvx_data_len,
	hvx_data
    };    
    hvx_params.handle = p_plume->coils_handles.value_handle;

    /* plan the notify */
    err_code = sd_ble_gatts_hvx(p_plume->conn_handle, &hvx_params);
    if (err_code != NRF_SUCCESS && 
	err_code != BLE_ERROR_NO_TX_BUFFERS &&
	err_code != NRF_ERROR_DATA_SIZE) {
	APP_ERROR_CHECK(err_code);
    }
    if (err_code == BLE_ERROR_NO_TX_BUFFERS) {
	got_tx_buffer_evt = false;
	return BLE_ERROR_NO_TX_BUFFERS;
    }
  
    return err_code;
}


static void on_connect(ble_plume_t * const p_plume, ble_evt_t * const p_ble_evt) {
    p_plume->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

    uint8_t init_data[COILS_DATA_SIZE];
    memset(init_data, 0, COILS_DATA_SIZE);
    sd_ble_gatts_sys_attr_set(p_plume->conn_handle, init_data, COILS_DATA_SIZE);

    UART_WRITE_DBG("connected\r\n"); 
}

static void on_disconnect(ble_plume_t * p_plume, ble_evt_t * p_ble_evt) {
    (void)p_ble_evt;
    p_plume->conn_handle = BLE_CONN_HANDLE_INVALID;
    
    /* We stop sending notify */
    stop_send_notif();
}

static inline uint8_t is_stm_state(uint8_t val) {
    switch (val) {
    case uartSTART:
    case uartOFF:
    case uartSTANDBY:
	return true;
    default:
	return false;
    }
}

static void on_write(ble_plume_t * p_plume, ble_evt_t * p_ble_evt) {
    
    ble_gatts_evt_write_t * p_evt_write = 
	&p_ble_evt->evt.gatts_evt.params.write;

    // Check if this the correct CCCD
    if (p_evt_write->handle == p_plume->coils_handles.cccd_handle
        && p_evt_write->len == 2) {
	// Check if this is a 'start notification'
	if (ble_srv_is_notification_enabled(p_evt_write->data)) {
	    // Do connection parameter negotiation if necessary
	    start_send_notif();
	}
	else {
	    stop_send_notif();
        }
    }

    /* We just got a frequency to transmit */
    if (p_evt_write->handle == p_plume->smpr_handles.value_handle
	     && p_evt_write->len == SMPR_DATA_SIZE) {
	
	uart_write_packet(uartSMPR, p_evt_write->data, SMPR_DATA_SIZE);
    }

    /* We just got calibrations values */
    if (p_evt_write->handle == p_plume->calib_handles.value_handle
	     && p_evt_write->len == CALIB_DATA_SIZE) {
	uart_write_packet(uartCALIBDATA, p_evt_write->data, CALIB_DATA_SIZE);
    }
    
    /* We just got a new state */
    if (p_evt_write->handle == p_plume->enable_handles.value_handle
	     && p_evt_write->len == ENABLE_DATA_SIZE) {
	
	uint8_t control = p_evt_write->data[0];
	if (is_stm_state(control)) {
	    uart_write_control(control);
	}
    }
}

void ble_plume_on_ble_evt(ble_plume_t * const p_plume, 
			  ble_evt_t * const p_ble_evt) {
    switch (p_ble_evt->header.evt_id) {
    case BLE_GAP_EVT_CONNECTED:
	on_connect(p_plume, p_ble_evt);
	break;
            
    case BLE_GAP_EVT_DISCONNECTED:
	on_disconnect(p_plume, p_ble_evt);
	break;
            
    case BLE_GATTS_EVT_WRITE:
	on_write(p_plume, p_ble_evt);
	break;

    case BLE_EVT_TX_COMPLETE:
	got_tx_buffer_evt = true;
	break;
            
    default:
	// No implementation needed.
	break;
    }

}

bool plume_coils_notify_ok(void) {
    return m_plume.notify_ena 
	&& m_plume.conn_handle != BLE_CONN_HANDLE_INVALID
	&& got_tx_buffer_evt;
}


static err_t my_char_add(const plume_char_init_t * const p_init, 
			 ble_gatts_char_handles_t * const p_handles) {

    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_gatts_attr_md_t attr_md;
    
    memset(&char_md, 0, sizeof(char_md));
    memset(&cccd_md, 0, sizeof(cccd_md));
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    memset(&attr_md, 0, sizeof(attr_md));

    if (p_init->notify) {
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	char_md.char_props.notify = 1;
	char_md.p_cccd_md = &cccd_md;
    }
    
    if (p_init->write) {
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
	char_md.char_props.write = 1;
    }
    
    if (p_init->read) {
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	char_md.char_props.read   = 1;
    }
    
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    ble_uuid_t ble_uuid_custom = {
	p_init->uuid_base,
	m_plume.uuid_type,
    };
    
    attr_char_value.p_uuid       = &ble_uuid_custom;
    attr_char_value.p_attr_md    = &attr_md;

    attr_char_value.init_len     = p_init->data_size;
    attr_char_value.max_len      = p_init->data_size;
    
    err_t err_code = sd_ble_gatts_characteristic_add(m_plume.service_handle, 
						     &char_md,
						     &attr_char_value,
						     p_handles);
    return err_code;

}


uint32_t ble_plume_init(void) {

    // Initialize service structure
    m_plume.conn_handle               = BLE_CONN_HANDLE_INVALID;

    
    /* uuid128 declaration (needs to assign a uuid128 to a regular uuid */
    ble_uuid_t ble_uuid_plume = {
	PLUME_UUID_BASE,
	0, /* This field will be filled by sd_ble_uuid_vs_add */
    };
    ble_uuid128_t ble_uuid128_plume = { PLUME_UUID128 };
    
    APP_ERROR_CHECK(sd_ble_uuid_vs_add(&ble_uuid128_plume, &ble_uuid_plume.type));
    
    m_plume.uuid_type = ble_uuid_plume.type;
    
    APP_ERROR_CHECK(sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, 
					     &ble_uuid_plume, 
					     &m_plume.service_handle)
		    );
    static const plume_char_init_t coils_init = {
	true,
	false,
	true,
	CHAR_COILS_UUID_BASE,
	COILS_DATA_SIZE,
    };
    APP_ERROR_CHECK(my_char_add(&coils_init, &m_plume.coils_handles));
    
    static const plume_char_init_t smpr_init = {
	false,
	true,
	true,
	CHAR_SMPR_UUID_BASE,
	SMPR_DATA_SIZE,
    };
    APP_ERROR_CHECK(my_char_add(&smpr_init, &m_plume.smpr_handles));

    static const plume_char_init_t calib_init = {
	false,
	true,
	true,
	CHAR_CALIB_UUID_BASE,
	CALIB_DATA_SIZE,
    };
    APP_ERROR_CHECK(my_char_add(&calib_init, &m_plume.calib_handles));
    
    static const plume_char_init_t enable_init = {
	false,
	true,
	true,
	CHAR_ENABLE_UUID_BASE,
	ENABLE_DATA_SIZE,
    };
    APP_ERROR_CHECK(my_char_add(&enable_init, &m_plume.enable_handles));
    
    return NRF_SUCCESS;
}


