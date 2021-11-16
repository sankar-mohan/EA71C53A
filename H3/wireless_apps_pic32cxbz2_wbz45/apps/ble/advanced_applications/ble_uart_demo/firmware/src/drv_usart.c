#include <string.h>
#include "framework_defs.h"
#include "drv_usart.h"  
#include "peripheral/dmac/plib_dmac.h"
    
typedef enum
{
    DRV_UART_UNINITIAL = 0, 
    DRV_UART_IDLE,
    DRV_UART_BUSY,
}_DRV_UART_Status_T;

typedef struct _DRV_UART_AsyncDev_T  {
	sercom_registers_t 	*p_sercomReg;
	uint8_t     dmaChannel;
	
 	SERCOM_USART_TX_DONE_CALLBACK		usartWriteCb;
	_DRV_UART_Status_T busy;
	uint8_t     *wbuffer;
	uint16_t    wbufferSize;
	int16_t     remainTxBytes;
	uint16_t    wbufferWriteIdx;
	uint16_t    wbufferReadIdx;
	uint16_t 	sentTxBytes;
}_DRV_UART_AsyncDev_T;

_DRV_UART_AsyncDev_T g_uartDev[SERCOM_UART_END] = 
{
	{NULL, 0, NULL, DRV_UART_UNINITIAL, NULL, 0, 0, 0, 0},
	{NULL, 0, NULL, DRV_UART_UNINITIAL, NULL, 0, 0, 0, 0}
};

__attribute__(( weak ))	bool SERCOM1_USART_Read(void *buffer, const size_t size );
__attribute__(( weak ))	size_t SERCOM1_USART_ReadCountGet( void );
__attribute__(( weak ))	void SERCOM1_USART_Initialize();
__attribute__(( weak ))	void SERCOM1_USART_ReadThresholdSet(uint32_t nBytesThreshold);
__attribute__(( weak ))	bool SERCOM1_USART_ReadNotificationEnable(bool isEnabled, bool isPersistent);
__attribute__(( weak ))	void SERCOM1_USART_ReadCallbackRegister( SERCOM_USART_RING_BUFFER_CALLBACK callback, uintptr_t context);

#define DRV_USART_SERCOM0_RX_INT_ENABLE()       SERCOM0_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC_Msk

void UART_PPSInit(_DRV_UART_Index_T idx)
{
    //SCOM_HSEN[2:0]: SCOM (Direct) Enable, 17=SCOM0, 18=SCOM1 
    configASSERT(idx < SERCOM_UART_END);
    uint32_t direct_set[]       = {0x20000, 0x40000}; 
    //Reset values
    static uint32_t default_set = 0x1f00443b ;
    
    configASSERT(idx < SERCOM_UART_END);
    __disable_irq();

    default_set |= direct_set[idx];
	CFG_REGS->CFG_CFGCON1 = default_set ;
 
    __enable_irq();
}

_DRV_UART_Status_T DRV_UART_PutFifo(_DRV_UART_AsyncDev_T *p_dev)
{
    uint32_t wbufferSize = p_dev->wbufferSize ;
    
    if (p_dev->remainTxBytes) 
    {
        uint8_t  *data_ptr ;
        uint16_t remain;
        uint16_t curr_ubuffer_ridx = p_dev->wbufferReadIdx;   
        void *dst = (void *)&p_dev->p_sercomReg->USART_INT.SERCOM_DATA;
        
        if (p_dev->busy == DRV_UART_IDLE)
            p_dev->busy = DRV_UART_BUSY;

        if ((curr_ubuffer_ridx +  p_dev->remainTxBytes ) <= wbufferSize)  
            remain = p_dev->remainTxBytes;
        else
            remain = wbufferSize - curr_ubuffer_ridx;
        
        p_dev->sentTxBytes = remain;
        data_ptr = &p_dev->wbuffer[curr_ubuffer_ridx];

        DMAC_ChannelTransfer(p_dev->dmaChannel, data_ptr, dst, remain);
    }
    else
        p_dev->busy = DRV_UART_IDLE;

    return (p_dev->busy);
}

void DRV_Uart_DmaCb(DMAC_CHANNEL_CALLBACK event, uintptr_t context)
{
	_DRV_UART_AsyncDev_T *p_dev = (_DRV_UART_AsyncDev_T *)context;
	
	p_dev->wbufferReadIdx 	+= p_dev->sentTxBytes;
 	p_dev->wbufferReadIdx 	%= p_dev->wbufferSize;
  	p_dev->remainTxBytes 	-= p_dev->sentTxBytes;
	configASSERT(p_dev->remainTxBytes >= 0);  	
	if (DRV_UART_PutFifo(p_dev) == DRV_UART_IDLE)
  	{
  		if (p_dev->usartWriteCb)
  			p_dev->usartWriteCb();
  	}
}

