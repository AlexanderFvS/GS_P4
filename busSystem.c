#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "timeModul.h"
#include "gp_io.h"
#include "busSystem.h"


#define CRC_POLYNOM 0x8C



uint8_t reset () {
	uint8_t aus = 1;
	pinOff();
	timeDelay(480);
	pinOn();
	setInput();
	timeDelay(70);
	aus = GPIOD -> IDR & OUTPUT_MASK_PIN_0;
	timeDelay(410);
	setOutput();
	
	return aus;
}

void writeComand (uint8_t comm) {
	for (int i = 0; i < 8 ; i++) {
		if (comm & 0x01){ 
			writeHigh();
	} else {
		writeLow();
	}
	comm >>= 1;
	}
	
}


void writeLow () {
	pinOff();
	timeDelay(60);
	pinOn();
	timeDelay(10);
	
}


void writeHigh () {
	
	pinOff();
	timeDelay(6);
	pinOn();
	timeDelay(64);
	
}

void readRom (uint8_t *rom) {
	readBytes(rom, 8);
}

void readScratchpad (uint8_t *scratch) {
	readBytes(scratch, 9);
}



	bool crcCheck(uint8_t *data, uint8_t length) {
    uint8_t crc = 0;																		// crc initialisieren

    for (uint8_t i = 0; i < length - 1; i++) { 					// letzte Byte ausgenommen, da CRC selbst
        uint8_t byte = data[i];													// erstes Byte 
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ byte) & 0x01;					// vergleicht das niedrigste Bit von crc und byte. mix = 1 wenn ungleich
            crc >>= 1;																	// CRC um 1 Bit nach rechts schieben (wie Division durch 2)
            if (mix) {
                crc ^= CRC_POLYNOM;											// One-Wire CRC polynom überprüft crc an bestimmten stellen (8,7,3,2)
            }
            byte >>= 1;																	// nächstet bit 
        }
    }

    // true wenn CRC korrekt
    return (crc == data[length - 1]);										// Ist der berechnete CRC gleich dem CRC aus dem Protokoll -> return true
}
	
	
