#ifndef _PRINTSYSTEM_H
#define _PRINTSYSTEM_H
#include "busSystem.h"

void printHeaderRow();

void printSensorInfo(ROM *rom, int x);

void printTemp(SCRATCH_PAD* scratchPointer, int sensorCount);

void printErrorSensor();

void printErrorCrc();

void clearScreen (int maxSensors, int sensorCount);


#endif
