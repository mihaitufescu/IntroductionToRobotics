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
