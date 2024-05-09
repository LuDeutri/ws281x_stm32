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
// Used for fadeOneColor(..), fadeOneColor(..), fadeAll()
#define FADE_TIME 30 // Time per brightness level, a higher value makes the fade slower


//  ------- Color wheel fade simulation -----------
// Used for fadeColorWheel()
// All leds lightning in the same color and are fading in different colors of the color wheel
// depending of the defined change rate and the speed
#define WHEEL_FADE_COLOR_CHANGE_RATE 0.2f // Higher value --> color difference per fade higher
#define WHEEL_FADE_CYCLE_TIME  10 // Higher value --> slow rainbow color change


//  ------- Rainbow simulation -----------
// Used for rainbow()
// Rainbow colors are moving in the strip
// The used colors can be modified in color.c rainbowColorReg[].
// Colors can be added or removed without any other necessary changes in the code.
#define RAINBOW_CYCLE_TIME 5 // Higher value --> Rainbow moves slower
#define RAINBOW_SAME_COLOR_LEDS 1 // Number of leds with the same color in the strip


//  ------- Running light simulation -----------
// Used for runningLight(..)
#define RUNNING_LIGHT_TIME 100 // Time one led is lighting, lower value makes the light faster


//  ------- Cyclone light simulation -----------
// Used for cyclone()
#define CYCLONE_RUN_DELAY 30 // Speed of the cyclone. Higher value --> faster cyclone
#define CYCLONE_DIFFERENCE_PER_LED 0.5f // 0.05f ... 100 // Higher value increase the color difference per LED
#define CYCLONE_COLOR_JUMP 6 //1 ... 10 // Higher value increase the color jump with the cyclone run


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
 * All leds light in the same color and are fading in diffrent colors of the color wheel, depending on the defined change rate.
 * With the defines for the rainbow fade simulation the speed and the color change rate can be adjusted.
 */
void fadeColorWheel();

/*
 * Show rainbow led simulation in one led strip.
 * The used colors are stored in an array in color.c (rainbowColorReg).
 * It is easily possible to change that array (remove or add colors). Other changes in the code are not necessary.
 * The speed the rainbow is moving and the number of leds which lightning in the same color
 * can be adjusted in the defined values above in this header file.
 */
void rainbow();

/*
 * Show a running light in the led strip. RUNNING_LIGHT_TIME defines the time one LED is lightning
 * @param color of the running light
 * @param offset at which led the running light should start
 * @param direction 1 running forward, -1 running backward. You can use 2,3,4,... or -2,-3,-4,... that leads to 2,3,4,... leds being skipped.
 * 			0 will return in an error and stop the method
 * @param resetLeds if true all leds are set off in the method. If you want to use multiple
 * 			parallel running lights you must set it false and reset the leds before executing the methods for the running lights.
 * @param runningLightTime initialize the variable with 0. Changes are not necessary. Its used for the method
 * @param runningLightCounter initialize the variable with 0. Changes are not necessary. Its used for the method
 * @return true if one cycle is done, otherwise always false
 */
bool runningLight(color_t color, uint16_t offset, int8_t direction, bool resetLeds, uint32_t* runningLightTime, int32_t* runningLightCounter);

/*
 * Cyclone light simulation is a running color change from the right to the left, back to the right,
 * again to the left and so on. You can modify the color difference per led, the color difference
 * after the cyclone run and the speed of the cyclone with changing the defined values.
 *
 * Attention! This method uses HAL_Delay() and will cause a delay of the main loop!
 */
void cyclone();


#endif
