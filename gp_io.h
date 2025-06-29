#ifndef _GP_IO_H
#define _GP_IO_H

#include "timeModul.h"


#define MODER_MASK_PIN_0 (0x03 << (2*0))
#define OUTPUT_MASK_PIN_0 (0x01 << (2*0))



void pinOn();

void pinOff();

void setInput ();

void setOutput ();

void openDrain ();

void pushPull ();




#endif
