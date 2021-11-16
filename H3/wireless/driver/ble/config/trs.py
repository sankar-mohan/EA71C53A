def instantiateComponent(svcBLE_TRSComponent):
    print('svcBLE_TRSComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))

    # Service UUID
    trsServiceUUID = svcBLE_TRSComponent.createStringSymbol("TRS_SERVICE_UUID", None)
    trsServiceUUID.setLabel('Service UUID')
    trsServiceUUID.setDefaultValue("0x55,0xE4,0x05,0xD2,0xAF,0x9F,0xA9,0x8F,0xE5,0x4A,0x7D,0xFE,0x43,0x53,0x53,0x49")
    trsServiceUUID.setVisible(True)
    trsServiceUUID.setReadOnly(False)
    
    # TX Char UUID
    trsServiceTxUUID = svcBLE_TRSComponent.createStringSymbol("TRS_TX_UUID", None)
    trsServiceTxUUID.setLabel('TX Characteristic UUID')
    trsServiceTxUUID.setDefaultValue("0x16,0x96,0x24,0x47,0xC6,0x23,0x61,0xBA,0xD9,0x4B,0x4D,0x1E,0x43,0x53,0x53,0x49")
    trsServiceTxUUID.setVisible(True)
    trsServiceTxUUID.setReadOnly(False)
    
    # RX Char UUID
    trsServiceRxUUID = svcBLE_TRSComponent.createStringSymbol("TRS_RX_UUID", None)
    trsServiceRxUUID.setLabel('RX Characteristic UUID')
    trsServiceRxUUID.setDefaultValue("0xB3,0x9B,0x72,0x34,0xBE,0xEC,0xD4,0xA8,0xF4,0x43,0x41,0x88,0x43,0x53,0x53,0x49")
    trsServiceRxUUID.setVisible(True)
    trsServiceRxUUID.setReadOnly(False)
    
    # CP Char UUID
    trsServiceCpUUID = svcBLE_TRSComponent.createStringSymbol("TRS_CP_UUID", None)
    trsServiceCpUUID.setLabel('CP Characteristic UUID')
    trsServiceCpUUID.setDefaultValue("0x7e,0x3b,0x07,0xff,0x1c,0x51,0x49,0x2f,0xb3,0x39,0x8a,0x4c,0x43,0x53,0x53,0x49")
    trsServiceCpUUID.setVisible(True)
    trsServiceCpUUID.setReadOnly(False)
    
    # Add ble_trs.c file
    bleTrsSourceFile = svcBLE_TRSComponent.createFileSymbol(None, None)
    bleTrsSourceFile.setSourcePath('driver/ble/templates/ble_trs.c.ftl')
    bleTrsSourceFile.setOutputName('ble_trs.c')
    bleTrsSourceFile.setOverwrite(True)
    bleTrsSourceFile.setDestPath('ble/service_ble/ble_trs')
    bleTrsSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_trs')
    bleTrsSourceFile.setType('SOURCE')
    bleTrsSourceFile.setEnabled(True)
    bleTrsSourceFile.setMarkup(True)

    # Add ble_trs.h file
    bleTrsHeaderFile = svcBLE_TRSComponent.createFileSymbol(None, None)
    bleTrsHeaderFile.setSourcePath('driver/ble/templates/ble_trs.h.ftl')
    bleTrsHeaderFile.setOutputName('ble_trs.h')
    bleTrsHeaderFile.setOverwrite(True)
    bleTrsHeaderFile.setDestPath('ble/service_ble/ble_trs')
    bleTrsHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_trs')
    bleTrsHeaderFile.setType('HEADER')
    bleTrsHeaderFile.setEnabled(True)
    bleTrsHeaderFile.setMarkup(True)   


def finalizeComponent(BLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(BLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['libBLEStack']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
