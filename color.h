/*
 * Contains information and methods for the color of the leds
 */

#ifndef WS281X_STM32_COLOR_H_
#define WS281X_STM32_COLOR_H_

#include <stdint.h>
#include <math.h>

#define PI 3.14159265f

typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_t;

extern const color_t off;
extern const color_t red;
extern const color_t romanticRed;
extern const color_t gold;
extern const color_t orange;
extern const color_t yellow;
extern const color_t green;
extern const color_t blue;
extern const color_t indigo;
extern const color_t violett;
extern const color_t white;

extern color_t colorFadeReg[];
extern size_t colorFadeRegSize;

extern color_t colorBlinkReg[];
extern size_t colorBlinkRegSize;

extern color_t colorRainbowReg[];
extern size_t colorRainbowRegSize;

/*
 * Translate HSV (hue, saturation, value) color code into RGB (red, green, blue)
 * @param hue 0-359, angle in the color wheel
 * @param saturation, 0-1, saturation 0 would return white light
 * @param brightness value, 0-1
 * @return color_t RGB struct element with rgb values
 */
color_t hsv_to_rgb(float hue, float saturation, float brightness);

/*
 * Calculate color of the color wheel with a sinus function and a given phase.
 * The result is a periodic color transition.
 * @param phase of sinus function
 * @return color_t calculated color wheel color
 */
color_t calculateColorWheelColor(float phase);

/*
 * Fade from one color to the other in a given ratio. An interpolate function is used.
 * @param actualColor
 * @param targetColor
 * @param ratio of the new color to the actually one
 * @return color mix from the given colors depending on given ratio
 */
color_t fadeToColor(color_t actualColor, color_t targetColor, float ratio);

#endif
