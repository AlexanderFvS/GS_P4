

#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "printSystem.h"
 
#define DS18B20_FAMCODE 0x28
#define DS18S20_FAMCODE 0x10

uint64_t rom_uint64_old;
uint8_t code_old;
 
void printHeaderRow()
{
	lcdGotoXY(0, 0);
	lcdPrintS("Sensor  PDROM                   Temp. [C]");
}
 
void printSensorInfo(ROM *rom)
{
	uint8_t code_new = rom->familyCode;
	lcdGotoXY(0, 1);
	if (code_new != code_old) {
		if (code_new == DS18B20_FAMCODE)
		{
			lcdPrintS("DS18B20 ");
		}
		else if (code_new == DS18S20_FAMCODE)
		{
			lcdPrintS("DS18S20 ");
		}
		code_old = code_new;
	}
	
	
	lcdGotoXY(8, 1);
	uint64_t rom_uint64_new = 0;
  memcpy(&rom_uint64_new, rom, sizeof(ROM));
 
	if (rom_uint64_new != rom_uint64_old) {
		char tempString[19];
		sprintf(tempString, "0x%016llx", (unsigned long long) rom_uint64_new);
		lcdPrintReplS(tempString);
		rom_uint64_old = rom_uint64_new;
	}
}
 
void printTemp(SCRATCH_PAD *spad)
{
	uint8_t temp_LSB = spad->temperaturLSB;
	uint8_t temp_MSB = spad->temperaturMSB;
	int16_t raw_temp = (temp_MSB << 8) | temp_LSB;
	double tempCelsius = raw_temp * 0.0625;
	char tempString[10];
	sprintf(tempString, "%.4f", tempCelsius);
	lcdGotoXY(32, 1);
	lcdPrintReplS(tempString);
}
 
void printErrorSensor()
{
	lcdGotoXY(0, 1);
	lcdPrintReplS("Keine Sensoren vorhanden");
}

void printErrorCrc()
{
	lcdGotoXY(0, 1);
	lcdPrintReplS("Fehler in CRC Datei");
}
