---
grand_parent: Wireless libraries
parent: BLE Library
title: BLE Library Usage
nav_order: 1
---

# BLE Library Usage

## Initializing the library

### Initializing BLE Stack

Before using any BLE Stack relevant API, BLE component must be initialized.
It is strongly recommend that BLE component be initialized during system initialization.
BLE component only operates with RTOS, the relevant parameters of RTOS must be assigned to BLE component.
Also, device address is necessary for BLE component.

```c
    //BLE Queue definition
    #define QUEUE_LENGTH_BLE (16)
    #define QUEUE_ITEM_SIZE_BLE (sizeof(void *))

    //BLE library Initialization Data
    OSAL_QUEUE_HANDLE_TYPE bleRequestQueueHandle;
    OSAL_API_LIST_TYPE     osalAPIList;
    uint8_t                bdAddr[7];     //address type + address[6]

    // Create BLE Stack Message QUEUE
    OSAL_QUEUE_Create(&bleRequestQueueHandle, QUEUE_LENGTH_BLE, QUEUE_ITEM_SIZE_BLE);

    // Initialize BLE Stack
    if(IB_GetBdAddr(bdAddr) == 0)
    {
        BT_SYS_Init(&bleRequestQueueHandle, &osalAPIList, NULL, bdAddr);
    }
    else
    {
        BT_SYS_Init(&bleRequestQueueHandle, &osalAPIList, NULL, NULL);
    }
```

BLE Stack provides various APIs for application, and those APIs belong to the specific module within dedicated group.
Currently there are four groups inside the BLE Stack.
Application shall initialize the dedicated group and modules in the BLE Stack if needs.

+ GAP: Provide the user interface for discovery of BLE devices, broadcast of BLE device, and connection management
+ L2CAP: Provide the user interface for credit based flow control procedure
+ GATT: Provide the user interface for GATT server and client procedure
+ SMP: Provide the user interface for pairing, authentication, and security configuration


Example of initializing modules in GAP group:
```c
    //Initialize BLE GAP main module
    BLE_GAP_Init();
    
    //Initialize BLE GAP advertising module
    BLE_GAP_AdvInit();

```

Example of initializing modules in L2CAP group:
```c
    //Initialize BLE L2cap main module
    BLE_L2CAP_Init();

    //Initialize BLE L2cap credit based flow control module
    BLE_L2CAP_CbInit();
```

Example of initializing modules in GATT group:
```c
    //Initialize BLE GATT server module
    GATTS_Init();

    //Initialize BLE GATT client module
    GATTC_Init();

```

Example of initializing modules in SMP group:
```c
    //Initialize BLE SMP main module
    BLE_SMP_Init();

```

BLE Stack would generate events to inform application if there is any status changed or activity.
Application may need to get the relevant information from BLE Stack and do the corresponding procedure.
Therefore, application shall register BLE Stack callback function after BLE Stack initialized.

Example of registering BLE Stack callback event:
```c
    /* Register BLE Stack callback event*/
    STACK_EventRegister(APP_BleStackCb);
```
### Initializing BLE Middleware

BLE component provides two modules in BLE Middleware to assist application.
Application shall initialize the dedicated modules if needs.
+ BLE_DM: Handle pairing procedures, connection management and the record of pairing information. Provide the simplified user interface for application.
+ BLE_DD: Handle the service discovery procedures. It shall be included if a GATT client profile is enabled.

Example of initializing modules in BLE Middleware:
```c
    //Initialize BLE device manager module
    BLE_DM_Init();

    //Initialize BLE device discovery module
    BLE_DD_Init();
```

BLE Middleware module would generate events to inform application if there is any status changed or activity.
Application may need to get the relevant information from BLE Middleware and do the corresponding procedure.
Therefore, application shall register BLE Middleware callback function after BLE Middleware initialized.

Example of registering BLE middleware callback event:
```c
    /* Register BLE DM module callback event */
    BLE_DM_EventRegister(APP_DmEvtHandler);

    /* Register BLE DD module callback event */
    BLE_DD_EventRegister(APP_DdEvtHandler);
```

