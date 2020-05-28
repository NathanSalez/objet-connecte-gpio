#include "gpio_utils.h"

void allumerLED(octet LED)
{
	digitalWrite(LED,HIGH);
}

void eteindreLED(octet LED)
{
	digitalWrite(LED,LOW);
}

void initGPIO()
{
	pinMode(PIN_LED_VERTE,OUTPUT);
	pinMode(PIN_LED_ROUGE,OUTPUT);
	pinMode(PIN_LED_JAUNE,OUTPUT);
	pinMode(PIN_LED_ORANGE,OUTPUT);
	pinMode(PIN_POUSSOIR,INPUT);
}
