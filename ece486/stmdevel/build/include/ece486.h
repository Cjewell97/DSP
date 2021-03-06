/*!
  @file
  @brief STM32L476G-Discovery ECE 486 Interface
  @author Don Hummels
  @date Jan, 2016

    @mainpage STM32L476G-Discovery support library for ECE 486 Digital Signal Processing

    This library defines an interface that allows a user use the 
    STM32L476G-Discovery development boards
    to stream input sampled data
    in real time from one or two ADCs, process the data, 
    and stream the resulting output samples to one or two output DACs.
	
    The user interface is as follows:
	
      -# Required include: @code #include "ece486.h" @endcode
      -# Optionally, the user calls @code
                    setblocksize(nsamp);
        @endcode
        This call is used to change the data block size that will be delivered
        to the user in later function calls. (If setblocksize() is not called,
        a default value of #DEFAULT_BLOCKSIZE is used.)  Using a larger block
        size may result in more efficient code, while using a smaller block 
        size will reduce the latency of the system.
      -# The user calls @code
		    initialize_ece486(fs, input_mode, output_mode, clk_ref);
	@endcode 
	where:
	  - @a fs determines the sampling frequency.  Predefined integer
                  constants for @a fs are provided at @ref FS_Count_Values. 
	  - @a input_mode should be #MONO_IN or #STEREO_IN
	  - @a output_mode should be #MONO_OUT or #STEREO_OUT
	  - @a clk_ref should be #MSI_INTERNAL_RC or #HSE_EXTERNAL_8MHz
	      
	The same sample rate is used for the input ADC 
	and output DAC data streams.  
	The initialize_ece486() call enables clocks and configures the 
	appropriate ADC(s), DAC(s), Timers, buttons, DMAs, UART, and 
	interrupts.  The function pauses at the beginning of the 
	initialization (with red and green LEDs flashing) to wait 
	for a user push-button (center joystick)
        indicating to continue (allowing the 
	device to be re-programmed without errors).  
	    
	For @a input_mode = #MONO_IN or #STEREO_IN, 
	Analog input waveforms are sampled using ADC1 (MONO) or 
	ADC1 | ADC2 (STEREO_IN).  For the STM32L476G-Discovery board,
	ADC1 is accessed as an input on 
	PA1, and ADC2 is accessed on PA2.  
	    
	Analog output is generated using DAC Channel 2 (MONO_OUT) or DAC Channels 1 and 2
	(STEREO_OUT).  DAC CHannel 2 is accessed as an output on PA5 (MONO_OUT or
	STEREO_OUT).  DAC Channel 1 actually drives an internal op-amp buffer (OPAMP1), with output
	to the discovery board connector on PA3 (STEREO_OUT only).
	   
	The green and red LEDs are configured and enabled.  The LEDs are used
	to provide feedback during program execution.  The red LED us used to 
	indicate an error condition.
      -# The user calls @code
		nsamp = getblocksize();
	@endcode 
	@a nsamp gives the data block size which will be delivered to
	the user in later function calls.  Input samples (provided by
	the ADC(s) via the DMA)
	will be accessed in blocks of @a nsamp samples at a time. 
	Likewise, the user programs should generate "nsamp" output 
	samples for delivery to the DAC.  Typically, users call
	getblocksize() to learn the required number of samples so 
	that the correct amount of memory for processing the blocks 
	of data may be allocated.
      -# Optionally, the user calls @code
		fs = getsamplingfrequency();
	@endcode
	This function returns the best guess (perfect accuracy of the MSI time reference) 
	at the actual sampling rate (in
	samples/second).  The actual sample rate may be slightly different from
	the requested rate or reported rate: The internal MSI oscilator is a factory
	calibrated RC time reference, and errors of a percent or so would not be
	unexpected.
      -# Users then repeatedly call getblock() or getblockstereo() to obtain
	samples from the ADC(s), and (after processing the samples) 
	putblock() or putblockstereo() to write the results back 
	to the DAC.  Using one of:
	@code
	  getblock(input1);  			// mono input
	  getblockstereo(input1, input2);	// stereo input
	@endcode
	will fill in the user's input array(s) (type float) with 
	normalized input (ADC) samples.  Callers are responsible to make sure 
	that the arrays @a input1 and @a input2 are allocated with enough 
	memory to hold @a nsamp float values.  After processing the 
	input waveforms to create results, the output samples are 
	placed in the DAC output buffers using one of
	@code
	  putblock(result1); 			// Mono Output 
	  putblockstereo(result1, result2);	// Stereo Output
	@endcode
	Typically, after calling the putblock() routine, the user 
	calls getblock() to wait for the next available block of 
	input data, and the process repeats.  If the DMA 
	completes filling a new block of data before the user calls 
	getblock(), a #SAMPLE_OVERRUN error is indicated, and 
	the RED LED is lit.  In this case, the data is not being 
	processed fast enough to keep up with the input data stream, 
	and input samples are being lost.
      
      Sample values in the "input" or "result" arrays are of 
      type float, and are normalized to range from -1.0 to +1.0 
      over the voltage range of the ADCs | DACs. (A sample of -1.0 is near 0V,
      +1.0 is near 3V, and 0.0 indicates the mid-scale voltage of 1.5V)

      By default, Pin PD0 on the STM32L476-Discovery board is configured
      as a digital output pin.  The pin status may be set using the macros
       @code
         DIGITAL_IO_SET();      // Set high
         DIGITAL_IO_RESET();    // Set low
         DIGITAL_IO_TOGGLE();   // Toggle state
       @endcode
      
      Other digital output can be configured using the standard HAL libraries
      The following
      configures PB6 (Port B, Pin 6) as a digital, push-pull output pin:
      @code
	GPIO_InitTypeDef  my_io_pin;
	
	__GPIOB_CLK_ENABLE();                    // Clock on for Port A
	my_io_pin.Mode = GPIO_MODE_OUTPUT_PP;    // Push/Pull digital output
	my_io_pin.Pull = GPIO_NOPULL;            // No pullup or pulldown resistor
	my_io_pin.Speed = GPIO_SPEED_HIGH;       // LOW, MEDIUM, FAST, or HIGH
	my_io_pin.Pin = GPIO_PIN_6;              // Set up PA1
	HAL_GPIO_Init(GPIOB, |my_io_pin);
      @endcode
      Once the pin is initialized, you can set the output state:
       @code
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);  
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);  
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
       @endcode
      
      The LEDs on the discovery boards are accessed using the BSP_LED_xxx()
      functions.  By default, the green LED (LED5) and red LED (LED4)
      are initialized.  While both LEDs are available to the user, the red LED
      is used by the library to visually indicate error conditions.  To 
      drive green LED:
        @code
          BSP_LED_On(LED5);		// Turn on the LED
          BSP_LED_Off(LED5);		// Turn off the LED
          BSP_LED_Toggle(LED5);		// Toggle the LED
	@endcode
            
      Users can check the global variable #KeyPressed to 
      check whether the (blue) user button on the Discovery board 
      has been pressed.   #KeyPressed==SET indicates that 
      a button press has occurred.  
      The user should reset the value using #KeyPressed=RESET;
      
      A UART is also configured to direct printf() output to the ST-LINK USB serial
      port.  Assuming that the host is connected to the ST-LINK connector, printf output
      may be examined on the host by connecting a terminal to the serial port.  The terminal
      should be configured at 115200 bps, 8 bits, no parity, one stop-bit.  printf() calls are 
      blocking.
      
      The discovery board LCD display may be accessed through the board-specific libraries 
      provided with the discovery board. For example:
        @code
          BSP_LCD_GLASS_DisplayString((uint8_t *) "HELLO");    // Short message
          
          //  Scroll a message 5 times...
          BSP_LCD_GLASS_ScrollSentence((uint8_t*) "LOTS TO SAY HERE", 5, SCROLL_SPEED_HIGH);
	@endcode

*/

