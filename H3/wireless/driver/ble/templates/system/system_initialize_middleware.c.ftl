    // Create BLE Stack Message QUEUE
    OSAL_QUEUE_Create(&bleRequestQueueHandle, QUEUE_LENGTH_BLE, QUEUE_ITEM_SIZE_BLE);

    // Retrieve BLE calibration data
    btSysCfg.addrValid = IB_GetBdAddr(&btSysCfg.devAddr[0]);
    btSysCfg.rssiOffsetValid =IB_GetRssiOffset(&btSysCfg.rssiOffset);
<#if USE_CUSTOM_ANT_GAIN>
    btSysCfg.antennaGainValid = true;
    btSysCfg.antennaGain = CUSTOM_ANT_GAIN;
<#else>
    btSysCfg.antennaGainValid = IB_GetAntennaGain(&btSysCfg.antennaGain);
</#if>

    // Initialize BLE Stack
    BT_SYS_Init(&bleRequestQueueHandle, &osalAPIList, NULL, &btSysCfg);

