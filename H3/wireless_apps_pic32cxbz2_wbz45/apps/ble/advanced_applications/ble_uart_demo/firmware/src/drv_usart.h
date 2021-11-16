#ifndef USART_H
#define USART_H

#include "peripheral/sercom/usart/plib_sercom_usart_common.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"
#include "peripheral/sercom/usart/plib_sercom1_usart.h"
#include "peripheral/dmac/plib_dmac.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*SERCOM_USART_TX_DONE_CALLBACK)();

typedef enum
{
    SERCOM_UART0 = 0,
    SERCOM_UART1,
    SERCOM_UART_END
}_DRV_UART_Index_T;

typedef struct DRV_UART_PlibRegister_T {
	uint32_t 							usartReadThreadByte;
	bool								usartReadNotifyEnable;
	bool								usartReadNotifyPersistent;
	SERCOM_USART_RING_BUFFER_CALLBACK	usartReadCb;
	SERCOM_USART_TX_DONE_CALLBACK		usartWriteCb;
}_DRV_UART_PlibRegister_T;

/*************************************************************************************************/
/*!
 *  \brief  DRV_UART_PutBuf  
 *          the interface for put buffer to uart
 *
 *  \param  idx   			uart device index.
 *          len             len of buffer
 *          p_buf      
 *
 *  \return 0 is ok otherwise fails..
 */
/*************************************************************************************************/
uint32_t DRV_UART_PutBuf(_DRV_UART_Index_T idx, uint16_t len, uint8_t *p_buf);


/*************************************************************************************************/
/*!
 *  \brief  DRV_UART_Init  
 *          initialization function for uart
 *
 *  \param  
 *          idx            uart index
 *          buffer_size    reserved local buffer for this uart   
 *          p_plibSet      pointer of the plib setting                       
 *                                   
 *  \return none
 */
/*************************************************************************************************/
void DRV_UART_Init(_DRV_UART_Index_T idx, uint16_t buffer_size, _DRV_UART_PlibRegister_T *p_plibSet);

 
/*************************************************************************************************/
/*!
 *  \brief  DRV_UART_GetRxLength  
 *          poll how many rx bytes queued
 *
 *  \param  idx   			uart device index.
 *   
 *
 *  \return how many queued Rx bytes
 */
/*************************************************************************************************/
uint16_t DRV_UART_GetRxLength(_DRV_UART_Index_T idx);


/*************************************************************************************************/
/*!
 *  \brief  DRV_UART_ReadRxBuf  
 *          poll how many rx bytes queued
 *
 *  \param  idx   			uart device index.
 *          p_buffer        destination for drvier copy rx data to
 *          len             len for copy to  (this equals return value of DRV_UART_GetRxLength)
 *
 *  \return how many Rx bytes to be put
 */
/*************************************************************************************************/
uint16_t DRV_UART_ReadRxBuf(_DRV_UART_Index_T idx, uint8_t *p_buffer, uint16_t len);


/*************************************************************************************************/
/*!
 *  \brief  Check if SERCOM1 TX is proceeding
 *
 *
 *  \return true            TX is completed.
 *          false           TX is proceeding.
 */
/*************************************************************************************************/
bool DRV_UART_Sercom1TxIsReady(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* USART_H_INCLUDED */
