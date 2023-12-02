#ifndef WS281X_STM32_UTILITY_H_
#define WS281X_STM32_UTILITY_H_

#include <stdlib.h>
#include <stdint.h>
#include "color.h"


int getRandomNumberInRange(int min, int max);
uint8_t getRandom_u8(uint8_t min, uint8_t max);

uint8_t qsub8(uint8_t a, uint8_t b);
uint8_t qadd8(uint8_t a, uint8_t b);


#endif
