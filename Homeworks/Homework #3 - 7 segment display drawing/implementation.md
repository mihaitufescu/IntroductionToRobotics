#Homework #3 - 7 segment display drawing

## Requirement:
You will use the joystick to control the position of
the segment and "draw" on the display. The movement between segments
should be natural, meaning they should jump from the current position
only to neighbors, but without passing through "walls".

This implementation involves three steps. You can find the steps here:

## Step 1: Test each physical componenent's functionality

### Step 1.a: Place the 7 segment
For this step you will need:
-7 segment display
-8 resistors
-Jump wires
First we connect each segment to its pin through a resistor and test its functionality.

![Circuit implementation](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%233%20-%207%20segment%20display%20drawing/Screenshots%20and%20pictures/circuit1.jpg)

Example code:
```
const int segSize = 8;  // Number of segments in the display

// Define pin numbers for segments
int segmentPins[] = {6, 9, 5, 10, 11, 4, 7, 8};
int segmentStates[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};  // Store segment states
int currentSegment = 1;  // Initialize the current segment counter

void setup() {
  // Initialize the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);  // Turn off all segments
  }
}
```

### Step 1.b: Place the joystick and check its functionality

For this step you will need:
-Joystick
-Jump wires
First we connect the joystick. Note that since I am using interrupters for the button  I must use either digital pin 2 or 3.

![Circuit implementation](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%233%20-%207%20segment%20display%20drawing/Screenshots%20and%20pictures/circuit2.jpg)

Example code:

```
//X-axis threshold and values
int xValue = 0;
int minThresholdX = 400;
int maxThresholdX = 600;

//Y-axis threshold and values
int yValue = 0;
int minThresholdY = 500;
int maxThresholdY = 600;

// Interrupt handler for button press
void buttonInterruptHandler() {
  buttonPressed = true;
}

void setup() {
  // Initialize the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);  // Turn off all segments
  }
  pinMode(pinSW, INPUT_PULLUP);  // Button with pull-up resistor
  attachInterrupt(digitalPinToInterrupt(pinSW), buttonInterruptHandler, FALLING); //Listening for any changes of the button  state
}
```

## Step 2: Identify main componenents of the code and initialize data

In order to have a clean and efficient code I suggest the following methods and techniques:

- Store physical components logic that have the same functionality in arrays (7 segments pins and states)
- Use debouncer for buttons and joystick triggers ( to avoid unwanted triggers from your button)
- Handling main functions into separate methods:

    1. buttonInterruptHandler() - Interrupt handler for button press.
    2. toggleSegment(bool right) - Function to toggle the active segment clockwise/counter clockwise.
    3. toggleYAxis(bool down) - Function to toggle the Y-axis segments.
    4. updateHighLeds() - Function to display the state of the segments turned on

## Step 3: Finalizing the code

The following code efficiently handles button clicks and debounce, ensuring that button presses are accurately detected. It distinguishes short clicks from long presses, resetting the display when a button is held for 3 seconds. The code separates the logic neatly, with clear segments for button handling and joystick input. It's efficient in managing LED states, enabling blinking animations at regular intervals. The joystick input determines segment changes, with smooth transitions. The code is well-structured, making it easy to understand and maintain, while effectively controlling an 8-segment display with a button and joystick for a satisfying user experience.

Code:

