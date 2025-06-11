#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "timeModul.h"
#include "gp_io.h"
#include "busSystem.h"





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
    uint8_t crc = 0;

    for (uint8_t i = 0; i < length - 1; i++) { // letzte Byte = CRC selbst
        uint8_t byte = data[i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ byte) & 0x01;
            crc >>= 1;
            if (mix) {
                crc ^= 0x8C;
            }
            byte >>= 1;
        }
    }

    // true wenn CRC korrekt
    return (crc == data[length - 1]);
}
	
	
