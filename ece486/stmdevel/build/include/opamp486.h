/*!
 * @file
 * 
 * @brief Initialization of OPAMP1 to buffer DAC Ch 1 to an output pin
 * @author Don Hummels
 * @date Jan, 2016
 * 
 * The DAC Channel 1 output can be accessed through pin PA4---but PA4 is not 
 * extended to the ouput connector on the STM32L476G-Discovery board.  To get the 
 * signal to the output connector, DAC Channel 1 is internally configured to drive
 * OPAMP1. This opamp is used to drive PA3 (which is extended on the discovery board).
 * 
 */

#ifndef OPAMP_H
#define OPAMP_H

#include "stm32l4xx_hal.h"

/*!
 * @brief Set up OPAMP1 to buffer DAC Channel 1 to drive PA3
 * 
 * @param none
 * @returns none
 */
void opamp_init(void);

#endif 	// OPAMP_H
