#include "example.h"

void blinkOneColor(color_t color){
	if(HAL_GetTick() % BLINK_PERIODEN_TIME > BLINK_PERIODEN_TIME/2)
		setAllLEDs(off);
	else
		setAllLEDs(color);
}

uint32_t blinkTime = 0;
uint8_t blinkIndex = 0;
void blinkColors(color_t* color, size_t size){
	blinkOneColor(color[blinkIndex]);

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

void fadeOneColor(color_t color) {
	setAllLEDs(color);
	fadeAll();
}


uint8_t colorIndex = 0;
void fadeColors(color_t* color, size_t size){
	uint8_t tmpBrightness = getBrightness();

	if(tmpBrightness <= 0 && !fadeIn)	// Change the color if the brightness is set to 0
		colorIndex++;
	if(colorIndex >= size)	// Restart if all colors were faded
		colorIndex = 0;

	fadeOneColor(color[colorIndex]); // Fade one color
}

void rainbow(){

	for(uint8_t i=0; i < ((numberLeds-1)-RAINBOW_LED_SAME_COLOR); i += RAINBOW_LED_SAME_COLOR){
		// Get color for led with index i
		color_t colorRainbow = colorRainbowReg[i % colorRainbowRegSize];

		// Set color to all LEDs which show the same color
		for(uint8_t j=0; j < RAINBOW_LED_SAME_COLOR; j++)
			setLED(i+j,colorRainbow);
	}
}

uint32_t runningLightTime = 0;
uint8_t runningLightCounter = 0;
void runningLight(color_t color, uint8_t offset, int8_t direction){
	setAllLEDs(off); // Reset all led data

	if(offset >= numberLeds || offset < 0) // Validate parameter input
		return;

	if(HAL_GetTick() < runningLightTime + RUNNING_LIGHT_TIME) // Make the light faster/slower
		return;
	runningLightTime = HAL_GetTick();

	// Reset Counter and start from begining if led runs throw all leds
	if(offset+runningLightCounter >= numberLeds || offset+runningLightCounter <= 0)
		runningLightCounter = 0;

	setLED(offset+runningLightCounter*direction, color); // Set the lightning LED
}

void cyclon() {
  static uint8_t hue = 0;

	// First slide the led in one direction
	for(int i = 0; i < NUM_LED; i++) {
		// Set the i'th led to red
		setLED(i,hsv_to_rgb(hue++, 1, 1));
		// Show the leds
		ws281x_send();
		// now that we've shown the leds, reset the i'th led to black
		setLED(i,off);
		fadeAll();
		// Wait a little bit before we loop around and do it again
		HAL_Delay(10);
	}
	// Now go in the other direction.
	for(int i = (NUM_LED)-1; i >= 0; i--) {
		// Set the i'th led to red
		setLED(i,hsv_to_rgb(hue++, 1, 1));
		// Show the leds
		ws281x_send();
		// now that we've shown the leds, reset the i'th led to black
		setLED(i,off);
		fadeAll();
		// Wait a little bit before we loop around and do it again
		HAL_Delay(10);
	}
}


void fire(uint8_t direction){
// Array of temperature readings at each simulation cell
  static uint16_t heat[NUM_LED];

  // Step 1.  Cool down every cell a little
    for( uint8_t i = 0; i < NUM_LED; i++) {
    	heat[i] = qsub8( heat[i],  getRandom_u8(0, ((COOLING * 10) / NUM_LED) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( uint8_t k = NUM_LED - 1; k >= 2; k--) {
    	heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( getRandom_u8(0,255) < SPARKING ) {
    	uint8_t y = getRandom_u8(0,7);
    	heat[y] = qadd8( heat[y], getRandom_u8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LED; j++) {
      color_t color = getColorForTemperature(heat[j]);
      uint8_t pixelNumber;
      if( !direction )
    	  pixelNumber = (NUM_LED-1) - j;
      else
    	  pixelNumber = j;

      setLED(pixelNumber,color);
    }
}
