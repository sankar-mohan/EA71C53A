def instantiateComponent(svcBLE_HIDSComponent):
    print('svcBLE_HIDSComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))
