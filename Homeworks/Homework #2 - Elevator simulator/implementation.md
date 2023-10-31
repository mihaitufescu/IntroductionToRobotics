# Homework #2 - Elevator simulator

## Requirement:
This  assignment  involves  simulating  a  3-floor  elevator  control  system  usingLEDs, buttons, and a buzzer with Arduino. Follow me in order to gain experience in using button state change, implementing debouncing techniques,and coordinating multiple components to represent real-world scenarios.

This implementation involves three steps. You can find the steps here:

## Step 1: Check each physical componenent's functionality

### Step 1.a: Place each LED and check its functionality
For this step you will need:
-4 LEDs
-4 resistors
-Jump wires
First we connect each lead to its pin (I am using pins 11,10,9 and 6) through a resistor, connect its ground and then we check if all the LEDs are functioning.

![Circuit implementation](/Screenshots%20and%20pictures/step1a.jpg)

Example code:
```
const int ledPins[] = {6, 9, 10, 11}; // Array to store the LED pin numbers

void setup() {
  // Set all LED pins as OUTPUT
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // Loop through the LEDs and turn them on one by one
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(1000);
    digitalWrite(ledPins[i], LOW);
  }
}
```

### Step 1.b: Place each button and check its functionality

For this step you will need:
-3 buttons
-Jump wires
First we connect each button to its pin (I am using pins 5,4,3), connect its ground and then we check if all the buttons are functioning using Serial.

![Circuit implementation](/Screenshots%20and%20pictures/step1b.jpg)

Example code:

```
const int buttonPins[] = {5, 4, 3}; // Array to store the button pin numbers

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Set button pins as INPUT_PULLUP
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Read the state of the buttons and report it using Serial
  for (int i = 0; i < 3; i++) {
    int state = digitalRead(buttonPins[i]);
    if (state == LOW) {
      Serial.print("Button ");
      Serial.print(i + 1);
      Serial.println(" is pressed.");
    }
  }
}
```

### Step 1.c: Test the buzzer

For this step you will need:
- A buzzer
- Jump wires
Now, we'll test the buzzer by connecting it to port 2 and verifying its functionality.

![Circuit implementation](/Screenshots%20and%20pictures/step1c.jpg)

Example code:

``` 
const int buzzerPin = 2; 

void setup() {
  // Set the buzzer pin as an OUTPUT
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Turn on the buzzer for a short duration
  digitalWrite(buzzerPin, HIGH);
  delay(1000); // Adjust the delay to change the buzzer's sound duration
  digitalWrite(buzzerPin, LOW);
  delay(1000); // Add a pause between buzzer activations
}

```

## Step 2: Identify main componenents of the code and initialize data

In order to have a clean and efficient code I suggest the following methods and techniques:

- Store physical components logic that have the same functionality in arrays
- Use debouncer for buttons ( to avoid unwanted triggers from your button)
- While still handling the logic of the elevator inside the loop, separate the functions of the elevator into methods:

    1. moveElevatorTo - this methods implements the movement between floors (it is bidirectional).
    2. callElevator - this methods implements the sound of calling the elevator and turns on the LED state.
    3. closeDoor - this methods implements the sound of closing the elevator door and shuts the LED state. 

## Step 3: Finalizing the code

This code effectively integrates button control with elevator operation, creating a functional elevator simulation. Users can initiate elevator calls and track its floor-to-floor movement using LEDs to represent floor levels. Auditory feedback is provided through a buzzer. Notably, the code is structured with separate methods, improving code readability and maintainability. It serves as a practical example of how to implement an interactive project with a well-organized code structure.

Code:

