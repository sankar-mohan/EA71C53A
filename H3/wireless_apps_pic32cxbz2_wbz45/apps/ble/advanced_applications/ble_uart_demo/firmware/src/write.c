#include "drv_usart.h"


void _mon_putc (char c) 
{
#if 0
    DRV_UART_PutBuf(SERCOM_UART0, 1, (uint8_t *)&c);
#else
    DRV_UART_PutBuf(SERCOM_UART1, 1, (uint8_t *)&c);
#endif
}

void _mon_write (const char * s, unsigned int count)
{
#if 0
    DRV_UART_PutBuf(SERCOM_UART0, count, (uint8_t *)s);
#else
    DRV_UART_PutBuf(SERCOM_UART1, count, (uint8_t *)s);
#endif
}

int _mon_getc (int canblock)
{   
    return 0;
}
