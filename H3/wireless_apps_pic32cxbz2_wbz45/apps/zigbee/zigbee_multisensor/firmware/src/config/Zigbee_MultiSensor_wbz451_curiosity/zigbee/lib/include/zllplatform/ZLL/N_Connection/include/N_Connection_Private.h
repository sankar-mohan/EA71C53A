/*******************************************************************************
  ZLL Connection Private Header File

  Company:
    Microchip Technology Inc.

  File Name:
    N_Connection_Private.h

  Summary:
    This file contains the ZLL Connection Private functions.

  Description:
    This file contains the ZLL Connection Private functions.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END


#ifndef N_CONNECTION_PRIVATE_H
#define N_CONNECTION_PRIVATE_H

/*******************************************************************************
* INCLUDE FILES
*******************************************************************************/

#include "zllplatform/infrastructure/N_Types/include/N_Types.h"

/*******************************************************************************
* C++ DECLARATION WRAPPER
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* EXPORTED FUNCTIONS
*******************************************************************************/

/** Checks if LinkTarget set the InterPan mode to on

    \returns TRUE if LinkTarget set the InterPan mode to on; FALSE - otherwise
*/
bool N_Connection_TargetIsInterpanModeOn(void);

/** Component internal networkJoined notification
*/

/** Component internal networkJoined notification
*/

void N_Connection_RouterNetworkJoined(void);

/** Component internal networkJoined notification
*/
void N_Connection_NetworkJoined(void);

void N_Connection_EnddeviceNetworkJoined(void);

/** Component internal networkJoined notification
*/
void N_Connection_Connected(void);

/** Component internal router connected  notification
*/
void N_Connection_RouterConnected(void);

/** Component internal ed connected notification
*/
void N_Connection_EnddeviceConnected(void);

/** Component internal networkJoined notification
*/
void N_Connection_Disconnected(void);

/** Component internal disconnected notification
*/
void N_Connection_RouterDisconnected(void);

/** Component internal disconnected notification
*/
void N_Connection_EnddeviceDisconnected(void);

/** Leave network routine.
*/
void N_Connection_LeaveNetwork(void);

/** Returns the state of Reconnect urgent.
*/
bool N_Connection_Is_ReconnectUrgentOngoing(void);

/** Returns the state of Reconnect.
*/
bool N_Connection_Is_ReconnectOngoing(void);

/*******************************************************************************
* END OF C++ DECLARATION WRAPPER
*******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // N_CONNECTION_PRIVATE_H
