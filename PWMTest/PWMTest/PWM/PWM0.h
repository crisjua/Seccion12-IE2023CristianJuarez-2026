/*
 * PWM0.h
 *
 * Created: 9/4/2026 15:47:18
 *  Author: crist
 */ 


#ifndef PWM0_H_
#define PWM0_H_

#define invert 1
#define no_invert 0

#define fastPWM 1
#define phasePWM 0



void initPWM0A(uint8_t invertido,uint8_t modo, uint8_t prescaler);
void initPWM0B(uint8_t invertido,uint8_t modo, uint8_t prescaler);
void updateDutyCycle0A(uint8_t ciclo);
void updateDutyCycle0B(uint8_t ciclo);



#endif /* PWM0_H_ */