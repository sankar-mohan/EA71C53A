---
parent: Driver libraries
title: BLE Driver Library
has_children: false
has_toc: false
nav_order: 1

family: PIC32CX-BZ
function: BLE
---

# BLE Driver Library

There are four blocks in the BLE Driver Library.
* __BLE Stack__

    BLE Stack plays an improtant role in BLE library, it is the core of BLE library.
    BLE Stack operates under it is own task, and it is released as Library object.
<br><br/>

* __BLE Middleware__

    BLE Middleware assists application to handle some BLE procedures. 
    BLE Middleware operates under application task, and it is released as source code.
    BLE Middleware is optional block, application could disable it if application would like to handle all the BLE Stack procedures.
<br><br/>

* __BLE Profile__

    There are some BLE profiles included in the BLE Driver Library package.
    BLE profile operates under application task, and it is released as source code.
    Application could enable or disable any profile depending on the requirement.
<br><br/>

* __BLE Service__

    There are some BLE services included in the BLE Driver Library package.
    BLE service operates under application task, and it is released as source code.
    Generally BLE service is used by dedicated BLE profile.
    Therefore if the BLE profile is enabled, BLE profile would initialize the relevant BLE services.
    However, BLE service could be used by application directly.
    In that case, application needs to initialzie BLE service.
<br><br/>
  
Refer to the below link for the detail BLE Driver Library usage:

* [Initializing the library](usage.md#initializing-ble-library)

* [Using the library](usage.md#using-the-library)

* [Library interface](html/modules.html)
