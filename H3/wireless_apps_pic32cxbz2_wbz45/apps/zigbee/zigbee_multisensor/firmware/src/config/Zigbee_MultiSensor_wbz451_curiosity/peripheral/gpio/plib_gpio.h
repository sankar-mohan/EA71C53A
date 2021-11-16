/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************

/*** Macros for GPIO_RB0 pin ***/
#define GPIO_RB0_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<0))
#define GPIO_RB0_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<0))
#define GPIO_RB0_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<0))
#define GPIO_RB0_Get()               ((GPIOB_REGS->GPIO_PORT >> 0) & 0x1)
#define GPIO_RB0_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<0))
#define GPIO_RB0_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<0))
#define GPIO_RB0_PIN                  GPIO_PIN_RB0
/*** Macros for GPIO_RB3 pin ***/
#define GPIO_RB3_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<3))
#define GPIO_RB3_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<3))
#define GPIO_RB3_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<3))
#define GPIO_RB3_Get()               ((GPIOB_REGS->GPIO_PORT >> 3) & 0x1)
#define GPIO_RB3_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<3))
#define GPIO_RB3_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<3))
#define GPIO_RB3_PIN                  GPIO_PIN_RB3
/*** Macros for GPIO_RB5 pin ***/
#define GPIO_RB5_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<5))
#define GPIO_RB5_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<5))
#define GPIO_RB5_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<5))
#define GPIO_RB5_Get()               ((GPIOB_REGS->GPIO_PORT >> 5) & 0x1)
#define GPIO_RB5_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<5))
#define GPIO_RB5_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<5))
#define GPIO_RB5_PIN                  GPIO_PIN_RB5
/*** Macros for GPIO_RA11 pin ***/
#define GPIO_RA11_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<11))
#define GPIO_RA11_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<11))
#define GPIO_RA11_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<11))
#define GPIO_RA11_Get()               ((GPIOA_REGS->GPIO_PORT >> 11) & 0x1)
#define GPIO_RA11_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<11))
#define GPIO_RA11_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<11))
#define GPIO_RA11_PIN                  GPIO_PIN_RA11
/*** Macros for GPIO_RA12 pin ***/
#define GPIO_RA12_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<12))
#define GPIO_RA12_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<12))
#define GPIO_RA12_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<12))
#define GPIO_RA12_Get()               ((GPIOA_REGS->GPIO_PORT >> 12) & 0x1)
#define GPIO_RA12_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<12))
#define GPIO_RA12_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<12))
#define GPIO_RA12_PIN                  GPIO_PIN_RA12
/*** Macros for GPIO_RB6 pin ***/
#define GPIO_RB6_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<6))
#define GPIO_RB6_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<6))
#define GPIO_RB6_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<6))
#define GPIO_RB6_Get()               ((GPIOB_REGS->GPIO_PORT >> 6) & 0x1)
#define GPIO_RB6_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<6))
#define GPIO_RB6_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<6))
#define GPIO_RB6_PIN                  GPIO_PIN_RB6
/*** Macros for GPIO_RB7 pin ***/
#define GPIO_RB7_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<7))
#define GPIO_RB7_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<7))
#define GPIO_RB7_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<7))
#define GPIO_RB7_Get()               ((GPIOB_REGS->GPIO_PORT >> 7) & 0x1)
#define GPIO_RB7_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<7))
#define GPIO_RB7_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<7))
#define GPIO_RB7_PIN                  GPIO_PIN_RB7
/*** Macros for GPIO_RB8 pin ***/
#define GPIO_RB8_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<8))
#define GPIO_RB8_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<8))
#define GPIO_RB8_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<8))
#define GPIO_RB8_Get()               ((GPIOB_REGS->GPIO_PORT >> 8) & 0x1)
#define GPIO_RB8_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<8))
#define GPIO_RB8_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<8))
#define GPIO_RB8_PIN                  GPIO_PIN_RB8
/*** Macros for GPIO_RB9 pin ***/
#define GPIO_RB9_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<9))
#define GPIO_RB9_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<9))
#define GPIO_RB9_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<9))
#define GPIO_RB9_Get()               ((GPIOB_REGS->GPIO_PORT >> 9) & 0x1)
#define GPIO_RB9_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<9))
#define GPIO_RB9_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<9))
#define GPIO_RB9_PIN                  GPIO_PIN_RB9
/*** Macros for GPIO_RA4 pin ***/
#define GPIO_RA4_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<4))
#define GPIO_RA4_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<4))
#define GPIO_RA4_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<4))
#define GPIO_RA4_Get()               ((GPIOA_REGS->GPIO_PORT >> 4) & 0x1)
#define GPIO_RA4_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<4))
#define GPIO_RA4_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<4))
#define GPIO_RA4_PIN                  GPIO_PIN_RA4
/*** Macros for GPIO_RB10 pin ***/
#define GPIO_RB10_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<10))
#define GPIO_RB10_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<10))
#define GPIO_RB10_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<10))
#define GPIO_RB10_Get()               ((GPIOB_REGS->GPIO_PORT >> 10) & 0x1)
#define GPIO_RB10_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<10))
#define GPIO_RB10_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<10))
#define GPIO_RB10_PIN                  GPIO_PIN_RB10
/*** Macros for GPIO_RB11 pin ***/
#define GPIO_RB11_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<11))
#define GPIO_RB11_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<11))
#define GPIO_RB11_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<11))
#define GPIO_RB11_Get()               ((GPIOB_REGS->GPIO_PORT >> 11) & 0x1)
#define GPIO_RB11_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<11))
#define GPIO_RB11_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<11))
#define GPIO_RB11_PIN                  GPIO_PIN_RB11
/*** Macros for GPIO_RA0 pin ***/
#define GPIO_RA0_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<0))
#define GPIO_RA0_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<0))
#define GPIO_RA0_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<0))
#define GPIO_RA0_Get()               ((GPIOA_REGS->GPIO_PORT >> 0) & 0x1)
#define GPIO_RA0_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<0))
#define GPIO_RA0_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<0))
#define GPIO_RA0_PIN                  GPIO_PIN_RA0
/*** Macros for GPIO_RA1 pin ***/
#define GPIO_RA1_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<1))
#define GPIO_RA1_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<1))
#define GPIO_RA1_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<1))
#define GPIO_RA1_Get()               ((GPIOA_REGS->GPIO_PORT >> 1) & 0x1)
#define GPIO_RA1_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<1))
#define GPIO_RA1_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<1))
#define GPIO_RA1_PIN                  GPIO_PIN_RA1
/*** Macros for GPIO_RB12 pin ***/
#define GPIO_RB12_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<12))
#define GPIO_RB12_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<12))
#define GPIO_RB12_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<12))
#define GPIO_RB12_Get()               ((GPIOB_REGS->GPIO_PORT >> 12) & 0x1)
#define GPIO_RB12_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<12))
#define GPIO_RB12_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<12))
#define GPIO_RB12_PIN                  GPIO_PIN_RB12
/*** Macros for GPIO_RB13 pin ***/
#define GPIO_RB13_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<13))
#define GPIO_RB13_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<13))
#define GPIO_RB13_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<13))
#define GPIO_RB13_Get()               ((GPIOB_REGS->GPIO_PORT >> 13) & 0x1)
#define GPIO_RB13_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<13))
#define GPIO_RB13_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<13))
#define GPIO_RB13_PIN                  GPIO_PIN_RB13
/*** Macros for GPIO_RA5 pin ***/
#define GPIO_RA5_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<5))
#define GPIO_RA5_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<5))
#define GPIO_RA5_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<5))
#define GPIO_RA5_Get()               ((GPIOA_REGS->GPIO_PORT >> 5) & 0x1)
#define GPIO_RA5_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<5))
#define GPIO_RA5_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<5))
#define GPIO_RA5_PIN                  GPIO_PIN_RA5
/*** Macros for GPIO_RA6 pin ***/
#define GPIO_RA6_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<6))
#define GPIO_RA6_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<6))
#define GPIO_RA6_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<6))
#define GPIO_RA6_Get()               ((GPIOA_REGS->GPIO_PORT >> 6) & 0x1)
#define GPIO_RA6_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<6))
#define GPIO_RA6_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<6))
#define GPIO_RA6_PIN                  GPIO_PIN_RA6
/*** Macros for GPIO_RA8 pin ***/
#define GPIO_RA8_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<8))
#define GPIO_RA8_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<8))
#define GPIO_RA8_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<8))
#define GPIO_RA8_Get()               ((GPIOA_REGS->GPIO_PORT >> 8) & 0x1)
#define GPIO_RA8_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<8))
#define GPIO_RA8_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<8))
#define GPIO_RA8_PIN                  GPIO_PIN_RA8
/*** Macros for GPIO_RA9 pin ***/
#define GPIO_RA9_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<9))
#define GPIO_RA9_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<9))
#define GPIO_RA9_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<9))
#define GPIO_RA9_Get()               ((GPIOA_REGS->GPIO_PORT >> 9) & 0x1)
#define GPIO_RA9_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<9))
#define GPIO_RA9_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<9))
#define GPIO_RA9_PIN                  GPIO_PIN_RA9
/*** Macros for GPIO_RA7 pin ***/
#define GPIO_RA7_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<7))
#define GPIO_RA7_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<7))
#define GPIO_RA7_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<7))
#define GPIO_RA7_Get()               ((GPIOA_REGS->GPIO_PORT >> 7) & 0x1)
#define GPIO_RA7_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<7))
#define GPIO_RA7_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<7))
#define GPIO_RA7_PIN                  GPIO_PIN_RA7
/*** Macros for GPIO_RA10 pin ***/
#define GPIO_RA10_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<10))
#define GPIO_RA10_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<10))
#define GPIO_RA10_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<10))
#define GPIO_RA10_Get()               ((GPIOA_REGS->GPIO_PORT >> 10) & 0x1)
#define GPIO_RA10_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<10))
#define GPIO_RA10_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<10))
#define GPIO_RA10_PIN                  GPIO_PIN_RA10
/*** Macros for GPIO_RA13 pin ***/
#define GPIO_RA13_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<13))
#define GPIO_RA13_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<13))
#define GPIO_RA13_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<13))
#define GPIO_RA13_Get()               ((GPIOA_REGS->GPIO_PORT >> 13) & 0x1)
#define GPIO_RA13_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<13))
#define GPIO_RA13_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<13))
#define GPIO_RA13_PIN                  GPIO_PIN_RA13
/*** Macros for GPIO_RA14 pin ***/
#define GPIO_RA14_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<14))
#define GPIO_RA14_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<14))
#define GPIO_RA14_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<14))
#define GPIO_RA14_Get()               ((GPIOA_REGS->GPIO_PORT >> 14) & 0x1)
#define GPIO_RA14_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<14))
#define GPIO_RA14_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<14))
#define GPIO_RA14_PIN                  GPIO_PIN_RA14
/*** Macros for GPIO_RA2 pin ***/
#define GPIO_RA2_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<2))
#define GPIO_RA2_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<2))
#define GPIO_RA2_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<2))
#define GPIO_RA2_Get()               ((GPIOA_REGS->GPIO_PORT >> 2) & 0x1)
#define GPIO_RA2_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<2))
#define GPIO_RA2_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<2))
#define GPIO_RA2_PIN                  GPIO_PIN_RA2
/*** Macros for GPIO_RA3 pin ***/
#define GPIO_RA3_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<3))
#define GPIO_RA3_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<3))
#define GPIO_RA3_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<3))
#define GPIO_RA3_Get()               ((GPIOA_REGS->GPIO_PORT >> 3) & 0x1)
#define GPIO_RA3_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<3))
#define GPIO_RA3_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<3))
#define GPIO_RA3_PIN                  GPIO_PIN_RA3
/*** Macros for GPIO_RB2 pin ***/
#define GPIO_RB2_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<2))
#define GPIO_RB2_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<2))
#define GPIO_RB2_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<2))
#define GPIO_RB2_Get()               ((GPIOB_REGS->GPIO_PORT >> 2) & 0x1)
#define GPIO_RB2_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<2))
#define GPIO_RB2_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<2))
#define GPIO_RB2_PIN                  GPIO_PIN_RB2
/*** Macros for GPIO_RB1 pin ***/
#define GPIO_RB1_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<1))
#define GPIO_RB1_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<1))
#define GPIO_RB1_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<1))
#define GPIO_RB1_Get()               ((GPIOB_REGS->GPIO_PORT >> 1) & 0x1)
#define GPIO_RB1_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<1))
#define GPIO_RB1_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<1))
#define GPIO_RB1_PIN                  GPIO_PIN_RB1
/*** Macros for GPIO_RB4 pin ***/
#define GPIO_RB4_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<4))
#define GPIO_RB4_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<4))
#define GPIO_RB4_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<4))
#define GPIO_RB4_Get()               ((GPIOB_REGS->GPIO_PORT >> 4) & 0x1)
#define GPIO_RB4_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<4))
#define GPIO_RB4_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<4))
#define GPIO_RB4_PIN                  GPIO_PIN_RB4


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_A = GPIOA_BASE_ADDRESS,
    GPIO_PORT_B = GPIOB_BASE_ADDRESS,
} GPIO_PORT;

