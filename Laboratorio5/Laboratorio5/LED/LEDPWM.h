/*
 * LEDPWM.h
 *
 * Created: 20/4/2026 
 *  Author: crist
 */ 


#ifndef LED_PWM_H_
#define LED_PWM_H_

#include <stdint.h>

// Pin LED (igual que usabas antes)
#define LED_PIN PB0

extern volatile uint8_t pwmCounter;
extern volatile uint8_t pwmValue;

void initTimer0_LEDPWM(void);
void updateLEDBrightness(uint8_t brightness);

#endif