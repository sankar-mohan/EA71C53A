def instantiateComponent(profileBLE_HOGPComponent):
    print('profileBLE_HOGPComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))

    # createBooleanSymbol(id, parent)
    boolClientRole = profileBLE_TRP_Component.createBooleanSymbol('BOOL_CLIENT', None)
    boolClientRole.setLabel('Enable Client Role')
    boolClientRole.setDefaultValue(True)

    # createBooleanSymbol(id, parent)
    boolServerRole = profileBLE_TRP_Component.createBooleanSymbol('BOOL_SERVER', None)
    boolServerRole.setLabel('Enable Server Role')
    boolServerRole.setDefaultValue(False)
    