/**!
 * @file
 * 
 * @brief STM32L476-Discovery configuration routines to support ECE 486
 * 
 * @author Don Hummels
 * 
 * @date Jan 2016
 * 
 * Contains all of the subroutines to configure various peripherals of the stm32L476
 * 	 RCC, GPIO ports, DMAs, ADCs, DACs, etc.
 * 
 * @defgroup ECE486_Init ECE 486 Processor Initialization 
 * @{
 */

#ifndef __INIT486__
#define __INIT486__

/*
 * Includes..........................................................................
 */
#include <stdint.h>


/*
 * Defines..........................................................................
 */

/*
 * Compile Flags
 */
// #define TIM4_TO_PB6

/*! 
 *  @defgroup LED_L476 STM32L476-Discovery LED Assignments 
 * @{ 
 */
#define ERROR_LED LED4		//!< Red LED, STM32L476G-Discovery
#define NORMAL_LED LED5		//!< Green LED, STM32L476G-Discovery
/*! @} End of STM32L476-Discovery LED Assignments */



// ADC Input pins are also board-specific to avoid hardware conflicts
#if defined(STM32L476xx)	
/*!
 * 
 *  @defgroup ADC_Pin_Assignments_L476 STM32L476-Discovery ADC Assignments
 * @{
 * 
 * STM32L476-Discovery Pin Assignments:
 *    ADC1, Channel 6, tied to pin PA1.
 *    ADC2, Channel 7, tied to pin PA2.
 *    Digital Output: PD0
 */
#define ADC1_CHANNEL 		ADC_CHANNEL_6
#define ADC1_GPIO_PORT 		GPIOA
#define ADC1_GPIO_PIN  		GPIO_PIN_1
#define ADC1_GPIO_CLK_ENABLE() 	__HAL_RCC_GPIOA_CLK_ENABLE()

#define ADC2_CHANNEL 		ADC_CHANNEL_7
#define ADC2_GPIO_PORT 		GPIOA
#define ADC2_GPIO_PIN  		GPIO_PIN_2
#define ADC2_GPIO_CLK_ENABLE() 	__HAL_RCC_GPIOA_CLK_ENABLE()

#define DIGITAL_IO_PORT		GPIOD
#define DIGITAL_IO_PIN		GPIO_PIN_0
#define DIGITAL_IO_CLK_ENABLE()	__GPIOD_CLK_ENABLE()

/*! @} End of STM32L476-Discovery ADC Pin Assignments */

#else
#error "Unknown development board: Define STM32L486xx for STM32L476-Discovery"
#endif 

/*! @} End of Board_Specific_Config group */

// Valid Clock divisors to sample rate constants for initialize()
// (Integers used to divide an 80 MHz clock to obtain a desired sample rate)
/*! 
 * 
 * @defgroup FS_Count_Values Timer Count constants for determining ADC/DAC sample rates
 * @{
 * 
 * Timers are each driven by 80 MHz clock signals.  These integer constants are 
 * used to divide this clock rate down to audio sampling frequencies for the 
 * ADCs and DACs (Timer 4).  For example, to obtain a 50 ksps ADC/DAC clock, the 
 * timer period should be set to (80 MHz / 50 ksps)=1600 counts.
 */
