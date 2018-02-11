
## Init
#send_list_devices = require("./lib/scan_devices").send_list_devices
Plume = require('./lib/plume').Plume
helpers = require './lib/helpers'

ascii_2_hex = helpers.ascii_2_hex
hex_2_ascii = helpers.hex_2_ascii


## init sockets
my_host = 'localhost'
#my_host = 'ce-a406-10.enst.fr'
#rabbit = require('rabbit.js').createContext('amqp://137.194.23.99')

amqp = require 'amqp'
connection = amqp.createConnection { host: my_host, port: 5672 }


port_push = 'plume ble to calculator'
port_pull = 'plume calculator to ble'

push = new helpers.Sender(connection, port_push)
## socket events
# Message handler
pull = new helpers.Receiver connection, port_pull, (message) ->
    try
        data = message.data
        #data = arguments['0']
        mac = hex_2_ascii data[1..6]
        arg = data[7..]

        console.log '<< ' + mac + ' : Received: ' + data[0] + ' : args : ' + hex_2_ascii(arg)

        switch data[0]
            when 'd'.charCodeAt() then send_list_devices()
            when 's'.charCodeAt() then connect_to(mac, arg[0])
            when 'f'.charCodeAt() then set_frequency(mac, arg[0])
            when 'c'.charCodeAt() then set_calibration(mac, arg)
            when 'e'.charCodeAt() then set_stm(mac, arg[0])
            else
                console.log 'invalid code: ' + data[0]
                push.write 'z000000\x02'



    catch errr
        print errr


send_list_devices = ->
    Plume.send_devices(push)
    return

connect_to = (mac, state) ->
    #console.log mac
    peripheral = Plume.get_periph(mac, push,  notify_coils_callback)
    if peripheral
        switch state
            when Plume.states.DISCONNECTED
                peripheral.disconnect() if peripheral and typeof peripheral isnt 'undefined'
            when Plume.states.CONNECTED
                if peripheral.state isnt Plume.states.CONNECTED
                    peripheral.connect()

    #console.log mac + ' ' + state


set_frequency = (mac, frequency) ->
    peripheral = Plume.get_periph(mac, push, notify_coils_callback)
    if peripheral
        peripheral.set_frequency frequency


set_calibration = (mac, data) ->
    peripheral = Plume.get_periph(mac, push, notify_coils_callback)
    if peripheral
        peripheral.set_calibration data

set_stm = (mac, data) ->
    peripheral = Plume.get_periph(mac, push, notify_coils_callback)
    if peripheral
        peripheral.set_stm(data)


# decode the coils data received by ble
notify_coils_callback = (data, uuid_hexa) ->
    buf = new Buffer(1 + 6 + 2 + 36);
    buf[0] = 'v'.charCodeAt()

    # write mac address
    uuid_hexa.copy(buf, 1, 0)

    shift = data[0] >> 4

    # unused
    button = (data[0]) & 0x0E

    # write sign bits
    buf[7] = data[0] & 0x1 + button

    buf[8] = data[1]

    # write decoded data
    for i in [0..8]
        int = (data[2 + 2 * i] * 256 + data[3 + 2 * i]) << shift
        buf.writeUInt32BE(int, 9 + 4 * i, true)

    return buf
