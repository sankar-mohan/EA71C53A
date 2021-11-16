/*******************************************************************************
  ZLL Timer Header File

  Company:
    Microchip Technology Inc.

  File Name:
    N_Timer.h

  Summary:
    This file contains the ZLL Timer functions.

  Description:
    This file contains the ZLL Timer functions.
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


#ifndef N_TIMER_H
#define N_TIMER_H

/***************************************************************************************************
* INCLUDE FILES
***************************************************************************************************/

#include "zllplatform/infrastructure/N_Task/include/N_Task.h"
#include "zllplatform/infrastructure/N_Types/include/N_Types.h"

/***************************************************************************************************
* C++ DECLARATION WRAPPER
***************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
* EXPORTED TYPES
***************************************************************************************************/

typedef struct N_Timer_t N_Timer_t;

struct N_Timer_t
{
    N_Timer_t* pNext;
    uint32_t timestamp;
    N_Task_Id_t task;
    N_Task_Event_t evt;
};

/***************************************************************************************************
* EXPORTED FUNCTIONS
***************************************************************************************************/

/** Start a timer.
    \param timeoutMs The timer interval in milliseconds.
    \param pTimer The timer to start.

    It is possible to supply a small negative delay.
    The timer will expire immediately in this case.
*/
void N_Timer_Start32(int32_t timeoutMs, N_Timer_t* pTimer);

/** Cheaper to use version of \ref N_Timer_Start32 for small delays. */
void N_Timer_Start16(uint16_t timeoutMs, N_Timer_t* pTimer);

/* Reload a timer.
    \param intervalMs The timer interval in milliseconds.
    \param pTimer The timer to reload.

    This function can be used to create a reloading/repeating timer. It
    should only be called while handling the expiration of pTimer.

    Note that it does not start a timer that will keep on running.
    Instead this function must be called each time the timer expires.

    The next timeout will be interval milliseconds after the
    previous expiration time, so event handling latencies are ignored.
    If the next timeout has already expired, the event will fire immediately.
*/
void N_Timer_Reload32(int32_t intervalMs, N_Timer_t* pTimer);

/* Cheaper to use version of \ ref N_Timer_Reload32 for small intervals. */
void N_Timer_Reload16(uint16_t intervalMs, N_Timer_t* pTimer);

/** Stop a timer.
    \param pTimer The timer to stop.

    \note The event will not be triggered, even if the timer was already expired.
*/
void N_Timer_Stop(N_Timer_t* pTimer);

/** Check if the timer is running.
    \param pTimer The timer to check.
    \returns TRUE if the timer is running, FALSE otherwise

    Note that this function returns FALSE if the timer has expired, but
    the event has not been handled yet.
*/
bool N_Timer_IsRunning(N_Timer_t* pTimer);

/** Return the remaining time until the timer expires.
    \param pTimer The timer to check.
    \returns The remaining time in milliseconds. Even if the timer is not running.
*/
int32_t N_Timer_GetRemaining(N_Timer_t* pTimer);

/** Get the system time.
    \returns The system time (number of milliseconds since reset).
*/
uint32_t N_Timer_GetSystemTime(void);

/** Helper function to expire a timer (stop the running timer and set the event).
    \param task The timer task
    \param evt The timer event
    \returns TRUE if the timer was running, FALSE otherwise

    Only available if N_TIMER_ENABLE_EXPIRE is defined. It should only be
    used in unit tests.
*/
bool N_Timer_Expire(N_Task_Id_t task, N_Task_Event_t evt);

/***************************************************************************************************
* END OF C++ DECLARATION WRAPPER
***************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // N_TIMER_H
