
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"
#include "timer.h"
#include "gp_io.h"
#include "busSystem.h"
#include "printSystem.h"



// ROM befehle
#define READ_ROM 0x33
#define SKIP_ROM 0xCC
#define MATCH_ROM 0x55
#define SEARCH_ROM 0xF0
// Funktions Befehle
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE




int main(void){
	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);
	initTimer();
	GPIOD -> BSRR = (0x01 << 1);		// Pin 1 auf 3.3V
	openDrain();										// Pin 0 auf open Drain setzen
	pinOn();
	printHeaderRow();
	delay(20);
	
	
	
	while (true) {
		
		ROM sensor1;
		if (reset() != 0) {
			printErrorSensor();
		}
		writeComand(READ_ROM);
		readRom((uint8_t *)&sensor1);
		if (crcCheck((uint8_t *)&sensor1, 8)) {
			printSensorInfo(&sensor1);
		} else {
			printErrorCrc();
		}
		
		
		SCRATCH_PAD scratchPad1;
		
		reset();
		writeComand(SKIP_ROM);
		writeComand(CONVERT_T);
		pushPull();
		timeDelay(750000);
		openDrain();
		reset();
		writeComand(SKIP_ROM);
		writeComand(READ_SCRATCHPAD);
		readScratchpad((uint8_t *) &scratchPad1);
		printTemp(&scratchPad1);
		
		
		
		
		
		
		
		
	}
	
	
	
	
	
	
	
}

// EOF
