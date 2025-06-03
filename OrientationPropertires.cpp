#include "Orientation_Properties.h"

Adafruit_MPU6050 mpu; 

// Define global state variables
String currentState = "NORMAL";
unsigned long stateStartTime = 0;

void updateState(String newState) {
    if (newState != currentState) {
        currentState = newState;
        stateStartTime = millis();
    }
}

String detectState(float x, float y, float z) {
    if (isNormal(x, y, z)) return "NORMAL";
    if (isForward(x, y, z)) return "FORWARD";
    if (isBackward(x, y, z)) return "BACKWARD";
    if (isRight(x, y, z)) return "RIGHT";
    if (isLeft(x, y, z)) return "LEFT";
    return currentState;
}

String getCurrentState() {
    return currentState;
}

void gyroInitialize   () {

  if (!mpu.begin()) { 
      Serial.println("MPU6050 not Connected. Check your connection");
      while(1);
  }

   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
}

void readData(float &x, float &y, float &z) {
   sensors_event_t accel, gyro, temp;
   mpu.getEvent(&accel, &gyro, &temp);

   x = accel.acceleration.x;
   y = accel.acceleration.y;
   z = accel.acceleration.z;

   // Print the accelerometer values
   Serial.print("X: ");
   Serial.print(x);
   Serial.print(" | Y: ");
   Serial.print(y);
   Serial.print(" | Z: ");
   Serial.println(z);
}

unsigned long getStateDuration() {
    return millis() - stateStartTime;
}

bool isNormal(float x, float y, float z) {
    return (x >=5 && x < 11) && (y >= -5 && y < 0) && (z >= -6 && z < 0);
}

bool isForward(float x, float y, float z) {
    return (x >= -0 && x < 5) && (y >= -11 && y < -5) && (z >= -76 && z < -2);
}

bool isBackward(float x, float y, float z) {
    return (x >= 6 && x < 10) && (y >= -1 && y < 7) && (z >= -7 && z < -2);
}

bool isLeft(float x, float y, float z) {
    return (x >= 6 && x < 8) && (y >= -6 && y < 1) && (z >= -11 && z < -5);
}

bool isRight(float x, float y, float z) {
    return (x >= 5 && x < 9) && (y >= -5 && y < -2) && (z >= -7 && z < -3);
}
