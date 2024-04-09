/*
 * This class handles WS281x led´s based on a stm32 processor with using PWM and DMA.
 * Based on the work of https://controllerstech.com/interface-ws2812-with-stm32/
 *
 * It is important to set the clock configuration in the .ioc, otherwise
 * it will not work.
 * If you are using an other clock speed, you have to calculate the new ARR for the timer which is used
 * for the PWM data signal and modify the defined constants below.
 *
 * Set the following settings in the .ioc:
 *
 * System clock: 72 MHz
 *
 * Timer
 * TIM Prescaler: 0
 * TIM ARR: 90-1     // ARR = Sys_clk / 800000
 *
 * DMA
 * Direction: Memory to Peripheral
 * Mode: Normal
 * Priority: Medium
 * Peripheral Increment Address: unchecked
 * Peripheral Data Width: Half Word
 * Memory Increment Address: checked
 * Memory Data Width: Byte
 */


#ifndef __ws281x
#define __ws281x

// Standard library includes
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stddef.h>
#include <math.h>
#include <stdarg.h>

// STM32 processor includes
#include <stm32f1xx_hal.h>	// used uC hal
#include"../Core/Inc/main.h" // main header file

// ws281x specific functions
#include "example.h"
#include "utility.h"
#include "color.h"


// Your own project specific includes
#include "../config.h"


// --------------- To set by user ---------------------------

#define USE_WS2811 1	// WS2811: 1 , WS2812: 0 // WS2812 untested!

#define NUM_LED 6				// Total number of leds, doesn't matter if WS2812 or WS2811
#define ENABLE_BRIGHTNESS 1		// 1: enables brightness control of the leds, if deactivated it is always 100% (fading and other light simulations wont work)
#define BRIGTHNESS_DEFAULT 40	// Default setted brightness, allowed value 0 - 45

// Here defined settings are only used here and not written in the main.c
// You have to set the same adjustments in the .ioc
#define SYS_CLK 72000000 		// Sys clk frequence of the stm32 uC
#define TIMER &htim2			// Used timer for the led data PWM signal
#define TIMER_CHANNEL TIM_CHANNEL_4		// Used timer channel used for the led data PWM signal
#define TIM_PRESCALER 0			// Prescaler settings in the .ioc
#define TIM_ARR 90      		// (SYS_CLK / TIM_ARR) = 800 kHz

// Timer used for the led data signal
extern TIM_HandleTypeDef htim2;

// ---------------------------------------------------------------------
// -------------- Dont´t modify lines below ----------------------------
// ---------------------------------------------------------------------

#define PI 3.14159265
#define DMA_TIMEOUT 5000 //ms
extern uint8_t numberLeds;



void ws281x_init();

/*
 * Translate RGB data into PWM duty cycle and start the DMA.
 * SetLED() / setAllLEDs() should be called first.
 */
void ws281x_send();

/*
 * Set all led´s off
 */
void ws281x_settOff();

/*
 * Set the RGB data for the led with given index.
 * The led is not updated until ws281x_send() is called.
 * @param LEDnum number of the led which color is to set
 * @param color color_t element which store the RGB data of the color
 */
void setLED(uint8_t LEDnum, color_t color);

/*
 * Set the RGB data for the given leds.
 * Starting with the 'firstLed' index all follow leds will be set until 'numberOfLeds' are lightning.
 * @param color color_t element which store the RGB data of the color
 * @param firstLed index of the first LED which should be set
 * @param numberOfLeds total number of leds which should be set
 */
void setSpecificLEDs(color_t color, uint8_t firstLed, uint8_t numberOfLeds);

/*
 * Set all LEDs which are controlled by this mcu
 * The led is not updated until ws281x_send() is called.
 * @param color color_t element which store the RGB data of the color
 */
void setAllLEDs(color_t color);

/*
 * Set the brightness of the leds
 * @param brightness valid input 0-45 for the brightness of the leds
 */
void setBrightness(uint8_t br);

/*
 * Return safed brightness value
 * @return setted brightness value
 */
uint8_t getBrightness();

/*
 * Used to modify stored led Data with brightness value.
 * If Brightness is defines as enabled, this method is called in ws281x_send().
 */
void calculateLedDataWithBrightness();


#define IDX_STARTING_LED 1
#define IDX_NUM_LIGHTNING_LEDS 0
/*
 * Calculate and return the number of controller for the given number of leds
 * If WS2812 is used, the same number is returned.
 * If WS2811 is used, the number is divided by 3. If the given number can not be divided by 3 without a rest,
 * the number will be incresed (idx=false) / decreased (idx=true) until its possible
 * @param numLed 0-255
 * @param idx set true (IDX_STARTING_LED) if the given number is the index of the starting led. Set false (IDX_NUM_LIGHTNING_LEDS) if its the number of Leds which sould lighting.
 * @return number of controller of the leds
 */
uint8_t translateNumLeds_WS2811_WS2812(uint8_t numLed, bool idx);

/*
 * HAL function overwrite to detect the end of the DMA conversation.
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

#endif
