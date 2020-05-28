/**
 * Ce programme réalise les fonctionnalités suivantes :
 * - les 4 LEDS clignotent successivement  de façon  à faire NB_CYCLES cycles.
 * - Un cycle d'allume/extinction des LEDs correspond à :
 *     JAUNE => ORANGE => ROUGE => VERTE
 * - Après les cycles consommés, la LED dont le code numérique est donné en paramètre s'affiche
 */ 

#include "gpio_utils.h"
#include <stdlib.h>

struct
{
    octet contenu[NB_LEDS_DISPONIBLES];
    size_t longueur;
} cycle = { {PIN_LED_JAUNE, PIN_LED_ORANGE, PIN_LED_ROUGE, PIN_LED_VERTE},NB_LEDS_DISPONIBLES};

#define NB_CYCLES 8

#define NB_CLIGNOTEMENTS 5

int main (int argc, char **argv)
{
  octet LEDReponse = 0;
  wiringPiSetup();
  initGPIO();
  
  for (int  i =0; i < NB_CYCLES;i++)
  {
    for(int j=0; j< cycle.longueur; j++)
    {
      allumerLED( cycle.contenu[j] );
      delay(50);
      eteindreLED( cycle.contenu[j] );
    }
  }
  
  if( argc >= 2 )
    LEDReponse = atoi(argv[1]);
  
  for (int  i =0; i < NB_CLIGNOTEMENTS;i++)
  {
    allumerLED(LEDReponse);
    delay(500);
    eteindreLED(LEDReponse);
    delay(500);
  }
  
  return 0 ;
}

