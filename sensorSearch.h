#include "busSystem.h"
#include "busSystem.h"
#include "gp_io.h"

#define MAX_SENSORS 10



int SensorSearch (ROM* romList, int maxSensors);

int romSearch (ROM* rom);

int findDecision (int pos);

void setDecision (int pos, int value);

void readBitPair ();