uint32_t DRV_UART_PutBuf(_DRV_UART_Index_T usart_idx, uint16_t len, uint8_t *p_buf)
{
	configASSERT(usart_idx < SERCOM_UART_END);
	
	_DRV_UART_AsyncDev_T *p_dev = &g_uartDev[usart_idx]; 
    uint16_t ret = 0;
    uint32_t wbufferSize = p_dev->wbufferSize ;
    uint16_t remain;
    uint16_t idx;

    if (p_dev->busy == DRV_UART_UNINITIAL)
       return 1;

	configASSERT(wbufferSize >0);
    //check if space is enough 
    OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
    idx = p_dev->wbufferWriteIdx;
    if (p_dev->remainTxBytes +len <= wbufferSize)
    {
        remain = wbufferSize - idx;
        if (remain >= len)
            memcpy(&p_dev->wbuffer[idx], p_buf, len);
        else
        {
            memcpy(&p_dev->wbuffer[idx], p_buf, remain); 
            memcpy(p_dev->wbuffer, p_buf+ remain, len - remain);
        }
             
        p_dev->wbufferWriteIdx += len;
        p_dev->wbufferWriteIdx %= wbufferSize;
        
        OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);   
        p_dev->remainTxBytes +=len;  
        if (p_dev->busy == DRV_UART_IDLE)
            DRV_UART_PutFifo(p_dev);
        OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, 0);  
    }
    else 
        ret =1;

    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, 0);    
        
        
    return ret;
}

void DRV_UART_DmaInit()
{
	static bool uninitialized = true;
	
	if (uninitialized == true)
	{
		uninitialized = false;
		
		DMAC_Initialize();
		
		NVIC_SetPriority(DMAC_0_3_IRQn, 7);
 		NVIC_EnableIRQ(DMAC_0_3_IRQn);
	}
}

void DRV_UART_Init(_DRV_UART_Index_T idx, uint16_t wbufferSize, _DRV_UART_PlibRegister_T *p_plibSet)
{
	configASSERT(idx < SERCOM_UART_END);
	
	_DRV_UART_AsyncDev_T *p_dev = &g_uartDev[idx];
	uint8_t irq_n[] = {SERCOM0_IRQn, SERCOM1_IRQn};
	
	memset(p_dev, 0, sizeof(_DRV_UART_AsyncDev_T));
	if (wbufferSize)
	{
		p_dev->wbuffer     	= (uint8_t *)OSAL_Malloc(wbufferSize); 
    	p_dev->wbufferSize	= wbufferSize;
    	configASSERT(p_dev->wbuffer);
   	}
   	UART_PPSInit(idx);
    DRV_UART_DmaInit();
    
	if (idx == SERCOM_UART0)
	{
		SERCOM0_USART_Initialize();
		SERCOM0_REGS->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN_Msk;
		while(SERCOM0_REGS->USART_INT.SERCOM_SYNCBUSY);
		
		SERCOM0_USART_ReadThresholdSet(p_plibSet->usartReadThreadByte);
		SERCOM0_USART_ReadNotificationEnable(p_plibSet->usartReadNotifyEnable, p_plibSet->usartReadNotifyPersistent);
		SERCOM0_USART_ReadCallbackRegister(p_plibSet->usartReadCb, (uint32_t)p_dev);
		DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, (DMAC_CHANNEL_CALLBACK)DRV_Uart_DmaCb, (uint32_t)p_dev);	
		 
    	p_dev->p_sercomReg 	= SERCOM0_REGS;
    	p_dev->dmaChannel 		= DMAC_CHANNEL_0;
    	p_dev->usartWriteCb	= p_plibSet->usartWriteCb;
 		p_dev->busy 		= DRV_UART_IDLE;
	}
	else
	{
		SERCOM1_USART_Initialize();
		SERCOM1_REGS->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN_Msk;
		while(SERCOM1_REGS->USART_INT.SERCOM_SYNCBUSY);
		
		DMAC_ChannelCallbackRegister(DMAC_CHANNEL_1, (DMAC_CHANNEL_CALLBACK)DRV_Uart_DmaCb, (uint32_t)p_dev);
	
    	p_dev->p_sercomReg 	= SERCOM1_REGS;
    	p_dev->dmaChannel 		= DMAC_CHANNEL_1;
 		p_dev->busy 		= DRV_UART_IDLE;
	}
	NVIC_DisableIRQ((IRQn_Type)irq_n[idx]);
    NVIC_SetPriority((IRQn_Type)irq_n[idx],  3);  
    NVIC_ClearPendingIRQ((IRQn_Type)irq_n[idx]);
    NVIC_EnableIRQ((IRQn_Type)irq_n[idx]);
}
 
uint16_t DRV_UART_GetRxLength(_DRV_UART_Index_T idx) 
{
	configASSERT(idx < SERCOM_UART_END);
	
	if (idx == SERCOM_UART0)
		return SERCOM0_USART_ReadCountGet();
	else  
		return SERCOM1_USART_ReadCountGet();
}

uint16_t DRV_UART_ReadRxBuf(_DRV_UART_Index_T idx, uint8_t *p_buffer, uint16_t len)
{
	configASSERT(idx < SERCOM_UART_END);
	
	if (idx == SERCOM_UART0)
    {
        size_t ret;

        ret = SERCOM0_USART_Read(p_buffer, len);
        DRV_USART_SERCOM0_RX_INT_ENABLE();

        return ret;
    }
	else  
        return SERCOM1_USART_Read(p_buffer, len);
}

bool DRV_UART_Sercom1TxIsReady(void)
{
    _DRV_UART_AsyncDev_T *p_dev = &g_uartDev[SERCOM_UART1];

    if (p_dev->busy != DRV_UART_BUSY)
    {
        return true;
    }
    else
    {
        return false;
    }
}
