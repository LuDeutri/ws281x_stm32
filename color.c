#include "color.h"

const color_t off = {0, 0, 0};
const color_t red = {255, 0, 0};
const color_t romanticRed = {237, 28, 26};
const color_t gold = {181, 154, 84};
const color_t orange = {255, 127, 0};
const color_t yellow = {255, 255, 0};
const color_t green = {0, 255, 0};
const color_t blue = {0, 0, 255};
const color_t indigo = {46, 43, 95};
const color_t violett = {136, 0, 255};
const color_t white = {255, 255, 255};


// --------- Arrays used for diffrent color light simulations --------------
// The colors can be changed as desired
color_t colorFadeReg[] = { violett, blue, green, yellow, orange, red};
size_t colorFadeRegSize = sizeof(colorFadeReg)/sizeof(colorFadeReg[0]);

color_t colorBlinkReg[] = { blue, green, red };
size_t colorBlinkRegSize = sizeof(colorBlinkReg)/sizeof(colorBlinkReg[0]);

color_t colorRainbowReg[] = { violett, blue, indigo, green, yellow, orange, red, romanticRed};
size_t colorRainbowRegSize = sizeof(colorRainbowReg)/sizeof(colorRainbowReg[0]);
// -------------------------------------------------------

color_t hsv_to_rgb(float hue, float saturation, float value) {
    color_t rgb;
    float hue_sector, f, p, q, t;

    // Ensure hue is in the range [0, 360)
    hue = fmod(hue, 360.0f);

    // Normalize hue, saturation, and value to the range [0, 1]
    hue /= 360.0f;
    saturation = fmin(fmax(saturation, 0.0f), 1.0f);
    value = fmin(fmax(value, 0.0f), 1.0f);

    if (saturation == 0) {
        // Grayscale, saturation is 0
        rgb.r = rgb.g = rgb.b = (uint8_t)(value * 255);
    } else {
        hue_sector = floorf(hue * 6);
        f = hue * 6 - hue_sector;
        p = value * (1 - saturation);
        q = value * (1 - f * saturation);
        t = value * (1 - (1 - f) * saturation);

        switch ((int)hue_sector) {
            case 0: rgb = (color_t){(uint8_t)(value * 255), (uint8_t)(t * 255), (uint8_t)(p * 255)}; break;
            case 1: rgb = (color_t){(uint8_t)(q * 255), (uint8_t)(value * 255), (uint8_t)(p * 255)}; break;
            case 2: rgb = (color_t){(uint8_t)(p * 255), (uint8_t)(value * 255), (uint8_t)(t * 255)}; break;
            case 3: rgb = (color_t){(uint8_t)(p * 255), (uint8_t)(q * 255), (uint8_t)(value * 255)}; break;
            case 4: rgb = (color_t){(uint8_t)(t * 255), (uint8_t)(p * 255), (uint8_t)(value * 255)}; break;
            case 5: // fall through
            case 6: rgb = (color_t){(uint8_t)(value * 255), (uint8_t)(p * 255), (uint8_t)(q * 255)}; break;
            default: break; // should not happen, just for safety
        }
    }

    return rgb;
}

color_t calculateColorWheelColor(float changeRate){
	color_t color;

	// Calculation of the phase angle based on the rate of change
	float phaseAngle = fmod(changeRate / 51.0 * PI, 2 * PI);

	// Calculate color with sinus function
	color.r = (uint8_t)(127.5 * (1 + sin(phaseAngle))); 			// red
	color.g = (uint8_t)(127.5 * (1 + sin(phaseAngle - PI / 3))); 	// green
	color.b = (uint8_t)(127.5 * (1 + sin(phaseAngle - 2*PI / 3))); // blue

	return color;
}

color_t fadeToColor(color_t actualColor, color_t targetColor, float ratio) {

    // Interpolate between colors
    uint8_t r = actualColor.r + (int16_t)((targetColor.r - actualColor.r) * ratio);
    uint8_t g = actualColor.g + (int16_t)((targetColor.g - actualColor.g) * ratio);
    uint8_t b = actualColor.b + (int16_t)((targetColor.b - actualColor.b) * ratio);

    return (color_t){r,g,b};
}
