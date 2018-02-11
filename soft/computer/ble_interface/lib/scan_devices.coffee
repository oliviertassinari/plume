noble = require 'noble'

plume_uuid = '003000016f736577617369656d756c50'

noble.on 'stateChange', (state) ->
    if state is 'poweredOn'
        noble.startScanning()
    else
        noble.stopScanning()
    return

# do nothing

noble.on 'discover', (peripheral) ->
    #console.log "peripheral with UUID " + peripheral.uuid + " found"


peripherals_size = ->
    sum = 0
    for key of noble._peripherals
        sum++
    return sum

## function for the handler

exports.send_list_devices = (sender, data) ->
    duration = data[1] * 256 + data[2]
    noble.startScanning()
    console.log   parseInt duration.toString()
    setTimeout ->
        noble.stopScanning()

        response = ['d']
        response.push peripherals_size()
        for uuid of noble._peripherals
            response.push uuid #if noble._peripherals[uuid].
            console.log noble._peripherals[uuid].advertisement.serviceUuids


        console.log response.toString()
        sender.send response

    , duration