/*!
  @page Peripherals Overview of STM32L476 Peripheral/Pin Configuration
    
  For the STM32L476-Discovery board, the following peripheral assignments are used:
 
|Pin    | Peripheral    | Use
|------ | ------------- | -------------------------------------------------
|PA5    | DAC Channel 2	| Primary (Mono or Stereo) output channel  
|       | DAC Channel 1	| Stereo output channel 
|       | DMA2 Channel 5| Used to move samples from a buffer to the DAC(s).
|PA3    | OPAMP1        | Used to buffer DAC Channel 1 to drive an output pin.
|	| TIMER 4	| Used to trigger ADC1 and DACs.
|PA3    | ADC1		| Primary (Mono or Stereo) input channel.
|PA2    | ADC2		| Stereo input channel.  
|	| DMA1 Channel 1| Used to move samples from the ADC(s) to a buffer.
|	| USART2	| Serial Communication to a host terminal

    - DMA2 Channel 5 is triggered by DAC2 using request 3
    - DMA1 Channel 1  is triggered by ADC1 using request 0
    - ADC2 is a slave to ADC1 in dual sampling mode (Stereo input case)
    
  DMA1 Channel 1 data is moved into an internal buffer of size #ADC_Buffer_Size.  
  Interrupts from DMA1 CHannel 1 are generated when the buffer is 
  half-full and full, indicating that the user may process one-half 
  of the samples, while the other half is being filled.  
  getblocksize() returns a value of #ADC_Block_Size, which is actually half 
  of the size of the actual DMA buffer (indicating the amount of data free to be processed in each sample block).
  
  ADC inputs and DAC output are accessed via the P1 connector on the 
  STM32L476G-Discovery board.  Pin assignments are as follows:
  
|        P1 Pin | P1 Label | ECE 486 Function  
|---------------|----------|----------------------------------------------
|        1	| 3V3	|  Power (3.3~V) 
|        2	| GND	|  Ground 
|        3	| 2V5	|  Power (2.5~V) 
|        4	| GND	|  Ground 
|        5	| 3V	|  Power (3~V) 
|        6	| BOOT0	|  -
|        7	| PB3	|  3V3_REG_ON: Tied to 3.3V Regulator via 4.7kohm
|        8	| PB2	|  Red LED
|        9	| PE8	|  Green LED
|        10	| PA0	|  GPIO Input: Connected to Joystic ``center" 
|        11	| PA5	|  Primary DAC Output(MONO and STEREO) 
|        12	| PA1	|  Primary ADC Input (MONO and STEREO)
|        13	| PA2	|  Second ADC Input (STEREO only)
|        14	| PA3	|  Second DAC Output via Opamp (STEREO only) 
|        15	| PB6	|  I2C1_SCL
|        16	| PB7	|  I2C1_SDA
|        17	| PD0	|  EXT_RST:  Configured as GPIO Digital Output 
|        18	| NC	|  -
|        19	| GND	|  Ground 
|        20	| GND	|  Ground
	
*/

#ifndef __ECE486__
#define __ECE486__

#include "init486.h"
#include "stm32l476g_discovery_glass_lcd.h"

#include "err486.h"
#include "sample486.h"

#endif