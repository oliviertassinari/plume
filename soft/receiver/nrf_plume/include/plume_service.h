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

#ifndef BLE_PLUME_H__
#define BLE_PLUME_H__

/** UUID section **/

#define PLUME_UUID128 "Plumeisawesome\0"
typedef enum {
    PLUME_UUID_BASE       = 0x0000,
    CHAR_COILS_UUID_BASE  = 0x0001,
    CHAR_SMPR_UUID_BASE   = 0x0002,
    CHAR_CALIB_UUID_BASE  = 0x0003,
    CHAR_ENABLE_UUID_BASE = 0x0004,
} plume_uuid_cst_t;

typedef enum {
    COILS_DATA_SIZE   = 20,
    SMPR_DATA_SIZE    =  1,
    CALIB_DATA_SIZE   = 18,
    ENABLE_DATA_SIZE  =  1,
} plume_uuud_size_t;

// Forward declaration of the ble_plume_t type. 
typedef struct ble_plume_s ble_plume_t;

/**@brief Battery Service structure. This contains various status information for the service. */
typedef struct ble_plume_s {
    uint16_t conn_handle;
    uint16_t service_handle;
    uint8_t uuid_type;
    ble_gatts_char_handles_t coils_handles; 
    ble_gatts_char_handles_t smpr_handles;
    ble_gatts_char_handles_t calib_handles;
    ble_gatts_char_handles_t enable_handles;
    uint8_t notify_ena;
    
} ble_plume_t;


typedef struct {
    uint8_t notify;
    uint8_t write;
    uint8_t read;
    uint16_t uuid_base;
    uint16_t data_size;
} plume_char_init_t;


/** Global variable **/
extern ble_plume_t m_plume;

/** Global functions **/

uint32_t ble_plume_init(void);

void ble_plume_on_ble_evt(ble_plume_t * const p_plume, ble_evt_t * const p_ble_evt);

bool plume_coils_notify_ok(void);

uint32_t ble_plume_coils_update(ble_plume_t * const p_plume, 
				uint8_t * const data, 
				const uint16_t size);
#endif // BLE_PLUME_H__


