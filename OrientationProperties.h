#ifndef ORIENTATION_PROPERTIES_H
#define ORIENTATION_PROPERTIES_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Extern variables
extern String currentState;
extern unsigned long stateStartTime;
extern Adafruit_MPU6050 mpu;  // Declare mpu object as extern

// Function declarations
void gyroInitialize();
void readData(float &x, float &y, float &z);
void updateState(String newState);
String detectState(float x, float y, float z);
String getCurrentState();
unsigned long getStateDuration();

bool isNormal(float x, float y, float z);
bool isForward(float x, float y, float z);
bool isBackward(float x, float y, float z);
bool isRight(float x, float y, float z);
bool isLeft(float x, float y, float z);

#endif
