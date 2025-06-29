
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
#include "sensorSearchModule.h"



ROM romList[MAX_SENSORS];
SCRATCH_PAD scratchPadList[MAX_SENSORS];



#define TEILAUFGABE_3



int main(void){
	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);
	initTimer();
	GPIOD -> BSRR = (0x01 << 1);		// Pin 1 auf 3.3V
	openDrain();										// Pin 0 auf open Drain setzen
	pinOn();
	printHeaderRow();
	delay(20);
	
	#if defined TEILAUFGABE_2
	
	while (1) {
		
		
		ROM sensor1;
		if (reset() != 0) {
			printErrorSensor();
		}
		writeComand(READ_ROM);
		readRom((uint8_t *)&sensor1);
		if (crcCheck((uint8_t *)&sensor1, 8)) {
			printSensorInfo(&sensor1, 1);
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
		readScratchpad(&scratchPad1);
		if (crcCheck((uint8_t *)&scratchPad1, 9)) {
			printTemp(&scratchPad1, 1);
		} else {
			printErrorCrc();
		}
		
	}
	
	#elif defined TEILAUFGABE_3
		
		while (1) {
			int sensorCount = sensorSearch(romList, MAX_SENSORS);
			if (sensorCount == 0) {
				clearScreen (MAX_SENSORS,0);
				printErrorSensor();
				while (sensorSearch(romList, MAX_SENSORS) == 0) {}
				
			}	else if (sensorCount == -1) {
				clearScreen (MAX_SENSORS,0);
				printErrorCrc();
				while (sensorSearch(romList, MAX_SENSORS) == -1) {}
			} else {
				clearScreen (MAX_SENSORS,sensorCount);
				printSensorInfo(romList, sensorCount);
			}
			
			readMultiTemp(romList, sensorCount);
			readMultiScratchpad(scratchPadList, romList, sensorCount);
			printTemp(scratchPadList, sensorCount);
				
		}
		
		
		
		
		
		
		#endif
	
	
	
	
	
	
	
}

// EOF