### Initializing BLE Profile

BLE component provides some example profiles.
Application shall initialize the profiles if needs.

Example of initializing BLE profiles:
```c
    //Initialize BLE transparent server profile
    BLE_TRSPS_Init();

    //Initialize BLE transparent client profile
    BLE_TRSPC_Init();
```

BLE profiles module would generate events to inform application if there is any status changed or activity.
Application may need to get the relevant information from BLE profiles and do the corresponding procedure.
Therefore, application shall register BLE profiles callback function after BLE profiles initialized.

Example of registering BLE profiles callback event:
```c
    /* Register BLE transparent server callback event */
    BLE_TRSPS_EventRegistration(APP_TrspsEvtHandler);

    /* Register BLE transparent client callback event */
    BLE_TRSPC_EventRegistration(APP_TrspcEvtHandler);
```

### Initializing BLE Service

BLE component provides some example services.
Some of the services are used by the specific profile, and some of them may be used by application directly.


Example of initializing BLE service:
```c
    //Initialize BLE device information service
    BLE_DIS_Add();

    //Initialize BLE battery service
    BLE_BAS_Add();
```

## Using the library

Application shall be able to get the events from BLE Stack if the callback event is registered.
And it is strongly recommend application shall not handle the event directly.
Because timing is critical for BLE Stack, application shall not occupy the execution time before task switch.
Application shall generate a new message to handle the event within application task execution time.

Example of handling event from BLE Stack:
```c

    //Create a new message and handler the event later
    void APP_BleStackCb(STACK_Event_T *p_stack)
    {
        STACK_Event_T stackEvent;
        APP_Msg_T   appMsg;
        APP_Msg_T   *p_appMsg;
    
        memcpy((uint8_t *)&stackEvent, (uint8_t *)p_stack, sizeof(STACK_Event_T));
        stackEvent.p_event=OSAL_Malloc(p_stack->evtLen);
        if(stackEvent.p_event==NULL)
        {
            return;
        }
        memcpy(stackEvent.p_event, p_stack->p_event, p_stack->evtLen);
        stackEvent.p_event=stackEvent.p_event;
        ...
    
        appMsg.msgId=APP_MSG_BLE_STACK_EVT;
    
        ((STACK_Event_T *)appMsg.msgData)->groupId=p_stack->groupId;
        ((STACK_Event_T *)appMsg.msgData)->evtLen=p_stack->evtLen;
        ((STACK_Event_T *)appMsg.msgData)->p_event=stackEvent.p_event;
    
        p_appMsg = &appMsg;
        OSAL_QUEUE_Send(&appData.appQueue, p_appMsg, 0);
    }
    
    //Handle the BLE Stack events
    void APP_Tasks ( void )
    {
        APP_Msg_T    appMsg[1];
        APP_Msg_T   *p_appMsg;
        p_appMsg=appMsg;
    
        /* Check the application's current state. */
        switch ( appData.state )
        {
            ...
            case APP_STATE_SERVICE_TASKS:
            {
                if (OSAL_QUEUE_Receive(&appData.appQueue, &appMsg, OSAL_WAIT_FOREVER))
                {
                    if(p_appMsg->msgId==APP_MSG_BLE_STACK_EVT)
                    {
                        APP_BleStackEvtHandler((STACK_Event_T *)p_appMsg->msgData);
                    }
                }
                break;
            }
        }
    }
}
```

All the group events of BLE Stack comes in one entry point.
Hence, application shall check the groupId to identify the group first when receiving the BLE Stack event.
And then check the eventId in each group.

Example of handling the BLE Stack events:

```c
void APP_BleStackEvtHandler(STACK_Event_T *p_stackEvt)
{
    switch (p_stackEvt->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case STACK_GRP_BLE_L2CAP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case STACK_GRP_BLE_SMP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case STACK_GRP_GATT:
        {
            /* TODO: implement your application code.*/
        }
        break;

    } 
    OSAL_Free(p_stackEvt->p_event);
}
```
