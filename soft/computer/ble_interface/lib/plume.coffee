## Init library
noble = require 'noble'
ascii_2_hex = require('./helpers').ascii_2_hex
hex_2_ascii = require('./helpers').hex_2_ascii

noble.on 'stateChange', (state) ->
    if state is 'poweredOn'
        noble.startScanning()
    else
        noble.stopScanning()

    console.log state
    return

###
noble.on 'scanStart', ->
    timeo = 10000
    noble._peripherals = {}
    noble._peripheralUuid = []
    noble._discoveredPeripheralUUids = []
    setTimeout ->
        noble.stopScanning()
    , timeo

noble.on 'scanStop', ->
    timeo = 10000
    setTimeout ->
        noble.startScanning()
    , timeo

###
# Class representing the plume peripheral.
ERROR_CONNECT_RETRY_TIMEOUT = 10000
ERROR_COUNT_MAX = 10

uuid_plume_service =    '000000006f736577617369656d756c50'
uuid_battery_service =  '108f'

uuid_battery_char =     '2a19'
uuid_coils_char =       '000000016f736577617369656d756c50'
uuid_frequency_char =   '000000026f736577617369656d756c50'
uuid_calibration_char = '000000036f736577617369656d756c50'
uuid_stm_char =         '000000046f736577617369656d756c50'

create_state_message = (uuid_buffer, state) ->
    buf = new Buffer(1 + 6 + 1);
    buf[0] = 's'.charCodeAt()
    uuid_buffer.copy(buf, 1, 0)
    buf[7] = state
    return buf

