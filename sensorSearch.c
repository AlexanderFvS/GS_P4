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
    int pos;     																						 // Bitposition (0 bis 63)
    int value;	  																					 // Entscheidung: 0 oder 1
} Decision;

BIT_PAIR currPair;
Decision decisionPath[MAX_SENSORS];													// Speicher für Abzweigungen die wir genommen haben 					

int sensorCount = 0;
int decisionCount = -1; 																		// Zähler für Abzweigungen (Stack höhe)


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
	
	setDecision(-1, 0);																					// Setzt basis Decision um Nullpointer abzufangen --> LastDiscrepancy = -1
	
	
	while (sensorCount < maxSensors) {
		
		
		
		if (reset() != 0) {																				// Bus Reset
			return 0;
		}
	
		writeComand(SEARCH_ROM);																	// Befehl für Sensoren 
		
		if (romSearch(&romList[sensorCount]) != 0) {							// Einlesen von Sensor
      return -1;
		}
		
		if (!crcCheck((uint8_t *)&romList[sensorCount], 8)) {
			return -1;
    }
		
		sensorCount++;																							// Zähle gefundene Geräte
		
		if (decisionCount <= 0) {																		// nur abbrechen wenn es keine weiteren Konflikte gibt
			break;
		}
	}
	
	return sensorCount;
}








int romSearch (ROM * rom) {
	uint64_t temp = 0;
	
	int currDecision = decisionCount;																// aktuell zu behandelnder Konflikt
	
	for (int bitPos = 0; bitPos < 64 ; bitPos++) {
		
		readBitPair();																								// Einlesen
		
		if (ZERO) {																										// bei 0 gehe 0
			
			writeLow();
			
		} else if (ONE) {																							// bei 1 gehe 1
			
			temp |= ((uint64_t)1 << bitPos);														// und schreib 1
			writeHigh();
			
		} else if (FAULT) {																						// bei Fehler -> kein Sensor antwortet 
			
			return -1;
			
			
		} else {
			
			if (bitPos < decisionPath[currDecision].pos) {							// Wenn der Konflikt vor dem aktuellen liegt
				
				if (findDecision(bitPos) == 0) {													// gehen wie wir zuvor gegangen sind
					writeLow();
					
				} else if (findDecision(bitPos) == 1) {										
					temp |= ((uint64_t)1 << bitPos);
					writeHigh();
					
				} else {																									// Ein weiterer Sensor wurde währenddessen hinzugefügt
					setDecision(bitPos, 0);
					writeLow();
				}
			} else if (bitPos == decisionPath[currDecision].pos) {			// Wenn es der aktuelle Konflikt ist gehen wir jetzt mit 1 weiter
				decisionPath[currDecision].value = 1;
				temp |= ((uint64_t)1 << bitPos);
				writeHigh();
			} else {																										// weitere Konflikte sind nach dem Aktuellen aufgetreten			
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
	
	
																														
	if (currDecision == decisionCount) {											// Wenn kein weiterer Konflikt aufgetreten ist -> Stack dekrementieren 
		decisionCount--;
	}
	while (decisionPath[decisionCount].value == 1) {					// Sollte der vorherige Konflikt auch schon abgearbeitet sein -> Stack dekrementieren
			decisionCount--;
		}
	return 0;
}







int findDecision (int pos) {																// Returnt die gewählte Entschiedung 0 oder 1 
	for (int i = 0; i <= decisionCount; i++) {
		if (decisionPath[i].pos == pos) {
			return decisionPath[i].value;
		}
	}

	return -1;
}







void setDecision (int pos, int value) {											// Packt eine neue Decision auf den Stack
	
	decisionCount++;
	decisionPath[decisionCount].pos = pos;
	decisionPath[decisionCount].value = value;
	
}






void readBitPair () {																				// liest Bitpaar ein
	
	currPair.bit1 = readBit();
	currPair.bit2 = readBit();

}
