#include "color.h"

const color_t off = {0, 0, 0};
const color_t red = {237, 28, 26};
const color_t romanticRed = {255, 209, 194};
const color_t gold = {181, 154, 84};
const color_t orange = {255, 127, 0};
const color_t yellow = {255, 255, 0};
const color_t green = {0, 255, 0};
const color_t blue = {0, 0, 255};
const color_t indigo = {46, 43, 95};
const color_t violett = {136, 0, 255};
const color_t white = {255, 255, 255};


// ---------Variables for example.h methods --------------
color_t colorFadeReg[] = { violett, blue, green, yellow, orange, red };
size_t colorFadeRegSize = sizeof(colorFadeReg)/sizeof(colorFadeReg[0]);

color_t colorBlinkReg[] = { blue, green, red };
size_t colorBlinkRegSize = sizeof(colorBlinkReg)/sizeof(colorBlinkReg[0]);

color_t colorRainbowReg[] = { red, orange, yellow, green, blue, indigo, violett };
size_t colorRainbowRegSize = sizeof(colorRainbowReg)/sizeof(colorRainbowReg[0]);
// -------------------------------------------------------

// --------------- Color for fire simulation --------------
DEFINE_GRADIENT_PALETTE(FlameColorsPalette,
    {0,   {0,   0,   0}},    // Dunkelblau
    {16,  {0,   0,  64}},    // Blau
    {32,  {0,   0, 128}},    // Dunkelblau
    {48,  {0,   0, 192}},    // Blau
    {64,  {0,   0, 255}},    // Blau
    {80,  {0,  64, 255}},    // Hellblau
    {96,  {0, 128, 255}},    // Hellblau
    {112, {0, 192, 255}},    // Hellblau
    {128, {0, 255, 255}},    // Türkis
    {144, {64, 255, 255}},   // Helltürkis
    {160, {128,255, 255}},   // Helltürkis
    {176, {192,255, 255}},   // Helltürkis
    {192, {255,255, 255}},   // Weiß
    {208, {255,224, 192}},   // Heller Rotton
    {224, {255,160, 128}},   // Rotton
    {240, {255, 96,  64}},   // Dunkler Rotton
    {255, {255,  0,   0}}    // Rot
);
// --------------- /Color for fire simulation --------------


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


color_t getColorForTemperature(uint8_t temperature) {
	// Iterate through the palette entries to find the appropriate color
    for (int i = 0; i < FlameColorsPalette.numEntries - 1; ++i) {
        if (temperature >= FlameColorsPalette.entries[i].position &&
            temperature < FlameColorsPalette.entries[i + 1].position) {

        	// Interpolate between the two colors based on temperature position
            uint8_t range = FlameColorsPalette.entries[i + 1].position - FlameColorsPalette.entries[i].position;
            uint8_t positionInRange = temperature - FlameColorsPalette.entries[i].position;
            color_t color1 = FlameColorsPalette.entries[i].color;
            color_t color2 = FlameColorsPalette.entries[i + 1].color;

            color_t resultColor;
            resultColor.r = color1.r + (positionInRange * (color2.r - color1.r)) / range;
            resultColor.g = color1.g + (positionInRange * (color2.g - color1.g)) / range;
            resultColor.b = color1.b + (positionInRange * (color2.b - color1.b)) / range;

            return resultColor;
        }
    }

    // Default: Return the color at the last position
    return FlameColorsPalette.entries[FlameColorsPalette.numEntries - 1].color;
}
