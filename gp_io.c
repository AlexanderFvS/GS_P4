#include "stm32f4xx_hal.h"
#include "timeModul.h"
#include "gp_io.h"
#include <stdio.h>
#include "busSystem.h"







void pushPull () {																			//  Push-Pull bedeutet der Pin kann aktiv HIGH und LOW setzen (default = High -> Spannungsversorgung)
	GPIOD -> OTYPER &= ~(1);
	
}






void openDrain () {																			// Pin kann senden und Empfangen je nach konfiguration
	GPIOD -> OTYPER |= (1 << 0);
}






void pinOn () {																					// Pin wird auf 1 gesetzt
	GPIOD -> BSRR = 1;
}







void pinOff () {																				// Pin wird auf 0 gesetzt
	GPIOD -> BSRR = (1<<16);
}






void setOutput () {																														// OpenDrain --> Output
	GPIOD -> MODER = (GPIOD -> MODER & ~MODER_MASK_PIN_0) | OUTPUT_MASK_PIN_0;
}





	
void setInput () {																														// 	// OpenDrain --> Inputput
	GPIOD -> MODER = GPIOD -> MODER & ~MODER_MASK_PIN_0;
}

