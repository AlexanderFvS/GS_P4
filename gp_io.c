#include "stm32f4xx_hal.h"
#include "timeModul.h"
#include "gp_io.h"
#include <stdio.h>
#include "busSystem.h"


// delay-times





int readBit () {
	
	pinOff();
	timeDelay(6);
	pinOn();
	setInput();
	timeDelay(9);
	int aus = GPIOD -> IDR & OUTPUT_MASK_PIN_0;
	timeDelay(45);
	setOutput();
	
	
	return aus;
}

uint8_t readByte () {
	int aus = 0;
	for (int i = 0; i < 8; i++ ) {
		aus |= (readBit() << i);
		
	}
	printf("%d\n", aus);
	return aus;
}

void readBytes(uint8_t *rom, int count)
{
	for (int i = 0; i < count; i++ ) {
		rom[i] = readByte();
	}
}

void pushPull () {
	GPIOD -> OTYPER &= ~(1);
	
}

void openDrain () {
	GPIOD -> OTYPER |= (1 << 0);
}


void pinOn () {
	GPIOD -> BSRR = 1;
}

void pinOff () {
	GPIOD -> BSRR = (1<<16);
}

void setOutput () {
	GPIOD -> MODER = (GPIOD -> MODER & ~MODER_MASK_PIN_0) | OUTPUT_MASK_PIN_0;
}

void setInput () {
	GPIOD -> MODER = GPIOD -> MODER & ~MODER_MASK_PIN_0;
}
