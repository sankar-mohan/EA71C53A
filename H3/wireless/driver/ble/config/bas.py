def instantiateComponent(svcBLE_BASComponent):
    print('svcBLE_BASComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))

    # Battery level with Notify
    basNotifyEnable = svcBLE_BASComponent.createBooleanSymbol('BAS_NOTIFY_ENABLE', None)
    basNotifyEnable.setLabel('Enble Notify Property')
    basNotifyEnable.setDefaultValue(False)
    basNotifyEnable.setVisible(True)    
    
    # Add ble_bas.c file
    bleBasHeaderFile = svcBLE_BASComponent.createFileSymbol(None, None)
    bleBasHeaderFile.setSourcePath('driver/ble/templates/ble_bas.c.ftl')
    bleBasHeaderFile.setOutputName('ble_bas.c')
    bleBasHeaderFile.setOverwrite(True)
    bleBasHeaderFile.setDestPath('ble/service_ble/ble_bas')
    bleBasHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_bas')
    bleBasHeaderFile.setType('SOURCE')
    bleBasHeaderFile.setEnabled(True)
    bleBasHeaderFile.setMarkup(True)    

    # Add ble_bas.h file
    bleBasHeaderFile = svcBLE_BASComponent.createFileSymbol(None, None)
    bleBasHeaderFile.setSourcePath('driver/ble/templates/ble_bas.h.ftl')
    bleBasHeaderFile.setOutputName('ble_bas.h')
    bleBasHeaderFile.setOverwrite(True)
    bleBasHeaderFile.setDestPath('ble/service_ble/ble_bas')
    bleBasHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_bas')
    bleBasHeaderFile.setType('HEADER')
    bleBasHeaderFile.setEnabled(True)
    bleBasHeaderFile.setMarkup(True)      


def finalizeComponent(BLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(BLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['libBLEStack']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
