/*******************************************************************************
  Application Debug Log Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_log.h

  Summary:
    This file contains the definitions of debug log for application.

  Description:
    This file contains the definitions of debug log for application.
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


#ifndef APP_LOG_H
#define APP_LOG_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#ifndef APP_LOG_LEVEL
#define APP_LOG_LEVEL                          APP_LOG_LEVEL_ERROR
#endif


#define APP_LOG_INTERNAL(level, ...) {                                  \
    if (APP_LOG_ENABLE && (APP_LOG_LEVEL >= level))                     \
    {                                                                   \
        printf(__VA_ARGS__);                                            \
    }                                                                   \
}

#define APP_LOG_ERROR(...)                      APP_LOG_INTERNAL(APP_LOG_LEVEL_ERROR, __VA_ARGS__)
#define APP_LOG_INFO(...)                       APP_LOG_INTERNAL(APP_LOG_LEVEL_INFO, __VA_ARGS__)
#define APP_LOG_DEBUG(...)                      APP_LOG_INTERNAL(APP_LOG_LEVEL_DEBUG, __VA_ARGS__)


/**
 * @brief Logger severity levels.
 */
typedef enum APP_LOG_Level_T
{
    APP_LOG_LEVEL_NONE,
    APP_LOG_LEVEL_ERROR,
    APP_LOG_LEVEL_INFO,
    APP_LOG_LEVEL_DEBUG
} APP_LOG_Level_T;


#endif