```
const int ledPins[] = {6, 9, 10, 11}; // array for the pins of the LEDs
const int buttonPins[] = {3, 4, 5};    // array for the pins of the buttons

volatile byte buttonStates[] = {0, 0, 0};     // array for the states of the buttons
volatile byte lastButtonStates[] = {0, 0, 0}; // array for the previous states of the buttons

const int buzzerPin = 2;
int currFloor = -1; // current floor (-1 means undefined)
int destFloor = -1; // destination floor (-1 means no destination)

const unsigned long debounceTime = 50;
unsigned long lastDebounceTime[] = {0, 0, 0};
const unsigned int delayMovement = 1000; // this sets the delay for the movement between two floors
const unsigned int doorCloseDuration = 300;
byte elevatorCalled = 0; // variable that is set to 1 when the elevator is called

void callElevator() {
  if (!elevatorCalled) {
    digitalWrite(ledPins[0], HIGH); // turning on the state LED
    tone(buzzerPin, 400, 500);     // sounding the buzzer at 400Hz for 500ms
    unsigned long callElevatorStartTime = millis();
    while (millis() - callElevatorStartTime < delayMovement) {
      // Wait for 1000ms (1 second)
    }
    elevatorCalled = 1;
  }
}
void closeDoor() {
  noTone(buzzerPin);              // stopping the buzzer
  tone(buzzerPin, 300, 300);      // closing the door sound
  delay(300);                     // delaying so the closing door sound is finished
  noTone(buzzerPin);              // stopping the sound
  destFloor = -1;
  elevatorCalled = 0;
  digitalWrite(ledPins[0], LOW);  // turning off the state LED to indicate that the door is closed
}

void moveElevatorTo(int floor) {
  int direction = (floor > currFloor) ? 1 : -1;
  unsigned long elevatorMoveStartTime = millis();
  while (currFloor != floor) {
    if (millis() - elevatorMoveStartTime >= delayMovement) {
      digitalWrite(ledPins[currFloor + 1], LOW); // Turn off the LED for the current floor
      currFloor += direction;
      digitalWrite(ledPins[currFloor + 1], HIGH); // Turn on the LED for the current floor
      elevatorMoveStartTime = millis();
    }
  }
  digitalWrite(ledPins[currFloor + 1], HIGH); // Turn on the LED for the new floor

  elevatorMoveStartTime = millis();
  while (millis() - elevatorMoveStartTime < delayMovement) {
    // Wait for 1 second to simulate elevator stopping
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {  //setting the leds pins
    pinMode(ledPins[i], OUTPUT);
  }

  for (int i = 0; i < 3; i++) {  //setting the buttons pins
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(buzzerPin, OUTPUT); //setting the buzzer pin
  pinMode(ledPins[0], OUTPUT); // state LED
  digitalWrite(ledPins[0], LOW); // initially, the elevator is not active so the state led is shut
}

void loop() {
  /// Buttons logic - this is where we handle the buttons
  for (int i = 0; i < 3; i++) {
    int reading = digitalRead(buttonPins[i]); //reading the input of each button

    // checking if the button state has changed
    if (reading != lastButtonStates[i]) {
      lastDebounceTime[i] = millis();
    }

    // checking if the button state has been stable for the debounce time
    if ((millis() - lastDebounceTime[i]) > debounceTime) {
      if (reading != buttonStates[i]) {
        buttonStates[i] = reading;

        // if the button is pressed and elevator is not already called, we call the elevator
        if (buttonStates[i] == LOW && !elevatorCalled) {
          callElevator();
          elevatorCalled = 1;
          destFloor = i;
        }
      }
    }

    lastButtonStates[i] = reading; //updating last state of the button
  }

  /// Elevator logic - this is where we handle the movement of the elevator
  if (destFloor != -1) {
    if (currFloor == -1) {
      // Start the elevator from the ground floor (state LED)
      digitalWrite(ledPins[0], HIGH); // Turn on the state LED
      moveElevatorTo(destFloor);
    } else if (currFloor != destFloor) {
      moveElevatorTo(destFloor);
    }
    else {
      // The elevator has reached the final destination
      closeDoor();
    }
  }
}
```

Watch the demo:
https://youtube.com/shorts/GVlHzoqc8wQ