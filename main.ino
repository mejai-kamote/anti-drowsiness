#include <BluetoothSerial.h>
#include <Pulse_Properties.h>
#include <AlertSystem.h>
#include <Orientation_Properties.h>

BluetoothSerial SerialBT;

// Global variables
long ir_value;
float heartBPM;
String orientationState;  // Made global for Bluetooth access
String alertState;

//millis variables for Bluetooth update interval
unsigned long previousBTMillis = 0;
const long btInterval = 500; // Send data every 500ms

void setup() {
  Serial.begin(115200);
  
  // Initialize Bluetooth
  SerialBT.begin("ESP32 BL - ADHD"); // Bluetooth device name
  
  // Initialize sensors
  outputBegin();
  pulseInitialize();
  gyroInitialize();

  xTaskCreatePinnedToCore(readPulse, "Pulse Reading", 8196, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(readGyro, "Gyro Reading", 8196, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(alertSystem, "Alert Trigger", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Handle Bluetooth data sending at regular intervals
  unsigned long currentMillis = millis();
  if (currentMillis - previousBTMillis >= btInterval) {
    previousBTMillis = currentMillis;
    
    // Send only state and BPM via Bluetooth
    // SerialBT.print("State:");
    SerialBT.print(alertState);
    SerialBT.print(",");
    SerialBT.print(orientationState);
    SerialBT.print(",");
    SerialBT.print((int)heartBPM);
    SerialBT.print(",");

  }
}

void readPulse(void* param) {
  while (1) {
    ir_value = getIRValue();
    updatePulse(ir_value);

    if (millis() - lastPrint >= 200) {
      heartBPM = getBPM();
      lastPrint = millis();
    }

    delay(20);
  }
}

void readGyro(void* param) {
  float accel_x, accel_y, accel_z;  // Local variables since we don't need them globally
  while (1) {
    readData(accel_x, accel_y, accel_z);
    delay(50);
  }
}

void alertSystem(void* param) {
  delay(1000); // wait for other tasks to initialize sensors

  float accel_x, accel_y, accel_z;  // Local variables
  
  while (1) {
    readData(accel_x, accel_y, accel_z);  // Read accelerometer data locally
    String newState = detectState(accel_x, accel_y, accel_z);
    updateState(newState);
    
    alertState = getAlert();
    orientationState = getCurrentState();  // Update global variable
    unsigned long stateDuration = getStateDuration();

    // Serial monitor output remains the same
    Serial.print(" | State: ");
    Serial.print(orientationState);
    // Serial.print(" | IR: ");
    // Serial.print(ir_value);
    // Serial.print(" | BPM: ");
    // Serial.print(heartBPM);
    // Serial.print(" | Duration: ");
    // Serial.println(stateDuration);

    updateAlert(orientationState, stateDuration, heartBPM);
    delay(50);
  }
}