```
// Define the pin numbers
const int pinSW = 2;  // Button pin
const int pinX = A1;  // X-axis pin
const int pinY = A0;  // Y-axis pin

const int segSize = 8;  // Number of segments in the display

// Define pin numbers for segments
int segmentPins[] = {6, 9, 5, 10, 11, 4, 7, 8};
int segmentStates[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};  // Store segment states
int currentSegment = 1;  // Initialize the current segment counter

//X-axis threshold and values
int xValue = 0;
int minThresholdX = 400;
int maxThresholdX = 600;

//Y-axis threshold and values
int yValue = 0;
int minThresholdY = 500;
int maxThresholdY = 600;

//Variables needed for debouncing, blinking animation and defining the reset interval
unsigned long lastButtonChangeTime = 0;
unsigned long lastJoyChangeTime = 0;
const unsigned long debounceDelay = 400;
int longPressButton = 3000;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500;

//Processing events
bool buttonPressed = false;
bool ledActive = true;


// Interrupt handler for button press
void buttonInterruptHandler() {
  buttonPressed = true;
}

void setup() {
  // Initialize the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);  // Turn off all segments
  }
  pinMode(pinSW, INPUT_PULLUP);  // Button with pull-up resistor
  attachInterrupt(digitalPinToInterrupt(pinSW), buttonInterruptHandler, FALLING); //Listening for any changes of the button  state
}

void loop() {
  // Check for button press
  if (buttonPressed) {
    int swState = digitalRead(pinSW);
    if (swState == LOW && millis() - lastButtonChangeTime >= debounceDelay) {
      // Toggle the current segment state
      segmentStates[currentSegment] = (segmentStates[currentSegment] == HIGH) ? LOW : HIGH;
      lastButtonChangeTime = millis();
      buttonPressed = !buttonPressed;
      // Check if the button was held for 3 seconds
      unsigned long buttonPressStartTime = millis();
      while (digitalRead(pinSW) == LOW) {
        if (millis() - buttonPressStartTime >= longPressButton) {
          reset();  // Perform the reset
          break;
        }
      }
    }
  }

  // Read the joystick values (x and y axes)
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  // Check if the joystick is moved right (x-axis) for segment change
  if (xValue > maxThresholdX && (millis() - lastJoyChangeTime > debounceDelay)) {
    toggleSegment(true);
  }

  // Check if the joystick is moved left (x-axis) for segment change
  if (xValue < minThresholdX && (millis() - lastJoyChangeTime > debounceDelay)) {
    toggleSegment(false);
  }

  // Check if the joystick is moved down (y-axis) for segment change
  if (yValue > maxThresholdY && (millis() - lastJoyChangeTime > debounceDelay)) {
    toggleYAxis(true);
  }

  // Check if the joystick is moved up (y-axis) for segment change
  if (yValue < minThresholdY && (millis() - lastJoyChangeTime > debounceDelay)) {
    toggleYAxis(false);
  }

  // Check if it's time to blink the LED
  if (segmentStates[currentSegment] == LOW) {
    if (ledActive && millis() - lastBlinkTime >= blinkInterval) {
      // Toggle the LED state
      digitalWrite(segmentPins[currentSegment], !digitalRead(segmentPins[currentSegment]));
      lastBlinkTime = millis();
    }
  } else {
    digitalWrite(segmentPins[currentSegment], HIGH);
  }

  // Update the display to show the high segments
  updateHighLeds();
}

// Function to toggle the active segment clockwise/counter clockwise
void toggleSegment(bool right) {
  digitalWrite(segmentPins[currentSegment], LOW);  // Turn off the previously lit segment
  if (right) {
    currentSegment = (currentSegment + 1) % (segSize - 1);  // Increment the segment counter
  } else {
    currentSegment = (currentSegment - 1 + segSize - 1) % (segSize - 1);  // Decrement the segment counter
  }
  digitalWrite(segmentPins[currentSegment], HIGH);  // Turn on the new segment
  ledActive = true;
  lastJoyChangeTime = millis();  // Update the last change time
}

// Function to toggle the Y-axis segments
void toggleYAxis(bool down) {
  digitalWrite(segmentPins[currentSegment], LOW);  // Turn off the previously lit segment
  switch (currentSegment) {
    case 0:  // Segment A
      currentSegment = down ? 0 : 7;  // Switch from A to D or G
      break;
    case 4:  // Segment D
      currentSegment = down ? 7 : 4;  // Switch from D to G or A
      break;
    case 7:  // Segment G
      currentSegment = down ? 0 : 4;  // Switch from G to A or D
      break;
    default:
      currentSegment = (currentSegment + 1) % (segSize - 1);  // Increment other segments
      break;
  }
  digitalWrite(segmentPins[currentSegment], HIGH);  // Turn on the new segment
  ledActive = true;
  lastJoyChangeTime = millis();  // Update the last change time
}

// Function to display the state of the segments turned on
void updateHighLeds() {
  for (int i = 0; i < segSize; i++) {
    if (segmentStates[i] == HIGH) {
      digitalWrite(segmentPins[i], HIGH);
    }
  }
}

// Function to reset the display
void reset() {
  for (int i = 0; i < segSize; i++) {
    digitalWrite(segmentPins[i], LOW);  // Turn off all segments
    segmentStates[i] = LOW;  // Set the state of all segments to LOW
  }
  currentSegment = 1;  // Set the current segment to 1
}
```

Watch the demo:
https://www.youtube.com/shorts/P6K85QJXZc8
