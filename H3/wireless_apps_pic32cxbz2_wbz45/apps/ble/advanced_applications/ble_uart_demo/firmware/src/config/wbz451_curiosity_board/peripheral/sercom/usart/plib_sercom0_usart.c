/*******************************************************************************
  SERCOM Universal Synchronous/Asynchrnous Receiver/Transmitter PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_sercom0_usart.c

  Summary
    USART peripheral library interface.

  Description
    This file defines the interface to the USART peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "interrupts.h"
#include "plib_sercom0_usart.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


/* SERCOM0 USART baud value for 921600 Hz baud rate */
#define SERCOM0_USART_INT_BAUD_VALUE            (45403UL)

static SERCOM_USART_RING_BUFFER_OBJECT sercom0USARTObj;

// *****************************************************************************
// *****************************************************************************
// Section: SERCOM0 USART Interface Routines
// *****************************************************************************
// *****************************************************************************

#define SERCOM0_USART_READ_BUFFER_SIZE      4096
#define SERCOM0_USART_READ_BUFFER_9BIT_SIZE     (4096 >> 1)
#define SERCOM0_USART_WRITE_BUFFER_SIZE      128
#define SERCOM0_USART_WRITE_BUFFER_9BIT_SIZE     (128 >> 1)
#define SERCOM0_USART_RX_INT_DISABLE()      SERCOM0_REGS->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENCLR_RXC_Msk
#define SERCOM0_USART_RX_INT_ENABLE()       SERCOM0_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC_Msk

static uint8_t SERCOM0_USART_ReadBuffer[SERCOM0_USART_READ_BUFFER_SIZE];


