// AlertSystem.h
#ifndef ALERTSYSTEM_H
#define ALERTSYSTEM_H

#define BUZZER_PIN 2
#define VIBMOTOR_PIN 4

void outputBegin();
void updateAlert(String gyroState, long duration, float bpm);
void activateWarning();
void stopAll();
void activateAlert();
void activateDanger();
String getAlert();

#endif
