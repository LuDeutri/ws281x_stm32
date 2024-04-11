# WS2811/12 with STM32
(WS2812 is not tested)
This Libary gives simple basic functions to control an ws2811/12 led strip with a stm32 processor.
It is based on an Timer with PWM Output and a DMA.

### Needed RAM:
approx. 234 Bits/Led
+ 5 KB RAM: 170 Leds (ws2811: 510 Leds)
+ 10 KB RAM: 341 Leds (ws2811: 1,023 Leds)
+ 20 KB RAM: 683 Leds (ws2811: 2,049 Leds)
+ 50 KB RAM: 1,709 Leds (ws2811: 5,127 Leds)

WS2811 use one controller for 3 Leds. Thats the reason why you can use 3x more leds.
In ws281x.h NUM_LED must be the total number of the leds. It doesnt matter if ws2811 or 12 is used. The right number ob controller is calculated in ws281x_init().


## To use this libary in your project you have to do following steps:
1. Set the ioc. settings like described below
2. Set the settings in the header file (ws281x.h)
3. Include ws281x.h in your project to use the funtions
4. ws281x_init() must run one time at the beginning.
5. At the end of your loop use ws281x_send() to update the leds
6. Enjoy your led show


# Settings in the .ioc:
## System clock
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/ec8e1c6e-852f-4a23-b266-676ce76d7a26)

 * 72 MHz: Make sure that the timer clock which you are using for your PWM data signal for the leds is also 72 MHz

You can use an other clock frequence but than you have to adjust the timer settings and the settings in ws281x.h like shown below.


## Timer
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/fec5687d-64c1-4594-ad83-b59d353235ee)
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/2798c76b-db0b-433c-9ea3-c9c7a2d6b516)
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/1df7f73e-41f5-45a0-9026-d9f6a2ece1bc)

 * TIM Prescaler: 0
 * TIM ARR: 90-1     // ARR = Sys_clk / 800000
   * If you are using an other system clock frequence, you have to re calculate the ARR for the timer.
  
 * Every timer can be used but you must take attention to the clock frequence of the one you are using. If you are using an other timer with the same timing as here shown, you have to set the timer in the .ioc and change the defined TIMER and TIMER_CHANNEL in the header file ws281x.h. You must synchronize also the DMA channel.
 * If you change the timing too, because the chosen timer dont work with 72 MHz or something else, you have to recalculate the ARR with the given calculation.
 * Example:
   * If you get 50 as result for the ARR change 90-1 into 50-1. In the header file ws281x.h change the 90 into 50 too but without the "-1". 


## DMA
Should be set automtaically if you added the DMA in the timer settings before.

![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/b761a814-86c0-4369-88dd-293d8db43f0c)
 * Direction: Memory to Peripheral
 * Mode: Normal
 * Priority: Medium
 * Peripheral Increment Address: unchecked
 * Peripheral Data Width: Half Word
 * Memory Increment Address: checked
 * Memory Data Width: Byte

# Settings in ws281x.h:
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/78158cad-6d04-40f0-8b74-0a620673ef21)
* Sychronize the uC HAL with your used uC. Here it is used the STM32F1
  
* WS2811
  * 0: WS2812 Leds
  * 1: WS2811 Leds
* USE_RAM_FOR_RESET
  * If information for all led are sent, a reset period is required to start the next sequence about all leds.
  * 1: A defined number (RESET_PERIOD) of 0 values are added to the pwm data array at the end. This needs RAM. Default value is 50.
  * 0: Only one 0 is added to the pwm data array at the end, but a delay about 1ms is added. No extra RAM is needed but you have an short delay in your loop.
* RESET_PERIOD
  * Number of 0 for the reset if ram is used (USE_RAM_FOR_RESET = 1) Otherwise not used
  * 50 is the default value, can be reduced with runtime analyzing. A rest period about 50µs is required.
* NUM_LED
  * Total number of leds
* ENABLE_BRIGTHNESS
  * 0: Birghtness control is disabled. Fading functions will not work without that.
  * 1: Brightness control is activated.
* BRIGHTNESS_DEFAULT
  * 0: min
  * 45: max 

Synchronize the defined variables ( SYS_CLK, TIMER, ...) with the .ioc settings

If you set the defined values above to other values as here described, like WS2811 = 3, everything can happen :) 
