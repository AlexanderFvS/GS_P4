#ifndef _BUSSYSTEM_H
#define _BUSSYSTEM_H

#include <stdbool.h>
#include "timer.h"

typedef struct __attribute__((packed)) ROM {
	uint8_t familyCode;
	uint8_t serialNumber[6];
	uint8_t crc;
	
} ROM;

typedef struct __attribute__((packed)) SCRATCH_PAD
{
	uint8_t temperaturLSB;
	uint8_t temperaturMSB;
	uint16_t user;
	uint16_t reserved;
	uint8_t countRemain;
	uint8_t countCelsius;
	uint8_t crc;
	
} SCRATCH_PAD;



void writeHigh ();

void writeLow ();

uint8_t reset ();

void writeComand (uint8_t comm);

void readRom (uint8_t *rom);

void readScratchpad (uint8_t *scratch);

bool crcCheck(uint8_t *data, uint8_t length);

#endif
