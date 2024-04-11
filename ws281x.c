#include "ws281x.h"

//---------------------------------------------------------------
//----Dont`t change things below --------------------------------
//---------------------------------------------------------------


uint16_t numberLeds = 0; // set in ws281x_init()
uint8_t brightness = 0; // set do default in init()
bool dataSentFlag; // True if DMA action is finished and the next one can start

// To seperate the information for the diffrent leds there must be a reset cycle about a given time. In this time the pwm signal must be low.
// First way: Set the last bit in the array to 0 and use an HAL delay. This way saves RAM.
// Second Way: Set enough bits to 0 at the end of the array and send them. This needs more RAM but you dont have an delay in your loop.
#if USE_RAM_FOR_RESET
	#define USED_RESET_PERIOD RESET_PERIOD
#else
	#define USED_RESET_PERIOD 1
#endif

#if USE_WS2811
	uint8_t ledData[NUM_LED/3][3]; // Store led color data in RGB
	uint8_t ledMod[NUM_LED/3][3]; // Store led color data in RGB with brightness modification
	uint8_t pwmData[(24*NUM_LED/3) + USED_RESET_PERIOD]; // Store led color and brightness data in bit format for the WS281x led's
#else
	uint8_t ledData[NUM_LED][3];
	uint8_t ledMod[NUM_LED][3];
	uint8_t pwmData[24*NUM_LED + USED_RESET_PERIOD];
#endif


void ws281x_init(void) {

	dataSentFlag = 0; // Initial DMA conversation status

	// Set number of controller of the leds depending if WS2811 or WS2812 is used
	numberLeds = translateNumLeds_WS2811_WS2812(NUM_LED);

	// Set Brightness initial value
	if(ENABLE_BRIGHTNESS)
		setBrightness(BRIGTHNESS_DEFAULT);

	// Reset all leds
	ws281x_settOff(); // Initial state
}

void ws281x_send (void){
	uint16_t indx=0;
	uint32_t color;

	if(ENABLE_BRIGHTNESS)
		calculateLedDataWithBrightness(); // Manipulate ledData with brightness factor

	for (uint16_t i= 0; i<numberLeds; i++){

		// Safe led data binary composed
		if(ENABLE_BRIGHTNESS)
			color = ((ledMod[i][0]<<16) | (ledMod[i][1]<<8) | (ledMod[i][2]));
		else
			color = ((ledData[i][0]<<16) | (ledData[i][1]<<8) | (ledData[i][2]));

		// Translate RGB data in binary format for WS281x leds
		// The clk configuration in the .ioc must be set to:   clk:72 MHz, ARR: 90
		for (int8_t i=23; i>=0; i--){
			if (color&(1<<i))
				pwmData[indx] = (uint8_t)(0.6667f * TIM_ARR);		// 2/3 of ARR (SysClk:72MHz/ARR:90 --> TimerFreq:800kHz)
			else
				pwmData[indx] = (uint8_t)(0.3334f * TIM_ARR);		// 1/3 of ARR

			indx++;
		}
	}
	// Reset period
	// If no delay is wihsed (USE_RAM_FOR_RESET) RESET_PERIOD elements are saved with 0 at the end of the array
	// If the reset can work with an delay only one last element with 0 is added
	while(indx < (sizeof(pwmData)/sizeof(pwmData[0]))){
		pwmData[indx] = 0; // Reset period
		indx++;
	}

	// Start the PWM transmission
	HAL_TIM_PWM_Start_DMA(TIMER, TIMER_CHANNEL, (uint32_t *)pwmData, indx);

	// Wait until the DMA interrupts and the message is sent
	uint32_t dmaTime = 0;
	while (!dataSentFlag){
		if(dmaTime == 0) dmaTime = HAL_GetTick();
		// Break condition if DMA dont answer stop it
		if(HAL_GetTick() > dmaTime + DMA_TIMEOUT) return;
	}
	dataSentFlag = 0; // Reset DMA interrupt flag
	dmaTime = 0; // Reset break condition

	if(!USE_RAM_FOR_RESET)
		HAL_Delay(1); // Delay for reset period
}

void setLED(uint16_t LEDnum, color_t color){

	// Validate paramters
	if(color.r % 2 != 0) color.r--;
	if(color.g % 2 != 0) color.g--;
	if(color.b % 2 != 0) color.b--;

	// Set LED RGB data for binary data transition
	ledData[LEDnum][0] = color.b;
	ledData[LEDnum][1] = color.r;
	ledData[LEDnum][2] = color.g;
}

void setSpecificLEDs(color_t color, uint16_t firstLed, uint16_t numberOfLeds){
	// If the ws2811 is used, one controller is used for 3 leds
	firstLed = translateNumLeds_WS2811_WS2812(firstLed);
	numberOfLeds = translateNumLeds_WS2811_WS2812(numberOfLeds);

	// Check parameter
	if(numberOfLeds > NUM_LED)
		numberOfLeds = NUM_LED;
	if(firstLed+numberOfLeds >= NUM_LED)
		firstLed = NUM_LED - numberOfLeds;


	for(uint16_t i = firstLed; i < firstLed+numberOfLeds; i++)
		setLED(i,color);
}

void setAllLEDs(color_t color){
	for(uint16_t i = 0; i<numberLeds;i++)
		setLED(i,color);
}

void calculateLedDataWithBrightness(){
    if(!ENABLE_BRIGHTNESS)
        return;

    if(brightness > 45)
        brightness = 45;

    float angle = 90 - brightness;  // in Grad
    angle = angle * PI / 180;  // in rad
    float tanAngle = tan(angle);

    for (uint16_t i = 0; i < numberLeds; i++){
        float factor = ledData[i][0] / tanAngle;
        ledMod[i][0] = (uint8_t)factor;
        factor = ledData[i][1] / tanAngle;
        ledMod[i][1] = (uint8_t)factor;
        factor = ledData[i][2] / tanAngle;
        ledMod[i][2] = (uint8_t)factor;
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

uint16_t translateNumLeds_WS2811_WS2812(uint16_t numLed){
	if(USE_WS2811){
		// Be sure the given number can be devided by 3
		while(numLed % 3 != 0)
			numLed++;
		// Divide the result by 3
		numLed /= 3;
	}
	return numLed;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	HAL_TIM_PWM_Stop_DMA(htim, TIMER_CHANNEL);
	dataSentFlag = 1;
}
