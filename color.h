/*
 * color.h
 *
 *  Created on: Dec 1, 2023
 *      Author: lucad
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

// -------- Fire simulation --------------------
#define DEFINE_GRADIENT_PALETTE(name, ...) \
    const gradientPalette_t name = { sizeof((paletteEntry_t[]){__VA_ARGS__}) / sizeof(paletteEntry_t), {__VA_ARGS__} }

typedef struct{
	uint8_t position;
	color_t color;
} paletteEntry_t;

typedef struct{
	uint8_t numEntries;
	paletteEntry_t entries[];
} gradientPalette_t;
// -------- /Fire simulation --------------------

extern color_t colorFadeReg[];
extern size_t colorFadeRegSize;

extern color_t colorBlinkReg[];
extern size_t colorBlinkRegSize;

/*
 * Translate HSV (hue, saturation, value) color code into RGB (red, green, blue)
 * @param hue 0-359, angle in the color wheel
 * @param saturation, 0-1, saturation 0 would return white light
 * @param brightness value, 0-1
 * @return color_t RGB struct element with rgb values
 */
color_t hsv_to_rgb(float hue, float saturation, float brightness);

/*
 * Translate temperature value in color value for fire light simulation with 16-value-palette.
 * Temperature values between 2 safed elements in the palett result in a mixed colour return of the two nearest
 * palett values
 * This method is based on HeatColor() of FastLED.
 * @param temperature value 0-255
 * @return color_t elemt based on given temperature
 */
color_t getColorForTemperature(uint8_t temperature);

/*
 * Calculate rainbow colors.
 * @param changeRate is the factor for the color difference per led
 * @return color_t rainbow color for one led
 */
color_t calculateRainbowColor(uint16_t phase);

#endif
