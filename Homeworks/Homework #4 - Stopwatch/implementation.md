#Homework #4 - Stopwatch

## Requirement:
Using the 4 digit 7 segment display and 3 buttons,
you should implement a stopwatch timer that counts in 10ths of a second
and has a save lap functionality (similar to most basic stopwatch functions
on most phones).

This implementation involves three steps. You can find the steps here:

## Step 1: Test each physical componenent's functionality

### Step 1.a: Place the IC Shift Register
For this step you will need:
-IC Shift Register
-Jump wires
Connect the shift register to the Arduino Board. https://www.etechnog.com/2021/10/ic-7495-pin-diagram-internal-circuit.html

![Circuit implementation](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%234%20-%20Stopwatch/Screenshots%20and%20pictures/step1.jpg){ width=50% }{ height=50% }

Example code:
```
// Declaring data for the shift register
const int latchPin = 11; 
const int clockPin = 10;  
const int dataPin = 12; 
```

### Step 1.b: Connect the 4 digit 7 segment display to the Arduino board

For this step you will need:
-4 digit 7 segment display
-Jump wires
Connect the 4 digit 7 segment display to the Arduino board. Connect the 4 digit 7 segment display to the Arduino board.

![Circuit implementation](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%234%20-%20Stopwatch/Screenshots%20and%20pictures/step2.jpg)

Example code:

```
// Declaring data for the 4 display 7 segments
int displayDigits[] = { 4, 5, 6, 7 };
const int displayCount = 4;  

```

### Step 1.c: Connect the segments to the Shift Register

In this step, segment pins are connected to the 74HC595 output pins.

![Circuit implementation](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%234%20-%20Stopwatch/Screenshots%20and%20pictures/step3.jpg)

### Step 1.c: Connect the buttons

![Circuit implementation](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%234%20-%20Stopwatch/Screenshots%20and%20pictures/step4.jpg)

Final setup:

```
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
![Circuit implementation](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%234%20-%20Stopwatch/Screenshots%20and%20pictures/step5.jpg)

## Step 2: Structure of the code and data initialization

This code is structured into several methods and data structures for avoiding code redudancy and performance:

- Store physical components logic that have the same functionality in the same data structure (Binary encodings for each digit, buttons and laps).
- Use debouncer for buttons.
- Data structures:
  1. Stack for the laps:

      The LapStack class is a circular stack implementation with a fixed size of 4. It maintains lap time data for a countdown timer. The class has methods to push a new lap time onto the stack, printing the lap times sequentially. When the stack is full, it shifts elements to make room for the new lap time. The printStack method displays the current lap time, and when the bottom of the stack is reached, it wraps around to the top, ensuring continuous display of the most recent lap times. This structure efficiently manages lap data, allowing for easy retrieval and display in a circular manner.
      
      Code:
      ```
      const int stackSize = 4;
      class LapStack {
        private:
          int stack[stackSize];
          int currElement;
        public:
          LapStack() : currElement(-1) {}

          void push(int value) {
            Serial.println("Push: ");
            Serial.println(value);
            if (currElement < stackSize - 1) {
              stack[++currElement] = value;
            } else {
              // Stack is full, shift elements to make room for the new one
              for (int i = 0; i < stackSize - 1; ++i) {
                stack[i] = stack[i + 1];
              }
              stack[stackSize - 1] = value;
            }
          }

          void printStack() {
            Serial.println("Stack: ");
            if (currElement >= 0) { // Printing on the display the current number if we havent reached the bottom
              number = stack[currElement];
              currElement--;
            } else { // Repositioning to the top if you have reached the bottom
              currElement = 3;
              printStack();
            }
          }
      };

      LapStack laps;
      ```
  2. Struct for the buttons:

      This is used to avoid data redundancy when implementing debounce.

      Code:
      ```
      struct buttonData {
        byte state;
        byte lastState = HIGH;
        byte active = LOW;
        long lastDebounceTime = 0;
      } startStopButton, resetButton, flagButton; 
      ```

- Handling main functionalities into separate methods:

    1. void buttonStartStopHandler() // Method to handle the start/stop button
    2. void buttonResetHandler() // Method to handle the start/stop button
    1. void buttonStartStopHandler() // Method to handle the flag button
    4. void initDisplay(int phase) // Method to initialize the display with a given phase
    5. void activateDisplay(int displayNumber) // Method to activate a specific display
    6. void writeReg(int digit) // Method to write a digit to the shift register
    7. void writeNumber(int number) // Method to write a number to the displays

## Step 3: Finalizing the code

This following code manages a countdown timer with 4-digit 7-segment displays and buttons control. It employs a debouncing mechanism for start/stop, reset, and lap buttons. The LapStack class maintains lap times in a circular stack, allowing for continuous storage of the latest lap times. The program also handles display initialization, activation, and writing numbers to the displays. The start/stop button toggles countdown activation, and the reset button clears the count unless the timer is running. The flag button records lap times. The code ensures stable button readings and seamless display of countdown and lap data.

Code:

```
// Declaring data for the shift register
const int latchPin = 11; 
const int clockPin = 10;  
const int dataPin = 12;   

// Declaring data for the 4 display 7 segments
int displayDigits[] = { 4, 5, 6, 7 };
const int displayCount = 4;  
const int encodingsNumber = 10;

