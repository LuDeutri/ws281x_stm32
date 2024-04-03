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
 * System clock: 72 Mhz

## DMA
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/31bd80bb-69da-46a4-9e1a-9ccbd67d8104)
 * Direction: Memory to Peripheral
 * Mode: Normal
 * Priority: Medium
 * Peripheral Increment Address: unchecked
 * Peripheral Data Width: Half Word
 * Memory Increment Address: checked
 * Memory Data Width: Byte

## Timer
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/b4f02c6e-d710-4765-8c59-a689897efb2f)
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/98479e97-f638-447f-8704-6d1a521874ea)
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/87eb4285-8c20-45d7-abea-5e02a8f29d38)

 * TIM Prescaler: 0
 * TIM ARR: 90-1     // ARR = Sys_clk / 800000
   * If you are using an other system clock frequence, you have to re calculate the ARR for the timer.

# Settings in the ws281x.h:
![grafik](https://github.com/LuDeutri/ws281x_stm32/assets/56504337/9b9ab66f-761a-4953-90bd-00ffd03d2da1)

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
