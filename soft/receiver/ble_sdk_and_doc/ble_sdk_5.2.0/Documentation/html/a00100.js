var a00100 =
[
    [ "Encoding Format", "a00100.html#Packet", null ],
    [ "BLE S110 Events", "a00110.html", [
      [ "0x10 - BLE_GAP_EVT_CONNECTED", "a00110.html#nrf51_evt_connected_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_connected_encoding_sec", null ],
        [ "Encoding of BLE GAP Event - Connected", "a00110.html#nrf51_evt_connected_encoding_sub_a", null ],
        [ "Encoding of Peer Address", "a00110.html#nrf51_evt_connected_encoding_sub_b", null ],
        [ "Encoding of IRK byte", "a00110.html#nrf51_evt_connected_encoding_sub_c", null ],
        [ "Encoding of GAP Connection Parameters", "a00110.html#nrf51_evt_connected_encoding_sub_d", null ]
      ] ],
      [ "0x11 - BLE_GAP_EVT_DISCONNECTED", "a00110.html#nrf51_evt_disconnected_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_disconnected_encoding_sec", null ],
        [ "Encoding of BLE GAP Disconnected event", "a00110.html#nrf51_evt_disconnected_encoding_sub_a", null ]
      ] ],
      [ "0x19 - BLE_GAP_EVT_TIMEOUT", "a00110.html#nrf51_evt_timeout_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_timeout_encoding_sec", null ],
        [ "Encoding of BLE GAP Timeout event", "a00110.html#nrf51_evt_timeout_encoding_sub_a", null ]
      ] ],
      [ "0x12 - BLE_GAP_EVT_CONN_PARAM_UPDATE", "a00110.html#nrf51_evt_conn_param_update_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_conn_param_update_encoding_sec", null ],
        [ "Encoding of GAP Connection Parameter Update event", "a00110.html#nrf51_evt_conn_param_update_encoding_sub_d", null ]
      ] ],
      [ "0x13 - BLE_GAP_EVT_SEC_PARAMS_REQUEST", "a00110.html#nrf51_evt_sec_param_request_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_sec_param_request_encoding_sec", null ],
        [ "Encoding of GAP Security Parameters Request event", "a00110.html#nrf51_evt_sec_param_request_encoding_sub_d", null ],
        [ "Encoding of  Bond, MITM, IO Caps and OOB", "a00110.html#nrf51_evt_sec_param_request_encoding_sub_e", null ]
      ] ],
      [ "0x14 - BLE_GAP_EVT_SEC_INFO_REQUEST", "a00110.html#nrf51_evt_sec_info_request_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_sec_info_request_encoding_sec", null ],
        [ "Encoding of GAP Connection Security Updated event", "a00110.html#nrf51_evt_sec_info_request_encoding_sub_d", null ],
        [ "Encoding of  Security mode and Level", "a00110.html#nrf51_evt_sec_info_request_encoding_sub_e", null ]
      ] ],
      [ "0x52 - BLE_GATTS_EVT_SYS_ATTR_MISSING", "a00110.html#nrf51_evt_sys_attr_missing_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_sys_attr_missing_encoding_sec", null ],
        [ "Encoding of GATTS System Attributes Missing event", "a00110.html#nrf51_evt_sys_attr_missing_encoding_sub_d", null ]
      ] ],
      [ "0x18 - BLE_GAP_EVT_CONN_SEC_UPDATE", "a00110.html#nrf51_evt_conn_sec_update_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_conn_sec_update_encoding_sec", null ]
      ] ],
      [ "0x50 - BLE_GATTS_EVT_WRITE", "a00110.html#nrf51_evt_write_encoding", [
        [ "Event packet encoding", "a00110.html#nrf51_evt_write_encoding_sec", null ],
        [ "Encoding of GATTS Write event", "a00110.html#nrf51_evt_write_encoding_sub_d", null ],
        [ "Encoding of Context", "a00110.html#nrf51_evt_write_encoding_sub_e", null ]
      ] ],
      [ "0x17 - BLE_GAP_EVT_AUTH_STATUS", "a00110.html#nrf51_evt_auth_status_encoding", [
        [ "Encoding of GAP Connection Security Updated event", "a00110.html#nrf51_evt_auth_status_encoding_sub_d", null ]
      ] ]
    ] ],
    [ "BLE S110 Functions", "a00111.html", [
      [ "0x54 - sd_ble_uuid_encode", "a00111.html#nrf51_uuid_encode_encoding", [
        [ "Function packet format", "a00111.html#nrf51_uuid_encode_sec_a", null ],
        [ "Response packet format", "a00111.html#nrf51_uuid_encode_sec_b", null ]
      ] ],
      [ "0x6C - sd_ble_gap_device_name_set", "a00111.html#nrf51_device_name_set_encoding", [
        [ "Function packet format", "a00111.html#nrf51_device_name_set_encoding_sec_function", null ],
        [ "Response packet format", "a00111.html#nrf51_device_name_set_encoding_sec_response", null ]
      ] ],
      [ "0x71 - sd_ble_gap_sec_info_reply", "a00111.html#nrf51_gap_sec_info_reply_encoding", [
        [ "Function packet format", "a00111.html#nrf51_gap_sec_info_reply_encoding_sec", null ],
        [ "Response packet format", "a00111.html#nrf51_gap_sec_info_reply_encoding_sec_response", null ]
      ] ],
      [ "0xA0 - sd_ble_gatts_service_add", "a00111.html#nrf51_gatts_service_add_encoding", [
        [ "Function packet format", "a00111.html#nrf51_gatts_service_add_sec_a", null ],
        [ "Response packet format", "a00111.html#nrf51_gatts_service_add_sec_b", null ]
      ] ],
      [ "0xA2 - sd_ble_gatts_characteristic_add", "a00111.html#nrf51_characteristics_add_encoding", [
        [ "Function packet format", "a00111.html#nrf51_characteristic_add_sec_function", null ],
        [ "Encoding of Characteristic Metadata", "a00111.html#nrf51_characteristic_add_sub_char_metadata", null ],
        [ "Encoding of Characteristic Metadata Property field", "a00111.html#nrf51_characteristic_add_sub_char_metadata_prop", null ],
        [ "Encoding of Characteristic Metadata Extended Property field", "a00111.html#nrf51_characteristic_add_sub_char_metadata_ext_prop", null ],
        [ "Encoding of Characteristic Metadata Presentation Format field.", "a00111.html#nrf51_characteristic_add_sub_char_metadata_pres_format", null ],
        [ "Encoding of User Descriptor, CCCD, and SCCD Descriptor Metadata fields.", "a00111.html#nrf51_characteristic_add_sub_char_metadata_desc_metadata", null ],
        [ "Encoding of Characteristic Attributes", "a00111.html#nrf51_characteristic_add_sub_char_attr", null ],
        [ "Encoding of Characteristic Attributes UUID field.", "a00111.html#nrf51_characteristic_add_sub_char_attr_uuid", null ],
        [ "Encoding of Characteristic Attributes Metadata field.", "a00111.html#nrf51_characteristic_add_sub_char_attr_metadata", null ],
        [ "Response packet format", "a00111.html#nrf51_characteristic_add_b", null ]
      ] ],
      [ "0x6D - sd_ble_gap_device_name_get", "a00111.html#nrf51_device_name_get_encoding", [
        [ "Function packet format", "a00111.html#nrf51_device_name_get_sec_a", null ],
        [ "Response packet format", "a00111.html#nrf51_device_name_get_sec_b", null ],
        [ "Device name data length", "a00111.html#nrf51_device_name_get_response_encoding_sec_d", null ],
        [ "Function packet format", "a00111.html#nrf51_appearance_set_encoding", null ],
        [ "Response packet format", "a00111.html#nrf51_appearance_set_encoding_sec_response", null ]
      ] ],
      [ "0x6A - sd_ble_gap_ppcp_set", "a00111.html#nrf51_ppcp_set_encoding", [
        [ "Function packet format", "a00111.html#nrf51_ppcp_set_encoding_sec", null ],
        [ "Response packet format", "a00111.html#nrf51_ppcp_set_encoding_sec_response", null ]
      ] ],
      [ "0x63 - sd_ble_gap_adv_start", "a00111.html#nrf51_adv_start_encoding", [
        [ "Function packet format", "a00111.html#nrf51_adv_start_encoding_sec", null ],
        [ "Encoding of Peer Address", "a00111.html#nrf51_adv_start_encoding_sub_a", null ],
        [ "Encoding of GAP Whitelist", "a00111.html#nrf51_adv_start_encoding_sub_b", null ],
        [ "Encoding of GAP Whitelist Addresses Array", "a00111.html#nrf51_adv_start_encoding_sub_c", null ],
        [ "Encoding of GAP Whitelist IRKs Array", "a00111.html#nrf51_adv_start_encoding_sub_d", null ],
        [ "Response packet format", "a00111.html#nrf51_adv_start_encoding_sec_response", null ]
      ] ],
      [ "0x62 - sd_ble_gap_adv_data_set", "a00111.html#nrf51_adv_set_encoding", [
        [ "Function packet format", "a00111.html#nrf51_adv_set_encoding_sec_function", null ],
        [ "Encoding of Advertising Packet when data is zero length", "a00111.html#nrf51_adv_set_encoding_sub", null ],
        [ "Response packet format", "a00111.html#nrf51_adv_set_encoding_sec_response", null ]
      ] ],
      [ "0x66 - sd_ble_gap_disconnect", "a00111.html#nrf51_disconnect_encoding", [
        [ "Function packet format", "a00111.html#nrf51_disconnect_encoding_sec_function", null ],
        [ "Response packet format", "a00111.html#nrf51_disconnect_encoding_sec_response", null ]
      ] ],
      [ "0x6F - sd_ble_gap_sec_params_reply", "a00111.html#nrf51_sec_params_reply_encoding", [
        [ "Function packet format", "a00111.html#nrf51_sec_params_reply_sec_function", null ],
        [ "Encoding of  Bond, MITM, IO Caps and OOB", "a00111.html#nrf51_sec_params_reply_encoding_sub_e", null ],
        [ "Response packet format", "a00111.html#nrf51_sec_params_reply_sec_response", null ]
      ] ],
      [ "0x69 - sd_ble_gap_appearance_get", "a00111.html#nrf51_appearance_get_encoding", [
        [ "Function packet format", "a00111.html#nrf51_appearance_get_sec_function", null ],
        [ "Response packet format", "a00111.html#nrf51_appearance_get_sub", null ]
      ] ],
      [ "0x31 - sd_system_power_off", "a00111.html#nrf51_sd_power_off", [
        [ "Function packet format", "a00111.html#nrf41_nrf51_sd_power_off_sec_function", null ],
        [ "Response packet format", "a00111.html#sd_power_system_off", null ]
      ] ],
      [ "0xA4 - sd_ble_gatts_value_set", "a00111.html#nrf51_gatts_value_set_encoding", [
        [ "Function packet format", "a00111.html#nrf51_gatts_value_set_sec_function", null ],
        [ "Response packet format", "a00111.html#nrf51_gatts_value_set_sec_response", null ]
      ] ],
      [ "0xAA - sd_ble_gatts_sys_attr_get", "a00111.html#nrf51_gatts_sys_attr_get_encoding", [
        [ "Function packet format", "a00111.html#nrf51_gatts_sys_attr_get_sec_function", null ],
        [ "Response packet format", "a00111.html#nrf51_gatts_sys_attr_get_sec_response", null ]
      ] ],
      [ "0x65 - sd_ble_gap_conn_param_update", "a00111.html#nrf51_gap_conn_param_update_encoding", [
        [ "Function packet format", "a00111.html#nrf51_gap_conn_param_update_sec_function", null ],
        [ "Response packet format", "a00111.html#nrf51_gap_conn_param_update_sec_response", null ]
      ] ],
      [ "0x6B - sd_ble_gap_ppcp_get", "a00111.html#nrf51_gap_ppcp_get_encoding", [
        [ "Function packet format", "a00111.html#nrf51_gap_ppcp_get_sec_function", null ],
        [ "Response packet format", "a00111.html#nrf51_gap_ppcp_get_sub", null ]
      ] ],
      [ "0xA9 - sd_ble_gatts_sys_attr_set", "a00111.html#nrf51_gatts_sys_attr_set_encoding", [
        [ "Function packet format", "a00111.html#nrf51_gatts_sys_attr_set_sec_a", null ],
        [ "Response packet format", "a00111.html#nrf51_gatts_sys_attr_set_sec_response", null ]
      ] ],
      [ "0xA6 - sd_ble_gatts_hvx", "a00111.html#nrf51_gatts_hvx_encoding", [
        [ "Function packet format", "a00111.html#nrf51_gatts_hvx_sec_a", null ],
        [ "Response packet format", "a00111.html#nrf51_gatts_hvx_sec_response", null ]
      ] ]
    ] ]
];