/*
 * Contains led light simulation examples
 */
#ifndef WS281X_STM32_EXAMPLE_H_
#define WS281X_STM32_EXAMPLE_H_

#include "ws281x.h"
#include "color.h"

//  ------- Blink light simulation -----------
#define BLINK_PERIODEN_TIME 1000
#define BLINK_CHANGE_COLOR_TIME 1000


//  ------- Fade light simulation -----------
#define FADE_TIME 30


//  ------- Rainbow simulation -----------
#define RAINBOW_LED_SAME_COLOR 2


//  ------- Running light simulation -----------
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
 * Blink all leds in one color
 * @param color color_t element for the shown color
 */
void blinkOneColor(color_t color);

/*
 * Blink all leds in the same color. Change the color after BLINK_CHANGE_COLOR_TIME
 * @param color pointer to a register list with color_t elements
 * @param size number of elements in color register
 */
void blinkColors(color_t* color, size_t size);

/*
 * Fade all LEDs without changing the color or sequence settings
 * The color has to be set separately
 */
void fadeAll();

/*
 * Fade one color. FADE_TIME defines the time per brightness level
 * ENABLE_BRIGHTNESS (ws281x.h) must be 1 (brightness must be enabled) otherwise the method stops immediately
 * @param color color_t element for the faded color
 */
void fadeOneColor(color_t color);

/*
 * Fade colors which are stored in the given color register.
 * ENABLE_BRIGHTNESS (ws281x.h) must be 1 (brightness must be enabled) otherwise the method stops immediately
 * @param color register of color_t elements
 * @param size number ob elemts in color register
 */
void fadeColors(color_t color[], size_t size);

/*
 * Show rainbow led simulation. Colors are fixed in example.c setted and can be changed there.
 * RAINBOW_LED_SAME_COLOR defines the number of LEDs which show the same color
 */
void rainbow();

/*
 * Show a running light in the LED strip. RUNNING_LIGHT_TIME defines the time one LED is lightning
 * @param color of the running light
 * @param offset at which led the running light should start
 * @param direction 1 running forward, -1 running backward
 */
void runningLight(color_t color, uint8_t offset, int8_t direction);

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
