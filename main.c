
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
	initTimer();
	GPIOD -> BSRR = (0x01 << 1);		// Pin 1 auf 3.3V
	openDrain();										// Pin 0 auf open Drain setzen
	pinOn();
	
	delay(20);
	
	while (true) {
		
		ROM sensor1;
		if (reset() != 0) {
			printf("Fehler: kein Sensor!");
		}
		writeComand(READ_ROM);
		readRom((uint8_t *)&sensor1);
		if (crcCheck((uint8_t *)&sensor1, 8)) {
			printRomInfo(&sensor1);
		} else {
			printf("Crc Fehler in Rom!");
		}
		delay(500);
		
		
		
		
	}
	
	
	
	
	
	
	
}

// EOF
