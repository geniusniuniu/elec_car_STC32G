#ifndef __ADC_H
#define __ADC_H

#define EDGE_PROTECT 	12

extern float ADC_proc[];

void ADC_InitAll(void);
void ADC_GetValue(void);

#endif
