#ifndef _PRINTSYSTEM_H
#define _PRINTSYSTEM_H
#include "busSystem.h"

void printHeaderRow();

void printSensorInfo(ROM *rom);

void printTemp(SCRATCH_PAD *spad);

void printErrorSensor();

void printErrorCrc();


#endif
