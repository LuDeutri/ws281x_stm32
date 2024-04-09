#include "ws281x.h"

//---------------------------------------------------------------
//----Dont`t change things below --------------------------------
//---------------------------------------------------------------


uint8_t numberLeds = 0; // set in ws281x_init()
uint8_t brightness = 0; // set do default in init()
bool dataSentFlag; // True if DMA action is finished and the next one can start

#if USE_WS2811
	uint8_t ledData[NUM_LED/3][3]; // Store led color data in RGB
	uint8_t ledMod[NUM_LED/3][3]; // Store led color data in RGB with brightness modification
	uint8_t pwmData[24*NUM_LED/3]; // Store led color and brightness data in bit format for the WS281x Led's
#else
	uint8_t ledData[NUM_LED][3];
	uint8_t ledMod[NUM_LED][3];
	uint8_t pwmData[24*NUM_LED];
#endif

void ws281x_init(void) {
	dataSentFlag = 0; // Initial DMA conversation status

	// Set number of controller of the leds depending if WS2811 or WS2812 is used
	numberLeds = translateNumLeds_WS2811_WS2812(NUM_LED, false);

	// Set Brightness initial value
	if(ENABLE_BRIGHTNESS)
		setBrightness(BRIGTHNESS_DEFAULT);

	// Reset all leds
	ws281x_settOff(); // Initial state
}

void ws281x_send (void){
	uint16_t indx=0;
	uint32_t color;

	for (int i= 0; i<numberLeds; i++){

		if(ENABLE_BRIGHTNESS)
			calculateLedDataWithBrightness(); // Manipulate ledData with brightness factor

		// Safe led data binary composed
		if(ENABLE_BRIGHTNESS)
			color = ((ledMod[i][0]<<16) | (ledMod[i][1]<<8) | (ledMod[i][2]));
		else
			color = ((ledData[i][0]<<16) | (ledData[i][1]<<8) | (ledData[i][2]));

		// Translate RGB data in binary format for WS281x leds
		// The clk configuration in the .ioc must be set to:   clk:72 Mhz, Arr: 90
		for (int i=23; i>=0; i--){
			if (color&(1<<i))
				pwmData[indx] = 60;		// 2/3 of 90 (SysClk:72MHz/ARR:90 --> TimerFreq:800kHz)
			else
				pwmData[indx] = 30;		// 1/3 of 90

			indx++;
		}
	}
	pwmData[indx] = 0; // Reset period with Delay minimum 270 us

	HAL_TIM_PWM_Start_DMA(TIMER, TIMER_CHANNEL, (uint32_t *)pwmData, indx);

	uint32_t dmaTime = 0;
	while (!dataSentFlag){
		if(dmaTime == 0) dmaTime = HAL_GetTick();
		// Break condition if DMA dont answer stop it
		if(HAL_GetTick() > dmaTime + DMA_TIMEOUT) return;
	}
	dataSentFlag = 0;
	dmaTime = 0; // Reset break condition

	HAL_Delay(1); // Delay for reset period
}

void setLED(uint8_t LEDnum, color_t color){

	// Validate paramters
	if(color.r % 2 != 0) color.r--;
	if(color.g % 2 != 0) color.g--;
	if(color.b % 2 != 0) color.b--;

	// Set LED RGB data for binary data transition
	ledData[LEDnum][0] = color.b;
	ledData[LEDnum][1] = color.r;
	ledData[LEDnum][2] = color.g;
}

void setSpecificLEDs(color_t color, uint8_t firstLed, uint8_t numberOfLeds){
	// If the ws2811 is used, one controller is used for 3 leds
	firstLed = translateNumLeds_WS2811_WS2812(firstLed, true);
	numberOfLeds = translateNumLeds_WS2811_WS2812(numberOfLeds, false);

	// Check parameter
	if(numberOfLeds > NUM_LED)
		numberOfLeds = NUM_LED;
	if(firstLed+numberOfLeds >= NUM_LED)
		firstLed = NUM_LED - numberOfLeds;


	for(uint8_t i = firstLed; i < firstLed+numberOfLeds; i++)
		setLED(i,color);
}

void setAllLEDs(color_t color){
	for(uint8_t i = 0; i<numberLeds;i++)
		setLED(i,color);
}

void calculateLedDataWithBrightness(){
	if(!ENABLE_BRIGHTNESS)
		return;
	if(brightness > 45)
		brightness = 45;

	for (int i=0; i<numberLeds; i++){
		for (int j=0; j<3; j++){
			float angle = 90-brightness;  // in degrees
			angle = angle*PI / 180;  // in rad
			(ledMod[i][j]) = (ledData[i][j])/(tan(angle));
			if(ledMod[i][j] % 2 != 0) (ledMod[i][j])--;
		}
	}
}

void setBrightness(uint8_t br){
	if(!ENABLE_BRIGHTNESS)
		return;

	if(br > 45)
		br = 45;

	brightness = br;
}

uint8_t getBrightness(){
	if(ENABLE_BRIGHTNESS == 0)
		return 0;
	return brightness;
}

void ws281x_settOff(){
	setAllLEDs(off);
	ws281x_send();
}

uint8_t translateNumLeds_WS2811_WS2812(uint8_t numLed, bool idx){
	if(USE_WS2811){
		// If the number is lower than 3, return 0. In this case the first 3 LED of the first controller are lightning. Less is with WS2811 not possible.
		if(numLed < 3 && idx) return 0; 	// IDX_STARTING_LED
		if(numLed < 3 && !idx) return 1; 	// IDX_NUM_LIGHTNING_LEDS

		// Be sure the given number can be devided by 3
		while(numLed % 3 != 0){
			if(idx == IDX_NUM_LIGHTNING_LEDS) numLed++;
			else numLed--;
		}
		// Divide the result by 3
		numLed /= 3;
	}
	return numLed;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	HAL_TIM_PWM_Stop_DMA(htim, TIMER_CHANNEL);
	dataSentFlag = 1;
}
