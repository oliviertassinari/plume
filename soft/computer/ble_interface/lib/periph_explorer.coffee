async = require "async"

explore = (peripheral, cb) ->
    console.log()
    console.log "   ### Entering Explore"
    console.log "services and characteristics:"
    peripheral.on "disconnect", ->
        return

    peripheral.connect (error) ->
        peripheral.discoverServices [], (error, services) ->
            serviceIndex = 0
            async.whilst ( ->
                serviceIndex < services.length
            ), ((callback) ->
                service = services[serviceIndex]
                serviceInfo = service.uuid
                serviceInfo += " (" + service.name + ")"  if service.name
                console.log serviceInfo

                service.discoverCharacteristics [], (error, characteristics) ->
                    characteristicIndex = 0
                    async.whilst ( ->
                            characteristicIndex < characteristics.length
                    ),
                    ((callback) ->
                        characteristic = characteristics[characteristicIndex]
                        characteristicInfo = "  " + characteristic.uuid
                        characteristicInfo += " (" + characteristic.name + ")"  if characteristic.name
                        async.series [
                            (callback) ->
                                characteristic.discoverDescriptors (error, descriptors) ->
                                    async.detect descriptors, ((descriptor, callback) ->
                                        callback descriptor.uuid is "2901"
                                    ),
                                    (userDescriptionDescriptor) ->
                                        if userDescriptionDescriptor
                                            userDescriptionDescriptor.readValue (error, data) ->
                                                characteristicInfo += " (" + data.toString() + ")"
                                                callback()
                                                return
                                        else
                                            callback()
                                        return

                                    return

                            (callback) ->
                                characteristicInfo += "\n    properties  " + characteristic.properties.join(", ")
                                if characteristic.properties.indexOf("read") isnt -1
                                    characteristic.read (error, data) ->
                                        if data
                                            string = data.toString("ascii")
                                            characteristicInfo += "\n    value       " + data.toString("hex") + " | '" + string + "'"
                                        callback()
                                else
                                    callback()
                            ->
                                console.log characteristicInfo
                                characteristicIndex++
                                callback()
                                return
                        ]
                        return
                    ), (error) ->
                        serviceIndex++
                        callback()
                        return

                    return

                return
            ), (err) ->
                #peripheral.disconnect ->
                cb err

                return

            return

        return
    return

exports.explore = explore