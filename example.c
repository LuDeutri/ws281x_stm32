#include "example.h"

void blinkOneColor(color_t color, uint16_t firstLed, uint16_t numberOfLeds){

	if(HAL_GetTick() % BLINK_PERIODEN_TIME > BLINK_PERIODEN_TIME/2)
		setSpecificLEDs(off, firstLed, numberOfLeds);
	else
		setSpecificLEDs(color, firstLed, numberOfLeds);
}

uint32_t blinkTime = 0;
uint8_t blinkIndex = 0;
void blinkColors(color_t* color, size_t size, uint16_t firstLed, uint16_t numberOfLeds){
	blinkOneColor(color[blinkIndex], firstLed, numberOfLeds);

	// Change the color if the leds are currently off
	if(!(HAL_GetTick() % BLINK_PERIODEN_TIME > BLINK_PERIODEN_TIME/2))
		return;

	if(HAL_GetTick() > blinkTime + BLINK_CHANGE_COLOR_TIME){ // Set next color after BLINK_CHANGE_COLOR_TIME
		blinkIndex++;
		blinkTime = HAL_GetTick();

		if(blinkIndex >= size) // Reset if all colors were shown
			blinkIndex = 0;
	}
}

bool fadeIn = false;
uint32_t fadeTime = 0;
void fadeAll(){
	// If brightness ctrl is deactivated, set all leds to the given color and stop the method
	if(!ENABLE_BRIGHTNESS)
		return;

	uint8_t tmpBrightness = getBrightness();

	if(tmpBrightness <= 0)	// start fading in if min brightness is reached
		fadeIn = true;
	else if(tmpBrightness >=45)	// end fading in if max brightness is reached
		fadeIn = false;

	// Wait with the next fade sequence
	if(HAL_GetTick() < fadeTime + FADE_TIME)
		return;

	if(tmpBrightness > 0 && !fadeIn)		// fade out
		setBrightness(tmpBrightness-1);
	else if(tmpBrightness < 45 && fadeIn)	// fade in
		setBrightness(tmpBrightness+1);

	fadeTime = HAL_GetTick();
}

void fadeOneColor(color_t color, uint16_t firstLed, uint16_t numberOfLeds) {
	setSpecificLEDs(color, firstLed, numberOfLeds);
	fadeAll();
}

uint8_t colorIndexFade = 0;
void fadeColors(color_t* color, size_t size, uint16_t firstLed, uint16_t numberOfLeds){
	uint8_t tmpBrightness = getBrightness();

	if(tmpBrightness <= 0 && !fadeIn)	// Change the color if the brightness is set to 0
		colorIndexFade++;
	if(colorIndexFade >= size)	// Restart if all colors were faded
		colorIndexFade = 0;

	fadeOneColor(color[colorIndexFade], firstLed, numberOfLeds); // Fade one color
}

uint8_t colorWheelPhase = 0;
void rainbow(){

	static uint32_t lastCycleTime = 0;
	uint32_t currentTime = HAL_GetTick();

	if (currentTime - lastCycleTime < RAINBOW_CYCLE_TIME / numberLeds) // Prüfen, ob es Zeit ist, die nächste LED im Regenbogen zu aktualisieren
	        return;

	// If less than 3 leds are used, stop the method because no rainbow can be shown
	if(numberLeds < 3)
		return;

	for(uint16_t i=0; i < numberLeds; i++){
		setLED(i,calculateRainbowColor(colorWheelPhase));
		colorWheelPhase += RAINBOW_COLOR_CHANGE_RATE; // Änderungsrate für alle LEDs gleichmäßig anwenden
   }
    lastCycleTime = currentTime;
}

uint32_t runningLightTime = 0;
int32_t runningLightCounter = 0;
bool runningLight(color_t color, uint16_t offset, int8_t direction, bool resetLeds){

	if(offset >= numberLeds) // Validate parameter input
		return false;
	if(direction == 0)
		return false;

	if(HAL_GetTick() < runningLightTime + RUNNING_LIGHT_TIME) // Make the light faster/slower
		return false;
	runningLightTime = HAL_GetTick();

	// Reset Counter and start from beginning if led runs throw all leds
	// Return true to give a signal that one cycle is finished
	if(offset + runningLightCounter >= numberLeds){
		runningLightCounter = 0;
		return true;
	}

	if(resetLeds) setAllLEDs(off); // Reset all led data

	setLED(offset + (runningLightCounter*direction), color); // Set the lightning led
	runningLightCounter++;

	return false;
}

void cyclone() {
    static uint8_t hue = 0;

    // First slide the led in one direction
    for (uint16_t i = 0; i < numberLeds; i++) {
        // Set the i'th led to red
    	setLED(i, hsv_to_rgb(hue+=6, 1, 1));
        // Show the leds
        ws281x_send();

        // Fade out the color
        for (float ratio = 1.0; ratio >= 0.0; ratio -= CYCLONE_DIFFERENCE_PER_LED) {
            fadeToColor(hsv_to_rgb(hue, 1, 1), off, ratio);
            ws281x_send();
            HAL_Delay(CYCLONE_RUN_DELAY); // Adjust delay time as needed
        }
    }
    // Now go in the other direction.
    for (int32_t i = (numberLeds)-1; i >= 0; i--) {
        // Set the i'th led to red
    	setLED(i, hsv_to_rgb(hue+=6, 1, 1));
        // Show the leds
        ws281x_send();

        // Fade out the color
        for (float ratio = 1.0; ratio >= 0.0; ratio -= CYCLONE_DIFFERENCE_PER_LED) {
            fadeToColor(hsv_to_rgb(hue, 1, 1), off, ratio);
            ws281x_send();
            HAL_Delay(CYCLONE_RUN_DELAY); // Adjust delay time as needed
        }
    }
}
