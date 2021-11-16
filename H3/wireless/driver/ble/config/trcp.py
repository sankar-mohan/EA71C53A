def bleTrcpsEnable(symbol, event):
    symbol.setEnabled(event["value"])

#def bleTrcpcEnable(symbol, event):
#    symbol.setEnabled(event["value"])
    
    
def instantiateComponent(profileBLE_TRCP_Component):
    print('profileBLE_TRCP_Component')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))

    #################################################################
    ##################      Client Role Settings      ###############
    #################################################################
    menuClient = profileBLE_TRCP_Component.createBooleanSymbol('TRCP_BOOL_CLIENT', None)
    menuClient.setLabel('Enable Client Role')
    menuClient.setDefaultValue(False)
    menuClient.setVisible(False)
    
       
    #################################################################
    ##################      Server Role Settings      ###############
    #################################################################
    menuSERVER = profileBLE_TRCP_Component.createBooleanSymbol('TRCP_BOOL_SERVER', None)
    menuSERVER.setLabel('Enable Server Role')
    menuSERVER.setDefaultValue(False)


    #################################################################
    ##################        Add Source File         ###############
    #################################################################

    # Add ble_trcbps.c file
    bleTrcbpsSourceFile = profileBLE_TRCP_Component.createFileSymbol(None, None)
    bleTrcbpsSourceFile.setSourcePath('driver/ble/src/ble_src/profile_ble/ble_trcbps/ble_trcbps.c')
    bleTrcbpsSourceFile.setOutputName('ble_trcbps.c')
    bleTrcbpsSourceFile.setOverwrite(True)
    bleTrcbpsSourceFile.setDestPath('ble/profile_ble/ble_trcbps/')
    bleTrcbpsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trcbps/')
    bleTrcbpsSourceFile.setType('SOURCE')
    bleTrcbpsSourceFile.setEnabled(False)
    bleTrcbpsSourceFile.setMarkup(True)
    bleTrcbpsSourceFile.setDependencies(bleTrcpsEnable, ["TRCP_BOOL_SERVER"])

    # Add ble_trcbps.h file
    bleTrcbpsHeaderFile = profileBLE_TRCP_Component.createFileSymbol(None, None)
    bleTrcbpsHeaderFile.setSourcePath('driver/ble/src/ble_src/profile_ble/ble_trcbps/ble_trcbps.h')
    bleTrcbpsHeaderFile.setOutputName('ble_trcbps.h')
    bleTrcbpsHeaderFile.setOverwrite(True)
    bleTrcbpsHeaderFile.setDestPath('ble/profile_ble/ble_trcbps/')
    bleTrcbpsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trcbps/')
    bleTrcbpsHeaderFile.setType('HEADER')
    bleTrcbpsHeaderFile.setEnabled(False)
    bleTrcbpsHeaderFile.setMarkup(True)
    bleTrcbpsHeaderFile.setDependencies(bleTrcpsEnable, ["TRCP_BOOL_SERVER"])

    # Add app_trcbps.c file - static file
    bleTrcbpsAppSourceFile = profileBLE_TRCP_Component.createFileSymbol(None, None)
    bleTrcbpsAppSourceFile.setSourcePath('driver/ble/src/app_trcbps_handler.c')
    bleTrcbpsAppSourceFile.setOutputName('app_trcbps_handler.c')
    bleTrcbpsAppSourceFile.setOverwrite(True)
    bleTrcbpsAppSourceFile.setDestPath('../../app_ble')
    bleTrcbpsAppSourceFile.setProjectPath('app_ble')
    bleTrcbpsAppSourceFile.setType('Source')
    bleTrcbpsAppSourceFile.setEnabled(False)
    bleTrcbpsAppSourceFile.setDependencies(bleTrcpsEnable, ["TRCP_BOOL_SERVER"])
    
    # Add app_trcbps.h file - static file
    bleTrcbpsAppHeaderFile = profileBLE_TRCP_Component.createFileSymbol(None, None)
    bleTrcbpsAppHeaderFile.setSourcePath('driver/ble/src/app_trcbps_handler.h')
    bleTrcbpsAppHeaderFile.setOutputName('app_trcbps_handler.h')
    bleTrcbpsAppHeaderFile.setOverwrite(True)
    bleTrcbpsAppHeaderFile.setDestPath('../../app_ble')
    bleTrcbpsAppHeaderFile.setProjectPath('app_ble')
    bleTrcbpsAppHeaderFile.setType('HEADER')
    bleTrcbpsAppHeaderFile.setEnabled(False)
    bleTrcbpsAppHeaderFile.setDependencies(bleTrcpsEnable, ["TRCP_BOOL_SERVER"])
      
    # Add ble_trcbpc.c file
    #bleTrcbpsSourceFile = profileBLE_TRCP_Component.createFileSymbol(None, None)
    #bleTrcbpsSourceFile.setSourcePath('driver/ble/src/ble_src/profile_ble/ble_trcbpc/ble_trcbpc.c')
    #bleTrcbpsSourceFile.setOutputName('ble_trcbpc.c')
    #bleTrcbpsSourceFile.setOverwrite(True)
    #bleTrcbpsSourceFile.setDestPath('ble/profile_ble/ble_trcbpc/')
    #bleTrcbpsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trcbpc/')
    #bleTrcbpsSourceFile.setType('SOURCE')
    #bleTrcbpsSourceFile.setEnabled(False)
    #bleTrcbpsSourceFile.setMarkup(True)
    #bleTrcbpsSourceFile.setDependencies(bleTrcpcEnable, ["TRCP_BOOL_CLIENT"])

    # Add ble_trcbpc.h file
    #bleTrcbpsHeaderFile = profileBLE_TRCP_Component.createFileSymbol(None, None)
    #bleTrcbpsHeaderFile.setSourcePath('driver/ble/src/ble_src/profile_ble/ble_trcbpc/ble_trcbpc.h')
    #bleTrcbpsHeaderFile.setOutputName('ble_trcbpc.h')
    #bleTrcbpsHeaderFile.setOverwrite(True)
    #bleTrcbpsHeaderFile.setDestPath('ble/profile_ble/ble_trcbpc/')
    #bleTrcbpsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trcbpc/')
    #bleTrcbpsHeaderFile.setType('HEADER')
    #bleTrcbpsHeaderFile.setEnabled(False)
    #bleTrcbpsHeaderFile.setMarkup(True)
    #bleTrcbpsHeaderFile.setDependencies(bleTrcpcEnable, ["TRCP_BOOL_CLIENT"])    

    # Add app_trcbpC.c file - static file
    #bleTrcbpcAppSourceFile = profileBLE_TRCP_Component.createFileSymbol(None, None)
    #bleTrcbpcAppSourceFile.setSourcePath('driver/ble/src/app_trcbpc_handler.c')
    #bleTrcbpcAppSourceFile.setOutputName('app_trcbpc_handler.c')
    #bleTrcbpcAppSourceFile.setOverwrite(True)
    #bleTrcbpcAppSourceFile.setDestPath('../../')
    #bleTrcbpcAppSourceFile.setProjectPath('')
    #bleTrcbpcAppSourceFile.setType('Source')
    #bleTrcbpcAppSourceFile.setEnabled(True)
    #bleTrcbpcAppSourceFile.setDependencies(bleTrcpcEnable, ["TRCP_BOOL_CLIENT"])
    
    # Add app_trcbpC.h file - static file
    #bleTrcbpcAppHeaderFile = profileBLE_TRCP_Component.createFileSymbol(None, None)
    #bleTrcbpcAppHeaderFile.setSourcePath('driver/ble/src/app_trcbpc_handler.h')
    #bleTrcbpcAppHeaderFile.setOutputName('app_trcbpc_handler.h')
    #bleTrcbpcAppHeaderFile.setOverwrite(True)
    #bleTrcbpcAppHeaderFile.setDestPath('../../')
    #bleTrcbpcAppHeaderFile.setProjectPath('')
    #bleTrcbpcAppHeaderFile.setType('HEADER')
    #bleTrcbpcAppHeaderFile.setEnabled(True)
    #bleTrcbpcAppHeaderFile.setDependencies(bleTrcpcEnable, ["TRCP_BOOL_SERVER"])    


def finalizeComponent(BLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(BLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['svcBLE_TRCS']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
