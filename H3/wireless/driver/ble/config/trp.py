global menuClient
global menuServer
def bleTrpcEnable(symbol, event):
    symbol.setEnabled(event["value"])

def bleTrpsEnable(symbol, event):
    symbol.setEnabled(event["value"])

def instantiateComponent(profileBLE_TRP_Component):
    print('profileBLE_TRP_Component')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))

    #################################################################
    ##################      Client Role Settings      ###############
    #################################################################
    menuClient = profileBLE_TRP_Component.createBooleanSymbol('TRP_BOOL_CLIENT', None)
    menuClient.setLabel('Enable Client Role')
    menuClient.setDefaultValue(False)

    #################################################################
    ##################      Server Role Settings      ###############
    #################################################################
    menuServer = profileBLE_TRP_Component.createBooleanSymbol('TRP_BOOL_SERVER', None)
    menuServer.setLabel('Enable Server Role')
    menuServer.setDefaultValue(False)

    #################################################################
    ##################        Add Source File         ###############
    #################################################################
    
    # Add ble_trsps.c file - static file
    bleTrpsSourceFile = profileBLE_TRP_Component.createFileSymbol(None, None)
    bleTrpsSourceFile.setSourcePath('driver/ble/src/ble_src/profile_ble/ble_trsps/ble_trsps.c')
    bleTrpsSourceFile.setOutputName('ble_trsps.c')
    bleTrpsSourceFile.setOverwrite(True)
    bleTrpsSourceFile.setDestPath('ble/profile_ble/ble_trsps/')
    bleTrpsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trsps/')
    bleTrpsSourceFile.setType('SOURCE')
    bleTrpsSourceFile.setEnabled(False)
    bleTrpsSourceFile.setMarkup(True)
    bleTrpsSourceFile.setDependencies(bleTrpsEnable, ["TRP_BOOL_SERVER"])
    
    # Add ble_trsps.h file - static file
    bleTrpsHeaderFile = profileBLE_TRP_Component.createFileSymbol(None, None)
    bleTrpsHeaderFile.setSourcePath('driver/ble/src/ble_src/profile_ble/ble_trsps/ble_trsps.h')
    bleTrpsHeaderFile.setOutputName('ble_trsps.h')
    bleTrpsHeaderFile.setOverwrite(True)
    bleTrpsHeaderFile.setDestPath('ble/profile_ble/ble_trsps/')
    bleTrpsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trsps/')
    bleTrpsHeaderFile.setType('HEADER')
    bleTrpsHeaderFile.setEnabled(False)
    bleTrpsHeaderFile.setMarkup(True)
    bleTrpsHeaderFile.setDependencies(bleTrpsEnable, ["TRP_BOOL_SERVER"])
    
    # Add app_trsps_handler.c file - static file
    bleTrpsAppSourceFile = profileBLE_TRP_Component.createFileSymbol(None, None)
    bleTrpsAppSourceFile.setSourcePath('driver/ble/src/app_trsps_handler.c')
    bleTrpsAppSourceFile.setOutputName('app_trsps_handler.c')
    bleTrpsAppSourceFile.setOverwrite(True)
    bleTrpsAppSourceFile.setDestPath('../../app_ble')
    bleTrpsAppSourceFile.setProjectPath('app_ble')
    bleTrpsAppSourceFile.setType('Source')
    bleTrpsAppSourceFile.setEnabled(False)
    bleTrpsAppSourceFile.setDependencies(bleTrpsEnable, ["TRP_BOOL_SERVER"])
    
    # Add app_trsps_handler.h file - static file
    bleTrpsAppHeaderFile = profileBLE_TRP_Component.createFileSymbol(None, None)
    bleTrpsAppHeaderFile.setSourcePath('driver/ble/src/app_trsps_handler.h')
    bleTrpsAppHeaderFile.setOutputName('app_trsps_handler.h')
    bleTrpsAppHeaderFile.setOverwrite(True)
    bleTrpsAppHeaderFile.setDestPath('../../app_ble')
    bleTrpsAppHeaderFile.setProjectPath('app_ble')
    bleTrpsAppHeaderFile.setType('HEADER')
    bleTrpsAppHeaderFile.setEnabled(False)
    bleTrpsAppHeaderFile.setDependencies(bleTrpsEnable, ["TRP_BOOL_SERVER"])
  
    
    # Add ble_trspc.c file - static file
    bleTrpcSourceFile = profileBLE_TRP_Component.createFileSymbol(None, None)
    bleTrpcSourceFile.setSourcePath('driver/ble/src/ble_src/profile_ble/ble_trspc/ble_trspc.c')
    bleTrpcSourceFile.setOutputName('ble_trspc.c')
    bleTrpcSourceFile.setOverwrite(True)
    bleTrpcSourceFile.setDestPath('ble/profile_ble/ble_trspc/')
    bleTrpcSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trspc/')
    bleTrpcSourceFile.setType('SOURCE')
    bleTrpcSourceFile.setEnabled(False)
    bleTrpcSourceFile.setMarkup(True)
    bleTrpcSourceFile.setDependencies(bleTrpcEnable, ["TRP_BOOL_CLIENT"])
    
    # Add ble_trspc.h file - static file
    bleTrpcHeaderFile = profileBLE_TRP_Component.createFileSymbol(None, None)
    bleTrpcHeaderFile.setSourcePath('driver/ble/src/ble_src/profile_ble/ble_trspc/ble_trspc.h')
    bleTrpcHeaderFile.setOutputName('ble_trspc.h')
    bleTrpcHeaderFile.setOverwrite(True)
    bleTrpcHeaderFile.setDestPath('ble/profile_ble/ble_trspc/')
    bleTrpcHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trspc/')
    bleTrpcHeaderFile.setType('HEADER')
    bleTrpcHeaderFile.setEnabled(False)
    bleTrpcHeaderFile.setMarkup(True)
    bleTrpcHeaderFile.setDependencies(bleTrpcEnable, ["TRP_BOOL_CLIENT"])
    
    # Add app_trspc_handler.c file - static file
    bleTrpcAppSourceFile = profileBLE_TRP_Component.createFileSymbol(None, None)
    bleTrpcAppSourceFile.setSourcePath('driver/ble/src/app_trspc_handler.c')
    bleTrpcAppSourceFile.setOutputName('app_trspc_handler.c')
    bleTrpcAppSourceFile.setOverwrite(True)
    bleTrpcAppSourceFile.setDestPath('../../app_ble')
    bleTrpcAppSourceFile.setProjectPath('app_ble')
    bleTrpcAppSourceFile.setType('Source')
    bleTrpcAppSourceFile.setEnabled(False)
    bleTrpcAppSourceFile.setDependencies(bleTrpcEnable, ["TRP_BOOL_CLIENT"])
    
    # Add app_trspc_handler.h file - static file
    bleTrpcAppHeaderFile = profileBLE_TRP_Component.createFileSymbol(None, None)
    bleTrpcAppHeaderFile.setSourcePath('driver/ble/src/app_trspc_handler.h')
    bleTrpcAppHeaderFile.setOutputName('app_trspc_handler.h')
    bleTrpcAppHeaderFile.setOverwrite(True)
    bleTrpcAppHeaderFile.setDestPath('../../app_ble')
    bleTrpcAppHeaderFile.setProjectPath('app_ble')
    bleTrpcAppHeaderFile.setType('HEADER')
    bleTrpcAppHeaderFile.setEnabled(False)
    bleTrpcAppHeaderFile.setDependencies(bleTrpcEnable, ["TRP_BOOL_CLIENT"])    


def finalizeComponent(BLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(BLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['svcBLE_TRS']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