void SERCOM0_USART_Initialize( void )
{
    /*
     * Configures USART Clock Mode
     * Configures TXPO and RXPO
     * Configures Data Order
     * Configures Standby Mode
     * Configures Sampling rate
     * Configures IBON
     */
    SERCOM0_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_INT_CTRLA_RXPO(0x1UL) | SERCOM_USART_INT_CTRLA_TXPO(0x2UL) | SERCOM_USART_INT_CTRLA_DORD_Msk | SERCOM_USART_INT_CTRLA_IBON_Msk | SERCOM_USART_INT_CTRLA_FORM(0x0UL) | SERCOM_USART_INT_CTRLA_SAMPR(0UL) | SERCOM_USART_INT_CTRLA_RUNSTDBY_Msk;

    /* Configure Baud Rate */
    SERCOM0_REGS->USART_INT.SERCOM_BAUD = (uint16_t)SERCOM_USART_INT_BAUD_BAUD(SERCOM0_USART_INT_BAUD_VALUE);

    /*
     * Configures RXEN
     * Configures TXEN
     * Configures CHSIZE
     * Configures Parity
     * Configures Stop bits
     */
    SERCOM0_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT | SERCOM_USART_INT_CTRLB_RXEN_Msk ;

    /* Wait for sync */
    while((SERCOM0_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }


    /* Enable the UART after the configurations */
    SERCOM0_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

    /* Wait for sync */
    while((SERCOM0_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }

    /* Initialize instance object */
    sercom0USARTObj.rdCallback = NULL;
    sercom0USARTObj.rdInIndex = 0U;
    sercom0USARTObj.rdOutIndex = 0U;
    sercom0USARTObj.isRdNotificationEnabled = false;
    sercom0USARTObj.isRdNotifyPersistently = false;
    sercom0USARTObj.rdThreshold = 0U;
    sercom0USARTObj.errorStatus = USART_ERROR_NONE;
    if (((SERCOM0_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_CHSIZE_Msk) >> SERCOM_USART_INT_CTRLB_CHSIZE_Pos) != 0x01U)
    {
        sercom0USARTObj.rdBufferSize = SERCOM0_USART_READ_BUFFER_SIZE;
        sercom0USARTObj.wrBufferSize = SERCOM0_USART_WRITE_BUFFER_SIZE;
    }
    else
    {
        sercom0USARTObj.rdBufferSize = SERCOM0_USART_READ_BUFFER_9BIT_SIZE;
        sercom0USARTObj.wrBufferSize = SERCOM0_USART_WRITE_BUFFER_9BIT_SIZE;
    }
    /* Enable error interrupt */
    SERCOM0_REGS->USART_INT.SERCOM_INTENSET = (uint8_t)SERCOM_USART_INT_INTENSET_ERROR_Msk;

    /* Enable Receive Complete interrupt */
    SERCOM0_REGS->USART_INT.SERCOM_INTENSET = (uint8_t)SERCOM_USART_INT_INTENSET_RXC_Msk;
}

uint32_t SERCOM0_USART_FrequencyGet( void )
{
    return 48000000UL;
}

bool SERCOM0_USART_SerialSetup( USART_SERIAL_SETUP * serialSetup, uint32_t clkFrequency )
{
    bool setupStatus       = false;
    uint32_t baudValue     = 0U;
    uint32_t sampleRate    = 0U;

    if((serialSetup != NULL) && (serialSetup->baudRate != 0U))
    {
        if(clkFrequency == 0U)
        {
            clkFrequency = SERCOM0_USART_FrequencyGet();
        }

        if(clkFrequency >= (16U * serialSetup->baudRate))
        {
            baudValue = 65536U - (uint32_t)(((uint64_t)65536U * 16U * serialSetup->baudRate) / clkFrequency);
            sampleRate = 0U;
        }
        else if(clkFrequency >= (8U * serialSetup->baudRate))
        {
            baudValue = 65536U - (uint32_t)(((uint64_t)65536U * 8U * serialSetup->baudRate) / clkFrequency);
            sampleRate = 2U;
        }
        else if(clkFrequency >= (3U * serialSetup->baudRate))
        {
            baudValue = 65536U - (uint32_t)(((uint64_t)65536U * 3U * serialSetup->baudRate) / clkFrequency);
            sampleRate = 4U;
        }
        else
        {
            /* Do nothing */
        }

        if(baudValue != 0U)
        {
            /* Disable the USART before configurations */
            SERCOM0_REGS->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE_Msk;

            /* Wait for sync */
            while((SERCOM0_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U)
            {
                /* Do nothing */
            }

            /* Configure Baud Rate */
            SERCOM0_REGS->USART_INT.SERCOM_BAUD = (uint16_t)SERCOM_USART_INT_BAUD_BAUD(baudValue);

            /* Configure Parity Options */
            if(serialSetup->parity == USART_PARITY_NONE)
            {
                SERCOM0_REGS->USART_INT.SERCOM_CTRLA =  (SERCOM0_REGS->USART_INT.SERCOM_CTRLA & ~(SERCOM_USART_INT_CTRLA_SAMPR_Msk | SERCOM_USART_INT_CTRLA_FORM_Msk)) | SERCOM_USART_INT_CTRLA_FORM(0x0UL) | SERCOM_USART_INT_CTRLA_SAMPR((uint32_t)sampleRate); 
                SERCOM0_REGS->USART_INT.SERCOM_CTRLB = (SERCOM0_REGS->USART_INT.SERCOM_CTRLB & ~(SERCOM_USART_INT_CTRLB_CHSIZE_Msk | SERCOM_USART_INT_CTRLB_SBMODE_Msk)) | ((uint32_t) serialSetup->dataWidth | (uint32_t) serialSetup->stopBits);
            }
            else
            {
                SERCOM0_REGS->USART_INT.SERCOM_CTRLA =  (SERCOM0_REGS->USART_INT.SERCOM_CTRLA & ~(SERCOM_USART_INT_CTRLA_SAMPR_Msk | SERCOM_USART_INT_CTRLA_FORM_Msk)) | SERCOM_USART_INT_CTRLA_FORM(0x1UL) | SERCOM_USART_INT_CTRLA_SAMPR((uint32_t)sampleRate); 
                SERCOM0_REGS->USART_INT.SERCOM_CTRLB = (SERCOM0_REGS->USART_INT.SERCOM_CTRLB & ~(SERCOM_USART_INT_CTRLB_CHSIZE_Msk | SERCOM_USART_INT_CTRLB_SBMODE_Msk | SERCOM_USART_INT_CTRLB_PMODE_Msk)) | (uint32_t) serialSetup->dataWidth | (uint32_t) serialSetup->stopBits | (uint32_t) serialSetup->parity ;
            }

            /* Wait for sync */
            while((SERCOM0_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U)
            {
                /* Do nothing */
            }

            /* Enable the USART after the configurations */
            SERCOM0_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

            /* Wait for sync */
            while((SERCOM0_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U)
            {
                /* Do nothing */
            }


            if (((SERCOM0_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_CHSIZE_Msk) >> SERCOM_USART_INT_CTRLB_CHSIZE_Pos) != 0x01U)
            {
                sercom0USARTObj.rdBufferSize = SERCOM0_USART_READ_BUFFER_SIZE;
                sercom0USARTObj.wrBufferSize = SERCOM0_USART_WRITE_BUFFER_SIZE;
            }
            else
            {
                sercom0USARTObj.rdBufferSize = SERCOM0_USART_READ_BUFFER_9BIT_SIZE;
                sercom0USARTObj.wrBufferSize = SERCOM0_USART_WRITE_BUFFER_9BIT_SIZE;
            }

            setupStatus = true;
        }
    }

    return setupStatus;
}

void static SERCOM0_USART_ErrorClear( void )
{
    uint8_t  u8dummyData = 0;

    /* Clear error flag */
    SERCOM0_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_ERROR_Msk;

    /* Clear all errors */
    SERCOM0_REGS->USART_INT.SERCOM_STATUS = SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk ;

    /* Flush existing error bytes from the RX FIFO */
    while((SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) == SERCOM_USART_INT_INTFLAG_RXC_Msk)
    {
        u8dummyData = SERCOM0_REGS->USART_INT.SERCOM_DATA;
    }

    /* Ignore the warning */
    (void)u8dummyData;
}

USART_ERROR SERCOM0_USART_ErrorGet( void )
{
    USART_ERROR errorStatus = sercom0USARTObj.errorStatus;

    sercom0USARTObj.errorStatus = USART_ERROR_NONE;

    return errorStatus;
}


/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static inline bool SERCOM0_USART_RxPushByte(uint16_t rdByte)
{
    uint32_t tempInIndex;
    bool isSuccess = false;

    tempInIndex = sercom0USARTObj.rdInIndex + 1U;

    if (tempInIndex >= sercom0USARTObj.rdBufferSize)
    {
        tempInIndex = 0U;
    }

    if (tempInIndex == sercom0USARTObj.rdOutIndex)
    {
        /* Queue is full - Report it to the application. Application gets a chance to free up space by reading data out from the RX ring buffer */
        if(sercom0USARTObj.rdCallback != NULL)
        {
            sercom0USARTObj.rdCallback(SERCOM_USART_EVENT_READ_BUFFER_FULL, sercom0USARTObj.rdContext);

            /* Read the indices again in case application has freed up space in RX ring buffer */
            tempInIndex = sercom0USARTObj.rdInIndex + 1U;

            if (tempInIndex >= sercom0USARTObj.rdBufferSize)
            {
                tempInIndex = 0U;
            }
        }
    }

    /* Attempt to push the data into the ring buffer */
    if (tempInIndex != sercom0USARTObj.rdOutIndex)
    {
        if (((SERCOM0_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_CHSIZE_Msk) >> SERCOM_USART_INT_CTRLB_CHSIZE_Pos) != 0x01U)
        {
            /* 8-bit */
            SERCOM0_USART_ReadBuffer[sercom0USARTObj.rdInIndex] = (uint8_t)rdByte;
        }
        else
        {
            /* 9-bit */
            ((uint16_t*)&SERCOM0_USART_ReadBuffer)[sercom0USARTObj.rdInIndex] = rdByte;
        }

        sercom0USARTObj.rdInIndex = tempInIndex;
        isSuccess = true;
    }
    else
    {
        /* Queue is full. Data will be lost. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void SERCOM0_USART_ReadNotificationSend(void)
{
    uint32_t nUnreadBytesAvailable;

    if (sercom0USARTObj.isRdNotificationEnabled == true)
    {
        nUnreadBytesAvailable = SERCOM0_USART_ReadCountGet();

        if(sercom0USARTObj.rdCallback != NULL)
        {
            if (sercom0USARTObj.isRdNotifyPersistently == true)
            {
                if (nUnreadBytesAvailable >= sercom0USARTObj.rdThreshold)
                {
                    sercom0USARTObj.rdCallback(SERCOM_USART_EVENT_READ_THRESHOLD_REACHED, sercom0USARTObj.rdContext);
                }
            }
            else
            {
                if (nUnreadBytesAvailable == sercom0USARTObj.rdThreshold)
                {
                    sercom0USARTObj.rdCallback(SERCOM_USART_EVENT_READ_THRESHOLD_REACHED, sercom0USARTObj.rdContext);
                }
            }
        }
    }
}

size_t SERCOM0_USART_Read(uint8_t* pRdBuffer, const size_t size)
{
    size_t nBytesRead = 0U;
    uint32_t rdOutIndex;
    uint32_t rdInIndex;

    /* Take a snapshot of indices to avoid creation of critical section */

    rdOutIndex = sercom0USARTObj.rdOutIndex;
    rdInIndex = sercom0USARTObj.rdInIndex;

    while (nBytesRead < size)
    {
        if (rdOutIndex != rdInIndex)
        {
            if (((SERCOM0_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_CHSIZE_Msk) >> SERCOM_USART_INT_CTRLB_CHSIZE_Pos) != 0x01U)
            {
                pRdBuffer[nBytesRead++] = SERCOM0_USART_ReadBuffer[rdOutIndex++];
            }
            else
            {
                ((uint16_t*)pRdBuffer)[nBytesRead++] = ((uint16_t*)&SERCOM0_USART_ReadBuffer)[rdOutIndex++];
            }

            if (rdOutIndex >= sercom0USARTObj.rdBufferSize)
            {
                rdOutIndex = 0U;
            }
        }
        else
        {
            /* No more data available in the RX buffer */
            break;
        }
    }

    sercom0USARTObj.rdOutIndex = rdOutIndex;

    return nBytesRead;
}

size_t SERCOM0_USART_ReadCountGet(void)
{
    size_t nUnreadBytesAvailable;
    uint32_t rdOutIndex;
    uint32_t rdInIndex;

    /* Take a snapshot of indices to avoid creation of critical section */
    rdOutIndex = sercom0USARTObj.rdOutIndex;
    rdInIndex = sercom0USARTObj.rdInIndex;

    if ( rdInIndex >=  rdOutIndex)
    {
        nUnreadBytesAvailable =  rdInIndex - rdOutIndex;
    }
    else
    {
        nUnreadBytesAvailable =  (sercom0USARTObj.rdBufferSize -  rdOutIndex) + rdInIndex;
    }

    return nUnreadBytesAvailable;
}

size_t SERCOM0_USART_ReadFreeBufferCountGet(void)
{
    return (sercom0USARTObj.rdBufferSize - 1U) - SERCOM0_USART_ReadCountGet();
}

size_t SERCOM0_USART_ReadBufferSizeGet(void)
{
    return (sercom0USARTObj.rdBufferSize - 1U);
}

bool SERCOM0_USART_ReadNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = sercom0USARTObj.isRdNotificationEnabled;

    sercom0USARTObj.isRdNotificationEnabled = isEnabled;

    sercom0USARTObj.isRdNotifyPersistently = isPersistent;

    return previousStatus;
}

void SERCOM0_USART_ReadThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0U)
    {
        sercom0USARTObj.rdThreshold = nBytesThreshold;
    }
}

void SERCOM0_USART_ReadCallbackRegister( SERCOM_USART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    sercom0USARTObj.rdCallback = callback;

    sercom0USARTObj.rdContext = context;
}


void static SERCOM0_USART_ISR_ERR_Handler( void )
{
    USART_ERROR errorStatus = (USART_ERROR)(SERCOM0_REGS->USART_INT.SERCOM_STATUS & (SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk ));

    if(errorStatus != USART_ERROR_NONE)
    {
        /* Save the error to report later */
        sercom0USARTObj.errorStatus = errorStatus;

        /* Clear error flags and flush the error bytes */
        SERCOM0_USART_ErrorClear();

        if(sercom0USARTObj.rdCallback != NULL)
        {
            sercom0USARTObj.rdCallback(SERCOM_USART_EVENT_READ_ERROR, sercom0USARTObj.rdContext);
        }
    }
}

void static SERCOM0_USART_ISR_RX_Handler( void )
{


    while ((SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) == SERCOM_USART_INT_INTFLAG_RXC_Msk)
    {
        if (SERCOM0_USART_RxPushByte( (uint16_t)SERCOM0_REGS->USART_INT.SERCOM_DATA) == true)
        {
            SERCOM0_USART_ReadNotificationSend();
        }
        else
        {
            /* UART RX buffer is full */
        }
    }
}


void SERCOM0_USART_InterruptHandler( void )
{
    bool testCondition = false;
    if(SERCOM0_REGS->USART_INT.SERCOM_INTENSET != 0U)
    {
        /* Checks for error flag */
        testCondition = ((SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk) == SERCOM_USART_INT_INTFLAG_ERROR_Msk);
        testCondition = ((SERCOM0_REGS->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_ERROR_Msk) == SERCOM_USART_INT_INTENSET_ERROR_Msk) && testCondition;
        if(testCondition)
        {
            SERCOM0_USART_ISR_ERR_Handler();
        }


        /* Checks for receive complete empty flag */
        testCondition = (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk);
        testCondition = (SERCOM0_REGS->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_RXC_Msk) && testCondition;
        if(testCondition)
        {
            SERCOM0_USART_ISR_RX_Handler();
        }
    }
}
