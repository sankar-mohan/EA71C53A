def instantiateComponent(svcBLE_ANSComponent):
    print('svcBLE_ANSComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))
