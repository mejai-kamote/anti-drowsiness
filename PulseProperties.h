#ifndef PULSE_PROPERTIES_H
#define PULSE_PROPERTIES_H

#include <Arduino.h>

extern const byte RATE_SIZE;
extern byte rates[];
extern byte rateSpot;
extern long lastBeat;

extern float beatsPerMinute;
extern int beatAvg;
extern float lastValidBPM;
extern bool fingerDetected;

extern unsigned long lastPrint;

void pulseInitialize();
long getIRValue();
float getBPM();
void updatePulse(long irValue);
void printOutput(long irValue);

#endif
