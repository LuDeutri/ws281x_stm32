#include "utility.h"

int getRandomNumberInRange(int min, int max) {
    // Check the validity of the parameters
    if (min > max) {
        // Unequal parameters are invalid; return an error value or handle the error in another way
        return 0;
    }

    // Calculate the range of random numbers
    int range = abs(max - min) + 1;

    // Calculate a random number within the range and shift it accordingly
    int randomNumber = rand() % range + min;

    return randomNumber;
}

uint8_t getRandom_u8(uint8_t min, uint8_t max){
	if(min>max) return 0;

	uint8_t range = abs(max-min)+1;
	return ((rand() % range) + min);
}

uint8_t qsub8(uint8_t a, uint8_t b) {
    if (b > a) return 0;
    else return a - b;
}

uint8_t qadd8(uint8_t a, uint8_t b) {
    uint8_t sum = a + b;
    if (sum < a) return 255;  // Überlauf, gibt den maximalen Wert zurück
    else return sum;
}
