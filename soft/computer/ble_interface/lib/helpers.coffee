

exports.hex_2_ascii = (buf) ->
    ret = ''
    for i in buf
        if i >= 0x10
            ret += i.toString(16)
        else
            ret += '0' + i.toString(16)
    return ret

exports.ascii_2_hex = (str) ->
    hex = str.toString()
    ret = new Buffer(6)
    for i in  [0..6]
        ret[i] = String.fromCharCode(parseInt(hex.substr(2*i, 2), 16)).charCodeAt()
    return ret


class Sender
    constructor: (@connection, @queue) ->
        return

    write: (buffer) =>
        @connection.publish @queue, buffer


class Receiver
    constructor: (@connection, @queue, @msg_handler) ->
        @connection.on 'ready', =>
            @connection.queue @queue, {durable: false, autoDelete: true, passive: false} ,(queue) =>
                queue.bind 'amq.direct', @queue

                queue.on 'queueBindOk', =>
                    console.log(' [*] Waiting for messages from nodeJS. To exit press CTRL+C')
                    queue.subscribe @msg_handler

exports.Sender = Sender
exports.Receiver = Receiver