#define FS_2K 40000 	//!< 2 ksamples/sec
#define FS_4K 20000 	//!< 4 ksamples/sec
#define FS_5K 16000 	//!< 5 ksamples/sec
#define FS_8K 10000 	//!< 8 ksamples/sec
#define FS_10K 8000 	//!< 10 ksamples/sec
#define FS_16K 5000 	//!< 16 ksamples/sec
#define FS_20K 4000 	//!< 20 ksamples/sec
#define FS_24K 3333 	//!< 24.0024 ksamples/sec
#define FS_25K 3200 	//!< 25 ksamples/sec
#define FS_32K 2500 	//!< 32 ksamples/sec
#define FS_40K 2000 	//!< 40 ksamples/sec
#define FS_48K 1667 	//!< 47.9904 ksamples/sec
#define FS_50K 1600 	//!< 50 ksamples/sec
#define FS_64K 1250 	//!< 64 ksamples/sec
#define FS_80K 1000 	//!< 80 ksamples/sec
#define FS_96K 833 	//!< 96.0384 ksamples/sec
#define FS_100K 800 	//!< 100 ksamples/sec
#define FS_125K 640 	//!< 125 ksamples/sec
#define FS_128K 625 	//!< 128 ksamples/sec
#define FS_160K 500 	//!< 160 ksamples/sec
#define FS_200K 400 	//!< 200 ksamples/sec
#define FS_250K 320 	//!< 250 ksamples/sec
#define FS_320K 250 	//!< 320 ksamples/sec
#define FS_400K 200 	//!< 400 ksamples/sec
#define FS_500K 160 	//!< 500 ksamples/sec
#define FS_625K 128 	//!< 625 ksamples/sec
#define FS_640K 125 	//!< 640 ksamples/sec
#define FS_800K 100 	//!< 800 ksamples/sec
#define FS_1000K 80 	//!< 1000 ksamples/sec

/*! @} End of FS_Count_Values group */

/*! 
 * 
 * @defgroup digital_io_macros Macros for the board-specific digital i/o line
 * @{
 * 
 * Each development board has one dedicated configured digital i/o pin.  
 * These macros control the state of that output pin.
 * (Pin PD0) for STM32L476G-Discovery)
 */
#define DIGITAL_IO_SET()    HAL_GPIO_WritePin(DIGITAL_IO_PORT, DIGITAL_IO_PIN, GPIO_PIN_SET)      
#define DIGITAL_IO_RESET()  HAL_GPIO_WritePin(DIGITAL_IO_PORT, DIGITAL_IO_PIN, GPIO_PIN_RESET)
#define DIGITAL_IO_TOGGLE() HAL_GPIO_TogglePin(DIGITAL_IO_PORT, DIGITAL_IO_PIN)                   
/*! @} End of digital_io_macros */

/*
 * structs, typedefs, enums .............................................................
 */

/*!
 * @brief Number of input audio channels
 */
enum Num_Channels_In {
  MONO_IN,		//!< Mono Input: Only configure ADC1, single DMA Transfer
  STEREO_IN		//!< Stereo Input: Configure ADC1 and ADC2, dual DMA Transfer
};


/*!
 * @brief Number of output audio channels
 */
enum Num_Channels_Out {
  MONO_OUT,		//!< Mono Output: Only configure DAC1, single DMA Transfer
  STEREO_OUT		//!< Stereo Output: Configure DAC1 and DAC2, dual DMA Transfer
};

/*!
 * @brief Clock Reference Source
 * 
 * By default, the STM32L476G-Discovery board uses the MSI internal RC Oscillator as
 * its clock source.  Specify MSI_INTERNAL_RC for an unmodified board (SB18 Opened,
 * SB21 and SB22 closed).
 * 
 * By modifying jumpers on the board, an external clock reference driving the 
 * high speed external (HSE) OSC_IN pin can be used.  The external clock can be 
 * provided by the ST-LINK MCU, or driven externally through the PH0 pin on the 
 * P2 header (pin 9).  Specify HSE_EXTERNAL_8MHz to use this improved clock 
 * source if your board has been modified.
 * 
 * - 8 MHz XTAL Ref from ST-LINK MCU: SB18 closed, SB22 opened, R89 not fitted.
 * 
 * - External 8MHz signal on PH0 on P2 header: SB18 opened, SB22 closed, R89 not fitted.
 * 
 */
enum Clock_Reference {
  MSI_INTERNAL_RC,	//!< Internal MSI RC Oscillator 
  HSE_EXTERNAL_8MHz	//!< External 8MHz reference 
};



/*
 * Global variables.....................................................................
 */
extern enum Num_Channels_Out Output_Configuration;
extern enum Num_Channels_In Input_Configuration;