class Plume
    # a list of plumes
    @connected_peripherals = {}
    @states = {
        UNKNOWN:      0
        CONNECTED:    1
        DISCONNECTED: 2
        TRYING:       3
    }
    @discovering = false

    constructor: (@peripheral, @push, @coils_handler) ->
        @called = false
        @error_count = 0
        @state = Plume.states.DISCONNECTED
        @trying = false
        @discovered_characteristics = {}
        @uuid_hexa = ascii_2_hex @peripheral.uuid
        @peripheral.on 'disconnect', () =>
            if @state is Plume.states.CONNECTED
                console.log @toStr("BLE <<", 'Deconnected')
                @state = Plume.states.DISCONNECTED
                @writeState()
                @error_count = 0

        @peripheral.on 'connect', (error) =>
            if error and error.toString() is 'Error: Transport endpoint is not connected'
                console.log('if :: ' + error.toString())
            else #if error is '[Error: Peripheral already connected]'
                console.log('else  :: ' + error.toString()) if error
                console.log @toStr('!', 'event connect ' + @state)

                try_connect = =>
                    #@state = Plume.states.TRYING
                    console.log @toStr("BLE <<", 'Connected')
                    @error_count = 0


                    console.log @toStr('!', 'before getting chars')
                    @__get_all_characteristics (characteristics) =>
                        console.log @toStr('!', 'into getting chars')
                        if uuid_coils_char of characteristics

                            characteristics[uuid_coils_char].notify true
                            characteristics[uuid_coils_char].on 'read', (data, is_notif) =>
                                @handle_coils data
                        else
                            console.log 'error on get_all_characteristics - uuid_coils'

                        #ret_cb = =>
                        @state = Plume.states.CONNECTED
                        #@writeState
                        tosend = create_state_message(@uuid_hexa, @state)
                        console.log @toStr("BLE >>", 'Sent: ' + hex_2_ascii(tosend))
                        @push.write tosend
                        console.log @toStr("STA ::", 'ok connecte toi')
                        #ret_cb()

                        #if @state is Plume.states.TRYING
                        #    @state = Plume.states.DISCONNECTED
                setTimeout try_connect, 10
                        #if @state is Plume.states.DISCONNECTED# or Plume.states.TRYING

                        #            #setTimeout(ret_cb, 0)
                        #    if not @called
                        #        @called = true
                        #        try_connect()

    writeState: =>
        tosend = create_state_message(@uuid_hexa, @state)
        console.log @toStr("PC  >>", 'Sent: ' + hex_2_ascii(tosend))
        @push.write tosend

    # The connect function with a retry
    connect: () =>
        if (@state is Plume.states.DISCONNECTED or @state is Plume.states.TRYING)and @peripheral? and typeof @peripheral isnt 'undefined'
            @peripheral.connect (error) =>
                if error
                    @error_count++
                    setTimeout(@connect, ERROR_CONNECT_RETRY_TIMEOUT) if @error_count < ERROR_COUNT_MAX

                    if @error_count >= ERROR_COUNT_MAX
                        console.log @toStr("STA ::", 'connect error + error_count : ' + @error_count)
                        @writeState()
                        @error_count = 0

                    return

        else if @state is Plume.states.CONNECTED
            @writeState()
        else
            @state = Plume.states.DISCONNECTED
            @writeState()


    # The disconnect function with a retry
    disconnect: () =>
        if @state is Plume.states.CONNECTED and @peripheral? and typeof @peripheral isnt 'undefined'
            @peripheral.disconnect (error) =>
                if error
                    @error_count++
                    setTimeout(@disconnect, ERROR_CONNECT_RETRY_TIMEOUT) if @error_count < ERROR_COUNT_MAX

                    if @error_count >= ERROR_COUNT_MAX
                        console.log @toStr("STA ::", ' disconnect error + error_count : ' + @error_count)
                        @writeState()
                        @error_count = 0
        else if @state is Plume.states.DISCONNECTED
            @writeState()

    # Write the frequency ( 8 bits )
    set_frequency : (frequency) =>
        console.log @toStr("BLE >>", 'writing frequency: ' + frequency)

        if @state is Plume.states.CONNECTED
            freq_buffer = new Buffer(1)
            freq_buffer[0] = frequency
            @discovered_characteristics[uuid_frequency_char].write freq_buffer, (err) =>
                console.log err if err
                return
            @writeState()
        else
            @writeState()
        return

    handle_coils : (data) =>
        #console.log @toStr("BLE <<", 'coils notify received: ' + data)

        buffer = @coils_handler(data, @uuid_hexa)
        return @push.write buffer

    set_stm: (stm) =>
        console.log @toStr("STA ::", 'writing stm_enable: ' + stm)

        if @state is Plume.states.CONNECTED
            stm_buffer = new Buffer(1)
            stm_buffer[0] = stm
            @discovered_characteristics[uuid_stm_char].write stm_buffer, (err) =>
                console.log err if err
                console.log @toStr("STA ::", 'ok : in set_stm')
                return
            @writeState()
        else
            @writeState()

    set_calibration: (calibration_buffer) =>
        if @state is Plume.states.CONNECTED
            err_cb = (err) =>
                console.log err if err
                console.log @toStr("STA ::", 'ok : in set_calibration')
                return
            if calibration_buffer.length is 18
                @discovered_characteristics[uuid_calibration_char].write(calibration_buffer, err_cb)
                console.log @toStr("PC  << ", 'got calibration: ' + hex_2_ascii(calibration_buffer))
            else
                console.log 'err got data_length of ' + calibration_buffer.length
            @writeState()
        else
            @writeState()

    ## Helper functions

    __get_all_characteristics : (callback) =>

        discover = =>
            @peripheral.discoverAllServicesAndCharacteristics (err, services, chars) =>
                throw err if err
                for elem in chars
                    @discovered_characteristics[elem.uuid] = elem
                console.log @toStr("STA ::", 'assign discovered chars to member of Plume')
                callback @discovered_characteristics

        @discovered_characteristics = {}
        discover()

    toStr: (pref, msg) =>
        return Plume.tostring(pref, @peripheral.uuid, msg)

    ## Static functions

    @tostring: (pref, mac, msg) ->
        return pref + ' ' + mac + ' : ' + msg

    # a static "constructor" creating an instance if it needs to
    @get_periph = (mac, push, coils_cb) ->
        periph = @connected_peripherals[mac]
        if !periph
            if noble._peripherals[mac]
                periph = @connected_peripherals[mac] = new Plume(noble._peripherals[mac], push, coils_cb)
                return periph
            else
                push.write create_state_message(ascii_2_hex(mac), Plume.states.UNKNOWN) # no device
                return null
        else
            return periph

    @send_devices = (push) ->
        for periphUuid of noble._peripherals
            if uuid_plume_service in noble._peripherals[periphUuid].advertisement.serviceUuids
                #console.log noble._peripherals[periphUuid].advertisement.serviceUuids

                if periphUuid of @connected_peripherals
                    setTimeout(@connected_peripherals[periphUuid].writeState, 500)

                else
                    push.write create_state_message(ascii_2_hex(periphUuid), Plume.states.DISCONNECTED) # free device
                console.log 'send_devices ' + periphUuid

exports.Plume = Plume