typedef enum
{
    GPIO_INTERRUPT_ON_MISMATCH,
    GPIO_INTERRUPT_ON_RISING_EDGE,
    GPIO_INTERRUPT_ON_FALLING_EDGE,
    GPIO_INTERRUPT_ON_BOTH_EDGES,
}GPIO_INTERRUPT_STYLE;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RA0 = 0,
    GPIO_PIN_RA1 = 1,
    GPIO_PIN_RA2 = 2,
    GPIO_PIN_RA3 = 3,
    GPIO_PIN_RA4 = 4,
    GPIO_PIN_RA5 = 5,
    GPIO_PIN_RA6 = 6,
    GPIO_PIN_RA7 = 7,
    GPIO_PIN_RA8 = 8,
    GPIO_PIN_RA9 = 9,
    GPIO_PIN_RA10 = 10,
    GPIO_PIN_RA11 = 11,
    GPIO_PIN_RA12 = 12,
    GPIO_PIN_RA13 = 13,
    GPIO_PIN_RA14 = 14,
    GPIO_PIN_RB0 = 16,
    GPIO_PIN_RB1 = 17,
    GPIO_PIN_RB2 = 18,
    GPIO_PIN_RB3 = 19,
    GPIO_PIN_RB4 = 20,
    GPIO_PIN_RB5 = 21,
    GPIO_PIN_RB6 = 22,
    GPIO_PIN_RB7 = 23,
    GPIO_PIN_RB8 = 24,
    GPIO_PIN_RB9 = 25,
    GPIO_PIN_RB10 = 26,
    GPIO_PIN_RB11 = 27,
    GPIO_PIN_RB12 = 28,
    GPIO_PIN_RB13 = 29,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(GPIOA_BASE_ADDRESS + (0x100 * (pin>>4))), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(GPIOA_BASE_ADDRESS + (0x100 * (pin>>4))))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(GPIOA_BASE_ADDRESS + (0x100 * (pin>>4)))) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(GPIOA_BASE_ADDRESS + (0x100 * (pin>>4))), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(GPIOA_BASE_ADDRESS + (0x100 * (pin>>4))), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(GPIOA_BASE_ADDRESS + (0x100 * (pin>>4))), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(GPIOA_BASE_ADDRESS + (0x100 * (pin>>4))), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(GPIOA_BASE_ADDRESS + (0x100 * (pin>>4))), 0x1 << (pin & 0xF));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