// Binary encodings for each digit, first column for displays without a decimal point, second column for the display with a decimal point
byte byteEncodings[encodingsNumber][2] = {
  {B11111100, B11111101},  // 0
  {B01100000, B01100001},  // 1
  {B11011010, B11011011},  // 2
  {B11110010, B11110011},  // 3
  {B01100110, B01100111},  // 4
  {B10110110, B10110111},  // 5
  {B10111110, B10111111},  // 6
  {B11100000, B11100001},  // 7
  {B11111110, B11111111},  // 8
  {B11110110, B11110111}   // 9
};

// Declaring data for displaying the countdown
unsigned long lastIncrement = 0;
unsigned long delayCount = 100;  
unsigned long number = 0;       

// Declaring data for the buttons
int const buttonPins[] = {2, 3, 13};
int debounceDelay = 200; // Increase debounce delay for more stable button readings

// Formatting the button data
struct buttonData {
  byte state;
  byte lastState = HIGH;
  byte active = LOW;
  long lastDebounceTime = 0;
} startStopButton, resetButton, flagButton; 

// Formatting the laps format
const int stackSize = 4;
class LapStack {
  private:
    int stack[stackSize];
    int currElement;
  public:
    LapStack() : currElement(-1) {}

    void push(int value) {
      Serial.println("Push: ");
      Serial.println(value);
      if (currElement < stackSize - 1) {
        stack[++currElement] = value;
      } else {
        // Stack is full, shift elements to make room for the new one
        for (int i = 0; i < stackSize - 1; ++i) {
          stack[i] = stack[i + 1];
        }
        stack[stackSize - 1] = value;
      }
    }

    void printStack() {
      Serial.println("Stack: ");
      if (currElement >= 0) { // Printing on the display the current number if we havent reached the bottom
        number = stack[currElement];
        currElement--;
      } else { // Repositioning to the top if you have reached the bottom
        currElement = 3;
        printStack();
      }
    }
};

LapStack laps;

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

void loop() {
  // Handling button presses
  buttonStartStopHandler();
  buttonResetHandler();
  buttonFlagHandler();
  
  // Displaying the current number
  writeNumber(number);
}

// Method to handle the start/stop button
void buttonStartStopHandler() {
  startStopButton.state = digitalRead(buttonPins[0]);
  if (millis() - startStopButton.lastDebounceTime > debounceDelay && startStopButton.state == LOW) { //Debouncer for the button
    startStopButton.state != startStopButton.lastState ? (startStopButton.active = !startStopButton.active, startStopButton.lastDebounceTime = millis()) : 0; // If the buttons changed its state we toggle its state, othewise we keep the number at 0 (this technique makes sure you always stay at 000.0 until you press the start button for the first time)
  }
  startStopButton.lastState = startStopButton.state;
  (millis() - lastIncrement > delayCount && startStopButton.active == 1) ? (number++, number %= 10000, lastIncrement = millis()) : 0; // Increment the number while its active and make it reset when it reaches the display limit
}

// Method to handle the reset button
void buttonResetHandler(){
  resetButton.state = digitalRead(buttonPins[1]);
  if (millis() - resetButton.lastDebounceTime > debounceDelay && resetButton.state == LOW) { //Debouncer for the button
    startStopButton.active == LOW ? (number = 0, resetButton.active = !resetButton.active, Serial.println("Active")) : number = number; // Prevent from resetting while the countdown is on, otherwise reset it
    resetButton.lastDebounceTime = millis();
    resetButton.lastState = resetButton.state;
  }
}

// Method to handle the flag button
void buttonFlagHandler(){
  flagButton.state = digitalRead(buttonPins[2]);
  if (millis() - flagButton.lastDebounceTime > debounceDelay && flagButton.state == LOW) { //Debouncer for the button
    resetButton.active == LOW ? laps.push(number) : laps.printStack(); //If the reset is on print the laps, otherwise flag the laps
    flagButton.lastDebounceTime = millis();
    flagButton.lastState = flagButton.state;
  }
}

// Method to initialize the display with a given phase
void initDisplay(int phase){
  for (int i = phase; i >= 0; i--) {
    activateDisplay(i);
    writeReg(B11111100);
    delay(0);
    writeReg(B00000000); 
  }
}

// Method to activate a specific display
void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], i == displayNumber ? LOW : HIGH);
  }
}

// Method to write a digit to the shift register
void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

// Method to write a number to the displays
void writeNumber(int number) {
  if (number == 0) { //Handling the case when the number is 0
    for (int i = 0; i < displayCount; i++) {
      activateDisplay(i);
      writeReg(byteEncodings[0][i == 2 ? 1 : 0]);
      delay(0);
      writeReg(B00000000);
    }
  } else {
    int displayDigit = 3;
    while (number != 0) { //Write the number digit by digit in its designated display
      int lastDigit = number % 10;
      activateDisplay(displayDigit);
      writeReg(byteEncodings[lastDigit][displayDigit == 2 ? 1 : 0]);
      delay(0);
      writeReg(B00000000);
      displayDigit--; // once the current digit is printed on its display, we move to the next display
      number /= 10;
    }
    initDisplay(displayDigit); // this is used to activate displays unused by the current number
  }
}
```
Demo :
https://youtu.be/0sVqS57-NV4
