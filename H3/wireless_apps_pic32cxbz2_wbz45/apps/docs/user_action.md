---
grand_parent: Examples applications
parent: User Docs
title: User Action
has_children: true
has_toc: false
nav_order: 511

family: PIC32CX-BZ
market:
  - wireless
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
#  **User Action** 

Developers are required to call API - app_idle_task() in vApplicationIdleHook(). This is required to enable periodic Persistent Data Storage, RF calibration. Persistant Data Storage provides means for storing data in non-volatile memory(Flash). User application and stack parameters like peer connection etc are saved. IDLE task is set to execute at the lowest priority in FreeRtos. 

1. Users will be prompted to call app_idle_task() and app_idle_updateRtcCnt(RTC_Timer32CounterGet()) when they compile/build the project with a compile error. 
	![](media/lowpower_user_action1.png)

	action required to be taken

	![](media/lowpower_user_action8.png)


2. users are required to open freertos_hooks.c file and add **#include "definitions.h"**

	![](media/lowpower_user_action3.png)

3. users are required to open freertos_hooks.c file and add API **app_idle_task()**

	![](media/user_action4.png)


4. users are required to comment out **#error** in app_user_edits.c file 

	![](media/lowpower_user_action5.png)

5. Initiate build and user will be able to build successfully  

	![](media/user_action6.png)

**Note:** User action is only required for the first time code gets generated. 

# **Additional User Action when Low Power is enabled**

Developers are required to call app_idle_updateRtcCnt(RTC_Timer32CounterGet()) in vApplicationTickHook(). This API call is used to compensate for FreeRTOSTimer when in sleep mode.
![](media/lowpower_user_action2.png)

1. users are required to open freertos_hooks.c file and add API **app_idle_updateRtcCnt(RTC_Timer32CounterGet())**

	![](media/lowpower_user_action4.png)

