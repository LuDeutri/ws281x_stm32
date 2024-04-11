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
#define RAINBOW_COLOR_CHANGE_FACTOR 15 // If higher the color difference per led is higher
#define RAINBOW_DELAY 100 // change the color again after X milliseconds


//  ------- Running light simulation -----------
// Time one led is lighting
#define RUNNING_LIGHT_TIME 100


//  ------- Fire light simulation -----------
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  50

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120



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
 * @param direction 1 running forward, -1 running backward. You can use 2,3,4,... or -2,-3,-4,... that leads to 2,3,4,... leds being skipped. 0 will return in an error and stop the method
 * @return true if one cylce is done, otherwise always false
 */
bool runningLight(color_t color, uint16_t offset, int8_t direction);

/*
 * Cyclon lieght simulation
 */
void cyclon();

/*
 * Fire light simulation. The colors are defined in a 16-value-palette which is defined in color.h
 * @param direction defines the base and the direction of the flame 0: LED0 = base, 1: LED_NUM-1 = base,
 */
void fire(uint8_t direction);



#endif
