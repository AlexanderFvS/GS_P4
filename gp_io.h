#ifndef _GP_IO_H
#define _GP_IO_H

#define MODER_MASK_PIN_0 (0x03 << (2*0))
#define OUTPUT_MASK_PIN_0 (0x01 << (2*0))

#include "timeModul.h"


void pinOn();

void pinOff();

void setInput ();

void setOutput ();

void openDrain ();

int readBit ();

void readBytes(uint8_t *rom, int count);


#endif
