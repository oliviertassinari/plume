
#include "conf.h"

void advertising_init(void) {
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanresp;
    
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    /* our uuids */
    ble_uuid_t adv_uuids[] = {
	{BLE_UUID_BATTERY_SERVICE, BLE_UUID_TYPE_BLE},
	{PLUME_UUID_BASE, m_plume.uuid_type},
    };

    /* Build and set advertising data */
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags.size              = sizeof(flags);
    advdata.flags.p_data            = &flags;
    
    memset(&scanresp, 0, sizeof(scanresp));
    scanresp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    scanresp.uuids_complete.p_uuids  = adv_uuids;

    err_code = ble_advdata_set(&advdata, &scanresp);
    APP_ERROR_CHECK(err_code);
}

void advertising_start(void) {
    uint32_t             err_code;
    ble_gap_adv_params_t adv_params;

    // Start advertising
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.p_peer_addr = NULL;
    adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    adv_params.interval    = APP_ADV_INTERVAL;
    adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = sd_ble_gap_adv_start(&adv_params);
    APP_ERROR_CHECK(err_code);
}
