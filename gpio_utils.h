#ifndef GPIO_UTILS
#define GPIO_UTILS

#include <wiringPi.h>

#define PIN_LED_VERTE 0
#define PIN_LED_ROUGE 1
#define PIN_LED_JAUNE 21
#define PIN_LED_ORANGE 22

#define NB_LEDS_DISPONIBLES 4

#define PIN_LED_DEFAUT PIN_LED_VERTE

#define PIN_POUSSOIR 4

typedef char octet;

void allumerLED(octet LED);
void eteindreLED(octet LED);
void initGPIO();

#endif
