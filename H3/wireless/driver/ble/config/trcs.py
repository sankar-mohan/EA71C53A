def instantiateComponent(svcBLE_TRCSComponent):
    print('svcBLE_TRCSComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))

    # Service UUID
    trcsServiceUUID = svcBLE_TRCSComponent.createStringSymbol("TRCS_SERVICE_UUID", None)
    trcsServiceUUID.setLabel('Service UUID')
    trcsServiceUUID.setDefaultValue("0x50,0xEC,0xED,0x1A,0xA0,0xE8,0xDB,0xBD,0xFC,0x45,0x20,0x21,0x43,0x53,0x53,0x49")
    trcsServiceUUID.setVisible(True)
    trcsServiceUUID.setReadOnly(False)
    
    # PSM UUID
    trcsPsmTxUUID = svcBLE_TRCSComponent.createStringSymbol("TRCS_PSM_UUID", None)
    trcsPsmTxUUID.setLabel('PSM UUID')
    trcsPsmTxUUID.setDefaultValue("0x1F,0xDD,0x25,0x3B,0xC1,0x68,0x9F,0x9A,0x91,0x49,0xDB,0xC2,0x43,0x53,0x53,0x49")
    trcsPsmTxUUID.setVisible(True)
    trcsPsmTxUUID.setReadOnly(False)

    # Ctrl UUID
    trcsCtrlUUID = svcBLE_TRCSComponent.createStringSymbol("TRCS_CTRL_UUID", None)
    trcsCtrlUUID.setLabel('CTRL UUID')
    trcsCtrlUUID.setDefaultValue("0x3C,0xD0,0xF7,0x1A,0xE9,0x35,0x46,0x1E,0xAE,0x18,0x84,0x02,0x43,0x53,0x53,0x49")
    trcsCtrlUUID.setVisible(True)
    trcsCtrlUUID.setReadOnly(False)
        
    # Add ble_trcbs.c file
    bleTrcbsSourceFile = svcBLE_TRCSComponent.createFileSymbol(None, None)
    bleTrcbsSourceFile.setSourcePath('driver/ble/templates/ble_trcbs.c.ftl')
    bleTrcbsSourceFile.setOutputName('ble_trcbs.c')
    bleTrcbsSourceFile.setOverwrite(True)
    bleTrcbsSourceFile.setDestPath('ble/service_ble/ble_trcbs')
    bleTrcbsSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_trcbs')
    bleTrcbsSourceFile.setType('SOURCE')
    bleTrcbsSourceFile.setEnabled(True)
    bleTrcbsSourceFile.setMarkup(True)

    # Add ble_trcbs.h file
    bleTrcbsHeaderFile = svcBLE_TRCSComponent.createFileSymbol(None, None)
    bleTrcbsHeaderFile.setSourcePath('driver/ble/templates/ble_trcbs.h.ftl')
    bleTrcbsHeaderFile.setOutputName('ble_trcbs.h')
    bleTrcbsHeaderFile.setOverwrite(True)
    bleTrcbsHeaderFile.setDestPath('ble/service_ble/ble_trcbs')
    bleTrcbsHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_trcbs')
    bleTrcbsHeaderFile.setType('HEADER')
    bleTrcbsHeaderFile.setEnabled(True)
    bleTrcbsHeaderFile.setMarkup(True)    


def finalizeComponent(BLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(BLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['libBLEStack']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
