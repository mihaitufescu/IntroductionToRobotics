# Homework #5 - Smart System Environment

## Requirement:
Develop a ”Smart Environment Monitor and Logger” using Arduino.  This sys-tem will utilize various sensors to gather environmental data, log this data intoEEPROM, and provide both visual feedback via an RGB LED and user interac-tion through a Serial Menu.  The project focuses on integrating sensor readings,memory management, Serial Communication and the general objective of build-ing a menu.

This implementation involves two steps. You can find the steps here:

## Step 1: Test each physical componenent's functionality
For this step you will need:
-Arduino Uno Board
-Ultrasonic Sensor (HC-SR04)
-LDR (Light-Dependent Resistor) aka Photocell aka Photoresistor aka LightSensor
-RGB LED
-Resistors as needed•Breadboard and connecting wires

<img src="https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%235%20-%20Smart%20Environment%20System/Screenshots%20and%20pictures/circuit.jpg" alt="Circuit implementation" width="50%" height="50%" />

Example code:

```
// Declare pins
const int ledPins[] = {6, 5, 7}; // green, blue, red
const int trigPin = 9;
const int echoPin = 10;
int ldrPin = A0;

// Declare data
int sampleRate = 0;
int ultraSonicThreshold = 0;
int ldrThreshold = 0;

void setup() {
  // Setting up pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  // Setting up the displays
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  // Setting up the buttons
  for(int i = 0; i < 3; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  Serial.begin(9600);
}
```
## Step 2: Structure of the code and data initialization

This code is structured into several methods and data structures for avoiding code redudancy and performance:

- Sets up the initial configuration for the Arduino. Configures pin modes for the ultrasonic sensor (trigPin and echoPin), LEDs, and initializes serial communication. Calls the menu() function to display the main menu.
- Configures pin modes for the ultrasonic sensor (trigPin and echoPin), LEDs, and initializes serial communication.
- The code implements a menu-based user interface through the Serial Monitor, allowing users to configure sensor settings, reset data, check system status, and control the RGB LED.
- Handling main functionalities into separate methods:

    1. void menu() // Displays the main menu options on the Serial Monitor.
    2. void checkSensors() // Checks the ultrasonic and LDR sensor values against predefined thresholds; if conditions are met, it logs the data to EEPROM and updates the LED states accordingly.
    1. void clear() // Clears the serial input buffer.
    4. void sonicReading() // Triggers the ultrasonic sensor to measure distance; Calculates the distance based on the duration of the pulse received from the echo pin.
    5. void ldrReading() // Reads the analog value from the LDR sensor.
    6. void clearData() // Clears stored data in EEPROM for both ultrasonic and LDR readings.
    7. void displaySets() // Displays the last 10 stored values in EEPROM for both ultrasonic and LDR readings.

## Step 3: Finalizing the code

The Arduino code establishes a menu-driven system via Serial Monitor. It enables users to configure ultrasonic and LDR sensor thresholds, control RGB LEDs, and view system status. The system logs sensor data in EEPROM, triggering LED alerts based on threshold conditions. Functions handle sensor readings, EEPROM data clearing, and data display. The program continuously loops, responding to user inputs and updating sensor states. Overall, it integrates sensors, user interface, and LED control, providing a versatile monitoring and control system for environmental conditions.

Code:

```
#include <EEPROM.h>

const int SONIC_MEM = 0;
const int LDR_MEM = 40;

int incomingByte = 0;
int ok = 0;
int sampleRate = 0;
int ultraSonicThreshold = 0;
int ldrThreshold = 0;
long lastSamplingTime = 0;

const int ledPins[] = {6, 5, 7}; // green, blue, red
const int trigPin = 9;
const int echoPin = 10;
long duration = 0;
int ultraSonicDistance = 0;

int ldrPin = A0;
int ldrValue;

int ultraSonicDistanceIndex = 0;
int ldrIndex = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600);
  menu();
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read() - 48;
    switch (ok) {
      case 0: {
        switch (incomingByte) {
          case 1: {
            Serial.println("1.1.Sensors Sampling Interval");
            Serial.println("1.2.Ultrasonic Alert Threshold");
            Serial.println("1.3.LDR Alert Threshold");
            Serial.println("1.4.Back");
            Serial.println();
            ok = 1;
            break;
          }
          case 2: {
            Serial.println("2.1.Yes");
            Serial.println("2.2.No");
            Serial.println();
            ok = 2;
            break;
          }
          case 3: {
            Serial.println("3.1.Current Sensor Readings");
            Serial.println("3.2.Current Sensor Settings");
            Serial.println("3.3.Display Logged Data");
            Serial.println("3.4.Back");
            Serial.println();
            ok = 3;
            break;
          }
          case 4: {
            Serial.println("4.1.Manual Color Control");
            Serial.println("4.2.LED: Toggle Automatic ON/OFF");
            Serial.println("4.3.Back");
            Serial.println();
            ok = 4;
            break;
          }
        }
        break;
      }

      case 1: {
        switch (incomingByte) {
          case 1: {
            Serial.println("Enter sampling interval: (between 1 to 10");
            clear();
            while (!Serial.available()) {}
            sampleRate = Serial.read() - 48;
            Serial.print("Sampling interval is: ");
            Serial.println(sampleRate);
            Serial.println();
            break;
          }
          case 2: {
            Serial.println("Ultrasonic Threshold");
            clear();
            while (!Serial.available()) {}
            ultraSonicThreshold = Serial.parseInt();
            Serial.print("Ultrasonic min. value is: ");
            Serial.println(ultraSonicThreshold);
            Serial.println();
            break;
          }
          case 3: {
            Serial.println("LDR Threshold");
            clear();
            while (!Serial.available()) {}
            ldrThreshold = Serial.parseInt();
            Serial.print("LDR min. value is: ");
            Serial.println(ldrThreshold);
            Serial.println();
            break;
          }
          case 4: {
            ok = 0;
            menu();
            break;
          }
        }
        break;
      }

      case 2: {
        switch (incomingByte) {
          case 1: {
            clearData();
            ok = 0;
            menu();
            break;
          }
          case 2: {
            ok = 0;
            menu();
            break;
          }
        }
        break;
      }

      case 3: {
        switch (incomingByte) {
          case 1: {
            Serial.println("For exit press 0");
            clear();
            while (Serial.available() == 0)
              if ((millis() - lastSamplingTime) >= sampleRate * 1000) {
                sonicReading();
                ldrReading();
                Serial.print("Current distance: ");
                Serial.println(ultraSonicDistance);
                Serial.print("Current lighting: ");
                Serial.println(ldrValue);
                lastSamplingTime = millis();
                Serial.println();
              }
            int exit = Serial.read() - 48;
            if (exit == 0) {
              Serial.println("3.1 Current Sensor Readings");
              Serial.println("3.2 Current Sensor Settings");
              Serial.println("3.3 Display Logged Data");
              Serial.println("3.4 Back");
              Serial.println();
              ok = 3;
            }
            break;
          }
          case 2: {
            Serial.print("Sampling Interval: ");
            Serial.println(sampleRate);
            Serial.print("Ultrasonic Threshold: ");
            Serial.println(ultraSonicThreshold);
            Serial.print("LDR Threshold: ");
            Serial.println(ldrThreshold);
            Serial.println();
            break;
          }
          case 3: {
            displaySets();
            Serial.println();
            break;
          }
          case 4: {
            ok = 0;
            menu();
            break;
          }
        }
        break;
      }

      case 4: {
        switch (incomingByte) {
          case 1: {
            Serial.println("Submenu 4.1 to be continued");
            Serial.println();
            break;
          }
          case 2: {
            Serial.println("Submenu 4.2 to be continued");
            Serial.println();
            break;
          }
          case 3: {
            ok = 0;
            menu();
            break;
          }
        }
        break;
      }
    }
  }
  sonicReading();
  ldrReading();
  checkSensors();
}

void menu() {
  Serial.println("Menu:");
  Serial.println("1.Sensor Settings");
  Serial.println("2.Reset Logger Data");
  Serial.println("3.System Status");
  Serial.println("4.RGB LED Control");
  Serial.println();
}
void checkSensors(){
  digitalWrite(ledPins[0], HIGH);
  if ((millis() - lastSamplingTime) >= sampleRate * 1000) {
    if (ultraSonicDistance < ultraSonicThreshold) {
      digitalWrite(ledPins[2], HIGH); // red
      digitalWrite(ledPins[0], LOW);  // green
      Serial.println("Object detected!");
      Serial.println();
      EEPROM.put(SONIC_MEM + ultraSonicDistanceIndex * sizeof(int), ultraSonicDistance);
      ultraSonicDistanceIndex = (ultraSonicDistanceIndex + 1) % 10;
    } else {
      digitalWrite(ledPins[2], LOW); // red
    }

    if (ldrValue < ldrThreshold) {
      digitalWrite(ledPins[1], HIGH); // blue
      digitalWrite(ledPins[0], LOW); // green
      Serial.println("Night is coming!");
      Serial.println();
      EEPROM.put(LDR_MEM + ldrIndex * sizeof(int), ldrValue);
      ldrIndex = (ldrIndex + 1) % 10;
    } else {
      digitalWrite(ledPins[1], LOW); // blue
    }

    lastSamplingTime = millis();
  }
}
void clear() {
  char chr[1];
  delay(100);
  while (Serial.available() != 0) {
    Serial.readBytes(chr, 1);
  }
  delay(1000);
}

void sonicReading() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  ultraSonicDistance = duration * 0.034 / 2;
}

void ldrReading() {
  ldrValue = analogRead(ldrPin);
}

void clearData() {
  // Clear EEPROM data
  for (int i = 0; i < 10; i++) {
    EEPROM.put(SONIC_MEM + i * sizeof(int), 0);
    EEPROM.put(LDR_MEM + i * sizeof(int), 0);
  }
}

void displaySets() {
  Serial.println("Last 10 Values:");
  Serial.println();
  for (int i = 0; i < 10; i++) {
    Serial.print("Distance: ");
    Serial.println(EEPROM.read(SONIC_MEM + sizeof(int) * i));
    Serial.print("LDR: ");
    Serial.println(EEPROM.read(LDR_MEM + sizeof(int) * i));
    Serial.println();
  }
}
```

Demo :
https://youtu.be/1cJqXVCWxZY
