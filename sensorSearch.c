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

typedef struct Decision{
    int pos;      // Bitposition (0 bis 63)
    int value;    // Entscheidung: 0 oder 1
} Decision;

Decision decisionPath[MAX_SENSORS];
BIT_PAIR currPair;



int sensorCount = 0;
int decisionCount = -1; 


#define CONFLICT 		(currPair.bit1 == 0x00 && currPair.bit2 == 0x00)
#define ZERO 				(currPair.bit1 == 0x00 && currPair.bit2 == 0x01)
#define ONE 				(currPair.bit1 == 0x01 && currPair.bit2 == 0x00)
#define FAULT				(currPair.bit1 == 0x01 && currPair.bit2 == 0x01)








int SensorSearch (ROM* romList, int maxSensors) {
	
																															// Werte zurücksetzen

	sensorCount = 0;
	decisionCount = -1;
	
	memset(romList, 0, (sizeof(ROM) * maxSensors));
	memset(decisionPath, 0, sizeof(decisionPath));
	
	setDecision(-1, 0);
	
	
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
		
		if (decisionCount <= 0) {																		// nur abbrechen wenn keine weiteren Konflikte möglich sind
			break;
		}
	}
	
	return sensorCount;
}








int romSearch (ROM * rom) {
	uint64_t temp = 0;
	
	
	
	int currDecision = decisionCount;
	
	for (int bitPos = 0; bitPos < 64 ; bitPos++) {
		
		readBitPair();
		
		if (ZERO) {
			
			writeLow();
			
		} else if (ONE) {
			
			temp |= ((uint64_t)1 << bitPos);
			writeHigh();
			
		} else if (FAULT) {
			
			return -1;
			
			
		} else {
			
			if (bitPos < decisionPath[currDecision].pos) {
				
				if (findDecision(bitPos) == 0) {
					writeLow();
					
				} else if (findDecision(bitPos) == 1) {
					temp |= ((uint64_t)1 << bitPos);
					writeHigh();
					
				} else {
					setDecision(bitPos, 0);
					writeLow();
				}
			} else if (bitPos == decisionPath[currDecision].pos) {
				decisionPath[currDecision].value = 1;
				temp |= ((uint64_t)1 << bitPos);
				writeHigh();
			} else {
				if (findDecision(bitPos) == -1) {
					setDecision(bitPos, 0);
				}
				writeLow();
			}
			
		}
			
	}
	
	uint8_t *tempPtr = (uint8_t*)&temp;												// Kopiere temp in die ROM-Struktur 
	rom->familyCode = tempPtr[0];
	for (int i = 0; i < 6; i++) {
		rom->serialNumber[i] = tempPtr[i + 1];
	}
	rom->crc = tempPtr[7];	
	
	if (currDecision == decisionCount) {
		decisionCount--;
	}
	if (decisionPath[decisionCount].value == 1) {
			decisionCount--;
			
		}
	return 0;
}







int findDecision (int pos) {
	for (int i = 0; i <= decisionCount; i++) {
		if (decisionPath[i].pos == pos) {
			return decisionPath[i].value;
		}
	}

	return -1;
}







void setDecision (int pos, int value) {
	
	decisionCount++;
	decisionPath[decisionCount].pos = pos;
	decisionPath[decisionCount].value = value;
	
}






void readBitPair () {
	
	currPair.bit1 = readBit();
	currPair.bit2 = readBit();

}
