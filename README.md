# WS2811/12 with STM32
(WS2812 is not tested)
This Libary gives simple basic functions to control an ws2811/12 led strip with a stm32 processor.
It is based on an Timer with PWM Output and a DMA.
To use this libary in your project you have to do following steps:

1. Set the ioc. settings like described below
2. Set the settings in the header file (ws281x.h)
3. Include ws281x.h in your project to use the funtions
4. At the end of your loop use ws281x_send() to update the leds
5. Enjoy your led show


# Settings in the .ioc:
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/ec8e1c6e-852f-4a23-b266-676ce76d7a26)

 * System clock: 72 MHz: Make sure that the timer clock which you are using for your PWM data signal for the leds is also 72 MHz

## Timer
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/cae1c71b-bb91-4471-b707-c388221947f6)
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/2798c76b-db0b-433c-9ea3-c9c7a2d6b516)
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/09e2bb1d-0646-4b09-ba16-66b18cf7e17a)

 * TIM Prescaler: 0
 * TIM ARR: 90-1     // ARR = Sys_clk / 800000
   * If you are using an other system clock frequence, you have to re calculate the ARR for the timer.


## DMA
Should be set automtaically if you added the DMA in the timer settings before.

![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/92ba7984-5581-4643-8146-894de376d872)
 * Direction: Memory to Peripheral
 * Mode: Normal
 * Priority: Medium
 * Peripheral Increment Address: unchecked
 * Peripheral Data Width: Half Word
 * Memory Increment Address: checked
 * Memory Data Width: Byte

# Settings in the ws281x.h:
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/6134a18b-bc01-4437-a57a-15a76f3b8b19)
* Sychronize the uC HAL with your used uC. Here it is used the STM32F1
* Sychronize the project include paths if your are using constants. If not let this part empty.
* WS2811
  * 0: WS2812 Leds
  * 1: WS2811 Leds
* NUM_LED
  * Total number of leds
* ENABLE_BRIGTHNESS
  * 0: Birghtness control is disabled. Fading functions will not work without that.
  * 1: Brightness control is activated.
* BRIGHTNESS_DEFAULT
  * 0: min
  * 45: max 
Synchronize the defined variables with the .ioc settings
