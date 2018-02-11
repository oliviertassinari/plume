# For the install

     List of command to use

>        make npm_install

    And to run the script (use sudo if you don't have HCI rights)

>        iced <script_name>


    Additional package

>        async
>        amqp
>        noble

# Communications details (BLE)
    
    Service 1 (name ? : coils)
	
>	Characteristic 1 : Coils_Data
>>  20 bytes, READ & NOTIFY
>>
>>	Notify the codec values

>	Characteristic 2 : Sample_Rate (smpr)
>>	1 byte , READ & WRITE
>>
>>	Set the sample rate value

>	Characteristic 3 : Calibration_data (calib)
>>	18 bytes, READ & WRITE
>>
>>	Set the calibration constants

>	Characteristic 4 : Change_stm_state (stm_state)
>>	1 bytes, READ & WRITE
>>
>>	Set the state of the stm

    Service 2 : Battery service

>   Characteristic 1 : Battery level
>>  1 byte, NOTIFY
>>
>>	Notify the battery level

# Communications details (NODEJS)

![Zozor](http://uploads.siteduzero.com/files/420001_421000/420263.png)

    Nodejs functions

>
>        mac_address : <uint8_t [6]>

>    get_mac_devices (void)
>>   return a list of address of devices advertising the previously defined services
>>
>>   socket message : `d"
>>
>>   socket response : `d<mac_address_1>`
>>
>>   ...
>>
>>   socket response : `d<mac_address_n>`

>    ble_connection_state (mac_address, state)
>>       state: <uint8_t>
>>   state 1 `Connect`
>>
>>   state 2 `Disconnect`
>>
>>   connect to <mac_address> device and enable battery and coils notifications
>>
>>   socket message :  `s<mac_address>1`

>    set_sample_rate (mac_address, smpr)
>>       smpr : <uint8_t>
>>
>>   socket message : `f<mac_address><smpr>`

>    set_calibration_data (mac_address, data[18])
>>       data: <uint8_t [18] >
>>
>>   socket message : `c<mac_address><data>`

>    set_stm_state (mac_address, state)
>>       state: <uint8_t>
>>   state 1 `Start`
>>
>>   state 2 `Off`
>>
>>   state 3 `Standby`
>>
>>   socket message : `e<mac_address><data>`