/*
 * Function Prototypes.....................................................................
 */

/*!
 * @defgroup Functions Configuration Functions
 * @{
 */


/*!
 * @brief Wrapper function to perform all processor initialization for ECE 486
 * 
 * initialize() is provided for backward compatibility: It assumes that the 
 * internal (MSI) RC oscillator is to be used as the clock reference signal. To
 * specify use of an external crystal reference, use @code initialize_ece486() @endcode
 * 
 * This wrapper function is called by users at the start of ECE 486 routines to
 * perform initialization of I/O pins, ADCs, DACs, Timers, DMAs, and Interrupts.
 * 
 * Input parameters determine the ADC/DAC sample rate, and the number of input
 * and output analog channels.  Sample rates are determined by specifying a 
 * @a timer_count_value, which determines the ultimate sample frequency.  
 * Various \#define constants are provided to obtain common sample rates: 
 * @ref FS_Count_Values.
 * 
 * For example:
 * @code
 *   initialize( FS_50K, MONO_IN, STEREO_OUT );
 * @endcode
 * is used to configure the STM32-Discovery board for sampling at a 50 ksamples/second
 * input and output rate, with one analog input channel, and two analog output channels.
 * 
 * It is not possible to have different ADC and DAC sample rates using this routine.
 * 
 * A call to initialize() also pauses program execution until the "USER" button is pressed,
 * (Joystic "center" on the STM32L476G-Discovery board.)
 * This pause allows the board to be re-programmed without interference from DMA activity.
 * 
 */
void initialize(
  uint16_t timer_count_value, 	//!< [in] Number of timer counts required per analog sample processed.
  enum Num_Channels_In chanin,	//!< [in] Number of input channels: MONO_IN or STEREO_IN
  enum Num_Channels_Out chanout	//!< [in] Number of output channels: MONO_OUT or STEREO_OUT
);

/*!
 * @brief Wrapper function to perform all processor initialization for ECE 486
 * 
 * This wrapper function is called by users at the start of ECE 486 routines to
 * perform initialization of I/O pins, ADCs, DACs, Timers, DMAs, and Interrupts.
 * 
 * Input parameters determine the ADC/DAC sample rate, and the number of input
 * and output analog channels, and the source of the clock reference signal.  
 * Sample rates are determined by specifying a 
 * @a timer_count_value, which determines the ultimate sample frequency.  
 * Various \#define constants are provided to obtain common sample rates: 
 * @ref FS_Count_Values.
 * 
 * For example:
 * @code
 *   initialize_ece486( FS_50K, MONO_IN, STEREO_OUT, MSI_INTERNAL_RC );
 * @endcode
 * is used to configure the STM32-Discovery board for sampling at a 50 ksamples/second
 * input and output rate, with one analog input channel, and two analog output channels.
 * The internal RC MSI oscillator is used to provide the clock generation reference.
 * 
 * It is not possible to have different ADC and DAC sample rates using this routine.
 * 
 * A call to initialize_ece486() also pauses program execution until the "USER" button is pressed,
 * (Joystic "center" on the STM32L476G-Discovery board.)
 * This pause allows the board to be re-programmed without interference from DMA activity.
 * 
 */
void initialize_ece486(
  uint16_t timer_count_value, 	//!< [in] Number of timer counts required per analog sample processed.
  enum Num_Channels_In chanin,	//!< [in] Number of input channels: MONO_IN or STEREO_IN
  enum Num_Channels_Out chanout,	//!< [in] Number of output channels: MONO_OUT or STEREO_OUT
  enum Clock_Reference clkref   //!< [in] Clock ref source: MSI_INTERNAL_RC or HSE_EXTERNAL_8MHz
);

/*!
 * @brief  Simple function to return the best guess at the actual sampling frequency.
 * 
 * @returns Estimated sampling frequency in samples/second
 */ 
float getsamplingfrequency(void);


/*! @} End of Functions group */

#endif

/*!
 * @} End of ECE486_Init group
 */