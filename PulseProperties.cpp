#include "Pulse_Properties.h"
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 pulseSensor;

const byte RATE_SIZE = 6;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

float beatsPerMinute = 0;
int beatAvg = 0;
float lastValidBPM = 0;
bool fingerDetected = false;

unsigned long lastPrint = 0;

void pulseInitialize() {
  Serial.println("Initializing...");

  if (!pulseSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1)
      ;
  }

  Serial.println("Place your index finger on the sensor with steady pressure.");

  pulseSensor.setup();
  pulseSensor.setPulseAmplitudeRed(0x18);
  pulseSensor.setPulseWidth(69);
  pulseSensor.setSampleRate(100);
  pulseSensor.enableDIETEMPRDY();
  pulseSensor.clearFIFO();
}

long getIRValue() {
  return pulseSensor.getIR();
}

float getBPM() {
  return beatsPerMinute > 0 ? beatsPerMinute : lastValidBPM;
}

void updatePulse(long irValue) {
  if (irValue > 10000) {
    fingerDetected = true;

    if (checkForBeat(irValue)) {
      long delta = millis() - lastBeat;
      lastBeat = millis();
      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute >= 55 && beatsPerMinute <= 600) {
        lastValidBPM = beatsPerMinute;
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;

        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++) beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      } else {
        beatsPerMinute = 0;
        beatAvg = 0;
      }
    }
  } else {
    fingerDetected = false;
    beatsPerMinute = 0;
    lastValidBPM = 0;
    beatAvg = 0;
  }
}

void printOutput(long irValue) {
  if (millis() - lastPrint >= 200) {
    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(" , Avg BPM = ");
    Serial.print(beatAvg);
    Serial.print(" , BPM=");
    Serial.println(beatsPerMinute > 0 ? beatsPerMinute : lastValidBPM);
    lastPrint = millis();
  }
}
