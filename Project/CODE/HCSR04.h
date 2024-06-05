#ifndef __HCSR04_H
#define __HCSR04_H

#define Echo_PIN 		P32		
#define Trig_PIN 		P33	
#define SOUND_SPEED  	3.46 // ÉùËÙ

extern volatile int time;

float sonar(void);
void HC_SR04_Init(void);

#endif