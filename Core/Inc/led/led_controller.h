#ifndef INC_LED_CONTROLLER_H_
#define INC_LED_CONTROLLER_H_

#include "main.h"

typedef enum {
  LED_OFF,
  LED_ON,
  LED_BLINK,
  LED_SHORT_PULSE,
  LED_LONG_PULSE
} LED_Animation_t;

void LED_Controller_Init(void);
void LED_Controller_SetAnimation(LED_Animation_t animation);
void LED_Controller_Toggle(void);

/* convenience helpers */
void LED_Controller_NextAnimation(void);
void LED_Controller_PrevAnimation(void);

#endif
