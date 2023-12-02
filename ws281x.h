/*
 * This class handles WS281x led´s based on a stm32 processor with using PWM and DMA.
 * Based on the work of https://controllerstech.com/interface-ws2812-with-stm32/
 *
 * It is important to set the clock configuration in the .ioc, otherwise
 * it will not work.
 * If you are using an other clock speed, you have to calculate the new ARR for the timer which is used
 * for the PWM data signal and modify the defined constants below.
 *
 * Set the following settings in the ioc:
 *
 * System clock: 72 Mhz
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

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stddef.h>
#include <math.h>
#include <stdarg.h>

#include "example.h"
#include "utility.h"
#include "color.h"

#include "../config.h"


// --------------- To set by user ---------------------------
// Here defined setttings are only used here and not written in the main.c
// You have to set the same adjustments in the .ioc

// In the WS2811 led strip are 3 leds controlled by one IC.
// In the WS2812 led strip every led has is one controller IC included.
// ------- WS2812 control is not tested !! -----------
// The reset period can be reduced for the WS2812 (not implemented)
#define USE_WS2811 1	// WS2811: 1 , WS2812: 0

//(NUM_LED defined in config.h)
//#define NUM_LED 				// Total number of leds controlled by mcu doesnt matter if 2812 or 2811

#define SYS_CLK 72000000 		// Sys clk frequence of the stm32 uC
#define TIMER &htim1			// Used timer for the led data PWM signal
#define TIMER_CHANNEL TIM_CHANNEL_1		// Used Timer channel used for the led data PWM signal
#define TIM_PRESCALER 0			// Prescaler settings in the .ioc
#define TIM_ARR 90      		// (SYS_CLK / TIM_ARR) = 800 kHz

#define ENABLE_BRIGHTNESS 1		// 1 enables brigthness control of the LEDs, if deacitvated it is always 100% (fadeing and other light simulations wont work)
#define BRIGTHNESS_DEFAULT 40	// Default setted Brightness, allowed value 0 - 45


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
 * HAL function overwrite to detect the end of the DMA conversation.
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

#endif
