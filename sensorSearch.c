#include <string.h>
#include "busSystem.h"
#include "busSystem.h"
#include "gp_io.h"
#include "sensorSearch.h"
#include "printSystem.h"




typedef struct BIT_PAIR {
	int bit1;
	int bit2;
} BIT_PAIR;


BIT_PAIR currPair;
int currConflict = -1;
int lastConflict = -1;
int sensorCount = 0;

#define CONFLICT 		(currPair.bit1 == 0x00 && currPair.bit2 == 0x00)
#define ZERO 				(currPair.bit1 == 0x00 && currPair.bit2 == 0x01)
#define ONE 				(currPair.bit1 == 0x01 && currPair.bit2 == 0x00)
#define FAULT				(currPair.bit1 == 0x01 && currPair.bit2 == 0x01)








int SensorSearch (ROM* romList, int maxSensors) {
	
	
	
	currConflict = -1;																			// Werte zurücksetzen
	lastConflict = -1;
	sensorCount = 0;
	memset(romList, 0, (sizeof(ROM) * maxSensors));
	
	
	
	
	while (sensorCount < maxSensors) {
		if (reset() != 0) {
			return 0;
		}
	
		
		writeComand(SEARCH_ROM);
		if (romSearch(&romList[sensorCount]) != 0) {
      return -1;
		}
		
		if (!crcCheck((uint8_t *)&romList[sensorCount], 8)) {
			return -1;
    }
		
		sensorCount++;																							// Zähle gefundene Geräte
		
		if (currConflict == -1) {																		// nur abbrechen wenn keine weiteren Konflikte möglich sind
			break;
		}
		lastConflict = currConflict;
	}
	
	return sensorCount;
}








int romSearch (ROM * rom) {
	uint64_t temp = 0;
	
	for (int bitPos = 0; bitPos < 64 ; bitPos++) {
		
		readBitPair();
		
		if (ZERO) {
			
			writeLow();
			
		} else if (ONE) {
			temp |= ((uint64_t)1 << bitPos);
			writeHigh();
			
		} else if (CONFLICT) {
			
			if (bitPos == lastConflict) {															// wenn bereits 0 gegeangen geh 1 
				lastConflict = currConflict;
				currConflict = -1;
				temp |= ((uint64_t)1 << bitPos);
				writeHigh();
				
				
			} else if (currConflict == -1 && (lastConflict == -1 || bitPos > lastConflict)) {
				currConflict = bitPos;
				writeLow();
				
			} else {
				writeLow();
			}
			
		} else if (FAULT) {
			
			return -1;
		}
	}
	
	uint8_t *tempPtr = (uint8_t*)&temp;												// Kopiere temp in die ROM-Struktur 
	rom->familyCode = tempPtr[0];
	for (int i = 0; i < 6; i++) {
		rom->serialNumber[i] = tempPtr[i + 1];
	}
	rom->crc = tempPtr[7];	
	
	return 0;
}








void readBitPair () {
	
	currPair.bit1 = readBit();
	currPair.bit2 = readBit();

}
