async = require "async"

notify = (peripheral, notify_cb, cb) ->
    console.log()
    console.log "   #### Entering notify"
    console.log(peripheral.toString())
    #characteristic.write(data, notify[, callback(error)]);
    serviceUUIDs = ["ff9f",]
    characteristicUUIDs = ["ff9e",]

    peripheral.on "disconnect", ->
        return

    console.log "services and characteristics:"
    peripheral.connect (error) ->
        peripheral.discoverServices serviceUUIDs, (error, services) ->
                serviceIndex = 0
                async.whilst ( ->
                    serviceIndex < services.length
                ),
                ((callback) ->
                    service = services[serviceIndex]
                    serviceInfo = service.uuid
                    serviceInfo += " (" + service.name + ")"  if service.name
                    console.log serviceInfo

                    service.discoverCharacteristics characteristicUUIDs, (error, characteristics) ->
                        characteristicIndex = 0
                        async.whilst (
                            -> characteristicIndex < characteristics.length
                        ),
                        ((callback) ->
                            characteristic = characteristics[characteristicIndex]
                            charInfo = "    " + characteristic.uuid
                            charInfo += " (" + characteristic.name + ")" if characteristic.name
                            console.log charInfo

                            characteristic.notify true
                            characteristic.on "read", (data, isNotification) ->
                                notify_cb data if isNotification


                            characteristicIndex++
                            callback()
                            return
                        ), ((err) ->
                            serviceIndex++
                            callback()
                        )



                    return
                ), ((err) ->
                    #peripheral.disconnect()
                    cb err
                )
                return
        return

exports.notify = notify