

#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "printSystem.h"
#include "sensorSearchModule.h"

#define DS18B20_FAMCODE 0x28
#define DS18S20_FAMCODE 0x10


 
void printHeaderRow()
{
	lcdGotoXY(0, 0);
	lcdPrintS("Sensor  PDROM                   Temp. [C]");
}






void printSensorInfo(ROM *rom, int sensorCount)
{
	
	for (int i = 0; i < sensorCount; i++) {
		ROM curr = rom[i];
		
		uint8_t code_new = curr.familyCode;
		lcdGotoXY(0, (i +1));
		lcdPrintReplS(" ");
		lcdGotoXY(0, (i +1));
		
		if (code_new == DS18B20_FAMCODE)
		{
			lcdPrintReplS("DS18B20 ");
		}
		else if (code_new == DS18S20_FAMCODE)
		{
			lcdPrintReplS("DS18S20 ");
		}
		
		
		lcdGotoXY(8,(i +1));
		uint64_t rom_uint64_new = 0;
		memcpy(&rom_uint64_new, &curr, sizeof(ROM));
	 
		char tempString[19];
		sprintf(tempString, "0x%016llx", (unsigned long long) rom_uint64_new);
		lcdPrintReplS(tempString);
		
	}
}
 





void printTemp(SCRATCH_PAD* scratchPointer, int sensorCount)
{
	for (int i = 0; i < sensorCount; i++) {
		
		uint8_t temp_LSB = scratchPointer[i].temperaturLSB;
		uint8_t temp_MSB = scratchPointer[i].temperaturMSB;
		int16_t raw_temp = (temp_MSB << 8) | temp_LSB;
		
		double tempCelsius = raw_temp * 0.0625;
		char tempString[10];
		
		sprintf(tempString, "%.4f", tempCelsius);
		lcdGotoXY(32, i + 1);
		lcdPrintReplS(tempString);
	}
}
 





void printErrorSensor()
{
	lcdGotoXY(0, 1);
	lcdPrintReplS("Keine Sensoren gefunden!                 ");
}






void printErrorCrc()
{
	lcdGotoXY(0, 1);
	lcdPrintReplS("Fehler in der CRC Datei!                 ");
}






void clearScreen (int maxSensors, int sensorCount) {
	for (int i = sensorCount; i < maxSensors; i++) {
		lcdGotoXY(0,i + 1);
		lcdPrintS(" ");
	}
}

