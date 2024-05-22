#include "zf_tim.h"
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "ZF_PWM.h"
#include "zf_gpio.h"
#include <STC32Gxx.H>
#include "Buzzer.h"

uint16 x10_ms = 0;

void Buzzer_Init(void)
{
	pwm_init(PWMB_CH4_P77,2000,0);
}








