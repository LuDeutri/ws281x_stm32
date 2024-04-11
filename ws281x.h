/*
 * This class handles WS281x led´s based on a stm32 processor with using PWM and DMA.
 *
 * Based on the work of:
 * Here you can find a detailed description how the control of the ws281x leds work:
 * https://controllerstech.com/interface-ws2812-with-stm32/
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
#include "color.h"

// --------------- To set by user ---------------------------

#define USE_WS2811 1	// WS2811: 1 , WS2812: 0 // WS2812 untested!

// If information for all led are sent, a reset period is required to start the next sequence about all leds. In this time the pwm signal must be low.
// First way: Set the last bit in the array to 0 and use an HAL delay. This way saves RAM.
// Second Way: Set enough bits to 0 at the end of the array and send them. This needs more RAM but you dont have an delay in your loop.
#define USE_RAM_FOR_RESET 1

// Can be reduced with running time analyze of your system
#define RESET_PERIOD 50 // reset_period, 1.25µs * RESET_PERIOD, You need at least 50µs @800kHz (datasheet)

#define NUM_LED 15				// Total number of leds, doesn't matter if WS2812 or WS2811
#define ENABLE_BRIGHTNESS 1		// 1: enables brightness control of the leds, if deactivated it is always 100% (fading and other light simulations wont work)
#define BRIGTHNESS_DEFAULT 40	// Default setted brightness, allowed value 0 - 45

// Here defined settings are only used here and not written in the main.c
// You have to set the same adjustments in the .ioc
#define SYS_CLK 72000000 		// Sys clk frequence of the stm32 uC
#define TIMER &htim1			// Used timer for the led data PWM signal
#define TIMER_CHANNEL TIM_CHANNEL_1		// Used timer channel used for the led data PWM signal
#define TIM_PRESCALER 0			// Prescaler settings in the .ioc
#define TIM_ARR 90      		// (SYS_CLK / TIM_ARR) = 800 kHz (400kHz are possible too, read the documentation for the ws281x leds)

// Timer used for the led data signal
extern TIM_HandleTypeDef htim1;

// ---------------------------------------------------------------------
// -------------- Dont´t modify lines below ----------------------------
// ---------------------------------------------------------------------

#define DMA_TIMEOUT 5000 //ms
extern uint16_t numberLeds;



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
void setLED(uint16_t LEDnum, color_t color);

/*
 * Set the RGB data for the given leds.
 * Starting with the 'firstLed' index all follow leds will be set until 'numberOfLeds' are lightning.
 * All other leds stay with the same settings like before.
 * @param color color_t element which store the RGB data of the color
 * @param firstLed index of the first LED which should be set
 * @param numberOfLeds total number of leds which should be set
 */
void setSpecificLEDs(color_t color, uint16_t firstLed, uint16_t numberOfLeds);

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


/*
 * Calculate and return the number of controller for the given number of leds
 * @param number of leds
 * @return number of controller of the leds
 */
uint16_t translateNumLeds_WS2811_WS2812(uint16_t numLed);

/*
 * HAL function overwrite to detect the end of the DMA conversation and set an boolean flag (dataSentFlag) true.
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

#endif
