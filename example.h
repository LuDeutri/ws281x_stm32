/*
 * Contains led light simulation examples
 */
#ifndef WS281X_STM32_EXAMPLE_H_
#define WS281X_STM32_EXAMPLE_H_

#include "ws281x.h"
#include "color.h"

//  ------- Blink light simulation -----------
// If you want to blink one time per color BLINK_PERIODEN_TIME and BLINK_CHANGE_COLOR_TIME must be the same
// If you want (for example) to blink 2 times per color, BLINK_PERIODEN_TIME must be the half of BLINK_CHANGE_COLOR_TIME
#define BLINK_PERIODEN_TIME 1000 // Blink faster with an low value: 1000 means 500ms on, 500ms off
#define BLINK_CHANGE_COLOR_TIME 1000 // Change the color after 1000ms


//  ------- Fade light simulation -----------
// Time per brightness level
#define FADE_TIME 30


//  ------- Rainbow simulation -----------
// Number of leds which are showing the same color
#define RAINBOW_COLOR_CHANGE_RATE 1 // Higher value --> color difference per led is higher
#define RAINBOW_CYCLE_TIME  500 // Higher value --> slow rainbow color change


//  ------- Running light simulation -----------
#define RUNNING_LIGHT_TIME 100 // Time one led is lighting, lower value makes the light faster


//  ------- Cyclone light simulation -----------
#define CYCLONE_RUN_DELAY 30 // Speed of the cyclone. Higher value --> faster cyclone
#define CYCLONE_DIFFERENCE_PER_LED 0.5f // 0.05f ... 100 // Higher value increase the color difference per LED
#define CYCLONE_COLOR_JUMP //1 ... 10 // Higher value increase the color jump with the cyclone run




/*
 * Blink all leds with index inside the given parameters in one color
 * @param color color_t element for the shown color
 * @param firstLed index of the first LED which should be set
 * @param numberOfLeds total number of leds which should be set
 */
void blinkOneColor(color_t color, uint16_t firstLed, uint16_t numberOfLeds);

/*
 * Blink all leds in the same color. Change the color after BLINK_CHANGE_COLOR_TIME
 * @param color pointer to a register list with color_t elements
 * @param size number of elements in color register
 * @param firstLed index of the first LED which should be set
 * @param numberOfLeds total number of leds which should be set
 */
void blinkColors(color_t* color, size_t size, uint16_t firstLed, uint16_t numberOfLeds);

/*
 * Fade all LEDs without changing the color or sequence settings
 * The color has to be set separately
 */
void fadeAll();

/*
 * Fade one color. FADE_TIME defines the time per brightness level
 * ENABLE_BRIGHTNESS (ws281x.h) must be 1 (brightness must be enabled) otherwise the method stops immediately
 * @param color color_t element for the faded color
 * @param firstLed index of the first LED which should be set
 * @param numberOfLeds total number of leds which should be set
 */
void fadeOneColor(color_t color, uint16_t firstLed, uint16_t numberOfLeds);

/*
 * Fade colors which are stored in the given color register.
 * ENABLE_BRIGHTNESS (ws281x.h) must be 1 (brightness must be enabled) otherwise the method stops immediately
 * @param color register of color_t elements
 * @param size number ob elemts in color register
 * @param firstLed index of the first LED which should be set
 * @param numberOfLeds total number of leds which should be set
 */
void fadeColors(color_t color[], size_t size, uint16_t firstLed, uint16_t numberOfLeds);

/*
 * Show rainbow led simulation. Colors are fixed in example.c setted and can be changed there.
 * RAINBOW_LED_SAME_COLOR defines the number of LEDs which show the same color
 */
void rainbow();

/*
 * Show a running light in the LED strip. RUNNING_LIGHT_TIME defines the time one LED is lightning
 * @param color of the running light
 * @param offset at which led the running light should start
 * @param direction 1 running forward, -1 running backward. You can use 2,3,4,... or -2,-3,-4,... that leads to 2,3,4,... leds being skipped.
 * 			0 will return in an error and stop the method
 * @param resetLeds if true all leds are set off in the method. If you want to use multiple
 * 			parallel running lights you must set it false and reset the leds before executing the methods for the running lights.
 * @return true if one cycle is done, otherwise always false
 */
bool runningLight(color_t color, uint16_t offset, int8_t direction, bool resetLeds);

/*
 * Cyclone light simulation is a running color change from the right to the left, back to the right,
 * again to the left and so on. You can modify the color difference per led, the color difference
 * after the cyclone run and the speed of the cyclone with changing the defined values.
 *
 * Attention! This method uses HAL_Delay() and will cause a delay of the main loop!
 */
void cyclone();


#endif
