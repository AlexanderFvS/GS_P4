#include "timer.h"



void timeDelay (uint32_t delay) {
	
	uint32_t timestamp1 = TIM2->CNT;
	delay = delay * 90;
	
	while ((TIM2->CNT - timestamp1) <= delay) {}
}
