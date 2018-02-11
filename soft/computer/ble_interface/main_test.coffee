
my_host = 'localhost'
#my_host = 'ce-a406-10.enst.fr'

port_push = 'plume calculator to ble'
port_pull = 'plume ble to calculator'

amqp = require 'amqp'
helpers = require './lib/helpers'
connection = amqp.createConnection { host: my_host, port: 5672 }

push = new helpers.Sender connection, port_push



ascii_2_hex = helpers.ascii_2_hex
hex_2_ascii = helpers.hex_2_ascii


macmac = [
    'c87fda6e6b3e',
    'c38f1f194bf7',
    'c52f397fd0b1',
    'db18369a7569',
    'dea5bea77a6f',
    'e6b3a059065c',
    'f59f2f10b721',
    'e3794ec582f5',
]


create_message_calib = (ty, mac, calib) ->
    if mac?
        buf = new Buffer(1 + 6 + 18);
        buf[0] = ty.charCodeAt()

        mac.copy(buf, 1, 0)



        #console.log buf

        #buf.write mac, 1, mac.length, 'ascii'
        return buf

hex_create_message = (ty, mac, state) ->
    if mac?
        buf = new Buffer(2 + 6);
        buf[0] = ty.charCodeAt()

        mac.copy(buf, 1, 0)

        buf[7] = state

        return buf

send_connect = (mac) ->
    push.write hex_create_message('s', mac, 1)

send_calib = (mac) ->
    func = ->
        buf = new Buffer(18)
        calib = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        for i in [0..17]
            buf[i] = 0
        return buf
    push.write create_message_calib('c', mac, func())

send_frequ = (mac) ->
    push.write hex_create_message('f', mac, 0x87)

tab = {}

x = 0
pull = new helpers.Receiver connection, port_pull, (reply) ->
    #reply = arguments[0]
    data = reply.data
    console.log "Received reply", x, ": [", data, "]"
    #console.log "Received reply", x, ": [", data.toString(), "]"
    x++
    if data[0] is 'z'.charCodeAt() or data[0] is 's'.charCodeAt()
        #mac = data[1..6]
        mac = ascii_2_hex macmac[7]
        console.log 'hi ' + hex_2_ascii mac
        if data[7] is 2
            send_connect(mac)
        if data[7] is 1
            if hex_2_ascii(mac) of tab
                console.log 'return'
                return
            else
                tab[hex_2_ascii mac] = 'set'
                console.log 'connected to ' + hex_2_ascii mac
                push.write hex_create_message('e', mac, 16)

                setTimeout ->
                    send_frequ(mac)
                , 500

                setTimeout ->
                    send_calib(mac)
                , 1000



                setTimeout ->
                    push.write hex_create_message('e', mac, 14)
                , 2000
    return


setTimeout ()->
    push.write 'z'
, 1000



