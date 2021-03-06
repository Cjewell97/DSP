/*!
 * @file
 * 
 * @brief USART Initialization to stream printf() output back to the ST-LINK
 * 
 * @author Hummels... really stolen from Sheaff.  
 * 
 * @date Jan  2016
 * 
 * ST-LINK V2 provides a serial port to the host computer, which is accessed
 * through a serial connector configured here...
 */



#ifndef USART_H
#define USART_H

#include "stm32l4xx_hal.h"

/*!
 * @brief USART Configuration to talk to ST-LINK
 * 
 * USART2 is initialized to comminicate with the STM debug IC, via
 * our ouput pin PD5. (Standard settings - 115200 8N1)
 * 
 * @param none
 * @returns none
 */
void usart_init(void);

// USART device
extern UART_HandleTypeDef UsartHandle;

#endif 	// USART_H
