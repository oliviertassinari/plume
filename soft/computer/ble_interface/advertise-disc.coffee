noble = require("noble")

noble.on "stateChange", (state) ->
    if state is "poweredOn"
        noble.startScanning()
    else
        noble.stopScanning()
    return

noble.on "discover", (peripheral) ->
    console.log "peripheral discovered (" + peripheral.uuid + "):"
    console.log "\thello my local name is:"
    console.log "\t\t" + peripheral.advertisement.localName
    console.log "\tcan I interest you in any of the following advertised services:"
    console.log "\t\t" + JSON.stringify(peripheral.advertisement.serviceUuids)
    serviceData = peripheral.advertisement.serviceData
    if serviceData and serviceData.length
        console.log "\there is my service data:"
        for i of serviceData
            console.log "\t\t" + JSON.stringify(serviceData[i].uuid) + ": " + JSON.stringify(serviceData[i].data.toString("hex"))
    if peripheral.advertisement.manufacturerData
        console.log "\there is my manufacturer data:"
        console.log "\t\t" + JSON.stringify(peripheral.advertisement.manufacturerData.toString("hex"))
    if peripheral.advertisement.txPowerLevel isnt `undefined`
        console.log "\tmy TX power level is:"
        console.log "\t\t" + peripheral.advertisement.txPowerLevel
    console.log()
    return