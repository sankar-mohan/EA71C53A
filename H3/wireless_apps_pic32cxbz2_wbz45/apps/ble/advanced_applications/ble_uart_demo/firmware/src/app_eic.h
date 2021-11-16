/*******************************************************************************
  Application External Interrupt Control(EIC) Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_eic.h

  Summary:
    This file contains the Application External Interrupt Control(EIC) functions for this project.

  Description:
    This file contains the Application External Interrupt Control(EIC) functions for this project.
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


#ifndef APP_EIC_H
#define APP_EIC_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define APP_EIC_PIN_RX_IND                              0x10         /**< PB4 is used for the RX_IND/Wake up pin. */


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/**@brief The function is used to register the EIC callback event.
 *
 * @retval none
 *
 */
void APP_EIC_Init(void);


/**@brief The function is used to get the wake up pin status.
 *
 * @retval 1                        Pin status is high.
 * @retval 0                        Pin status is low.
 *
 */
uint8_t APP_EIC_GetWakeUpPinStatus(void);


/**@brief The function is used to stop the UART RX polling timer.
 *
 * @retval APP_RES_SUCCESS                    Stop a timer successfully.
 * @retval APP_RES_FAIL                       Failed to stop the timer.
 * @retval APP_RES_INVALID_PARA               The timerId doesn't exist.
 *
 */
uint16_t APP_EIC_StopUartRxTimer(void);


#endif
