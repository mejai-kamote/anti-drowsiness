// AlertSystem.cpp
#include <Arduino.h>
#include "AlertSystem.h"

const unsigned long WARNING_TIME = 2000;
const unsigned long ALERT_TIME = 3000;
const unsigned long DANGER_TIME = 5000;
String alarmState;

void outputBegin() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBMOTOR_PIN, OUTPUT);
}
String getAlert(){
  return alarmState;
}

void updateAlert(String gyroState, long duration, float bpm) {
  if (bpm <= 55 && bpm > 30) {
    activateAlert();
  } else {
    if (gyroState == "NORMAL") {
      stopAll();
      return;
    }

    if (duration >= DANGER_TIME) activateDanger();
    else if (duration >= ALERT_TIME) activateAlert();
    else if (duration >= WARNING_TIME) activateWarning();
    else stopAll();
  }
}

void activateWarning() {
  digitalWrite(VIBMOTOR_PIN, HIGH);
  noTone(BUZZER_PIN);
  alarmState = "WARNING!";
}

void activateAlert(){
   tone(BUZZER_PIN, 300);
   digitalWrite(VIBMOTOR_PIN, (millis() % 500 < 250) ? HIGH : LOW);
  alarmState = "ALERT!!";

} 

void activateDanger(){
   tone(BUZZER_PIN, 800);
   digitalWrite(VIBMOTOR_PIN, HIGH);
  alarmState = "DANGER!!";

} 

void stopAll() {
  digitalWrite(VIBMOTOR_PIN, LOW);
  noTone(BUZZER_PIN);
  alarmState = "NORMAL";

}
