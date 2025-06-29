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


// ROM befehle
#define READ_ROM 0x33
#define SKIP_ROM 0xCC
#define MATCH_ROM 0x55
#define SEARCH_ROM 0xF0
// Funktions Befehle
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE






void writeHigh ();

void writeLow ();

uint8_t reset ();

void writeComand (uint8_t comm);

void writeRom (ROM* rom);

int readBit ();

void readBytes(uint8_t *pointer, int count);

void readRom (uint8_t *rom);

void readScratchpad (SCRATCH_PAD *scratch);

void readMultiScratchpad (SCRATCH_PAD * scratchPadList, ROM *romList,  int sensorCount);

void readMultiTemp (ROM *romList,  int sensorCount);

bool crcCheck(uint8_t *data, uint8_t length);

#endif
