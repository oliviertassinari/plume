
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

STATE_CONNECTED = 1
STATE_DISCONNECTED = 2
timeout1 = 200
timeout2 = 3000

create_message = (ty, mac, state) ->
    if mac?
        buf = new Buffer(2 + 6);
        buf[0] = ty.charCodeAt()

        mac_buf = ascii_2_hex mac
        mac_buf.copy(buf, 1, 0)

        buf[buf.length - 1] = state

        #console.log buf

        #buf.write mac, 1, mac.length, 'ascii'
        return buf


send_delay = (period, buf) ->
    setTimeout ->
        push.write buf
        console.log 'Send [', buf , ']'
        #send_delay()
    , period

test = (state) ->
    for mac in macmac
        send_delay timeout1, create_message 's', mac, state

macstate = [0,0,0,0,0,0,0]

test2 = (id_mac) ->
    if 0 <= id_mac <= macmac.length
        mac = macmac[id_mac]
        if mac?

            get_state = (state) ->
                if state is STATE_CONNECTED
                    return STATE_DISCONNECTED
                else
                    return STATE_CONNECTED

            state = get_state(macstate[id_mac])
            send_delay timeout1, create_message 's', mac, state
            macstate[id_mac] = state

test3 =  ->
    for id_mac in [0..macmac.length]
        mac = macmac[id_mac]
        state = macstate[id_mac]
        (send_delay timeout1, create_message 'f', mac, 150) if state is 1

test4 = ->
    push.write 'd'
    
    
test5 = (code, state) ->
    mac = macmac[5]
    send_delay timeout1, create_message code, mac, state
        

#test(1)
stdin = process.stdin
stdin.setRawMode true
stdin.resume()
stdin.setEncoding 'utf8'

stdin.on 'data', (key) ->
    switch key
        when '\u0003' then process.exit()
        when 'f' then test3()
        when 'd' then test4()
        when 'a' then test5 's', 1
        when 'z' then test5 's', 2
        when 'q' then test5 'e', 0x10 #standby
        when 's' then test5 'e', 0x0f #off
        when 'w' then test5 'e', 0x0e #start

        else
            test2 key.charCodeAt() - 0x30

# receive handler
x = 0
#pull.on "data", () ->
pull = new helpers.Receiver connection, port_pull, (reply) ->
    #reply = arguments[0]

    console.log "Received reply", x, ": [", reply.data, "]"
    console.log "Received reply", x, ": [", reply.data.toString(), "]"
    x++
    return