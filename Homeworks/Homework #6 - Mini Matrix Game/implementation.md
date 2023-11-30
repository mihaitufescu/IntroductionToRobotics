# Homework #6 - Mini Matrix Game

## Requirement:
Develop a small game on the 8x8 matrix. The game must have at least 3 types
of elements: player (blinks slowly), bombs/bullets (blinks fast), wall (doesn’t
blink). The game chosen in this case is Bomberman.

This implementation involves three steps. You can find the steps here:

## Step 1: Test each physical componenent's functionality
For this step you will need:
-Arduino Uno Board
-8x8 Matrix
-MAX7219 Driver
-Resistors as needed
-Breadboard
-Jump wires

<img src="https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%236%20-%20Mini%20Matrix%20Game/Screenshots%20and%20pictures/circuit.jpg" alt="Circuit implementation" width="50%" height="50%" />

Example code:

```
// Constants
const int dPin = 12;
const int cPin = 11;
const int lPin = 10;
const byte matSize = 8;

// Controller
const int xPin = A0;
const int yPin = A1;
const int pinSW = A2;

// LedControl instance
LedControl lc = LedControl(dPin, cPin, lPin, 1);

// Game settings
byte gameMapBrightness = 2;
byte gameMap[8][8];

// Player position
byte playerPosX = 0;
byte playerPosY = 0;
byte lastPlayerPosX = 0;
byte lastPlayerPosY = 0;

//Joystick variables used to set its sensitivity
const int minThreshold = 200;
const int maxThreshold = 600;
const byte moveInterval = 100;
unsigned long long lastMoved = 0;
bool mapHasChanged = true;

// Button and Bomb state
boolean buttonState = false;
boolean bombIsPlaced = false;
int bombPosX = -1;
int bombPosY = -1;
const int explosionTime = 2000;
int lastPositionSetTime;
```
## Step 2: Structure of the code and data initialization

This code is structured into several methods and data structures for avoiding code redudancy and performance:

- Sets up the initial configuration for the Arduino. Configures pin modes for the matrix, integrated circuit and joystick.
- Sets variables that define matrix size and data allocation, joystick position, and game related variables used for handling events.
- The code implements a Bomberman type game, generates a random map, handles player movement, handles the interaction between the player and the walls and handles the bomb logic and the case where the player dies.
- Handling main functionalities into separate methods:

    1. generateRandomgameMap: Initializes the gameMap array with a guaranteed path from the top-left to the bottom-right and generates random walls with a guaranteed path.
    2. setgameMap: Sets up the initial LED matrix display, initializes the LED control, sets the player's initial position on the game map, generates a random game map, and updates the LED matrix.
    3. resetgameMap: Resets the game map to its initial state or generates a new random game map, providing a delay to display the reset state before continuing the game.
    4. mapUpdate: Updates the LED matrix display based on the gameMap array.the duration of the pulse received from the echo pin.
    5. playerPosUpdate: Updates the player's position based on analog joystick readings, handles player movement, and updates the game map accordingly.
    6. clearCell: Clears the specified cell in the gameMap and updates the LED matrix.
    7. clearAdjacentCells: Clears adjacent cells around the specified position in the gameMap and updates the LED matrix.
    8. isValidPosition: Checks if the specified position is within the valid bounds of the gameMap.
    9. isPlayerInBombRange: Checks if the player is in the bomb range based on their positions. Returns true if the player is within one cell in both horizontal and vertical directions from the bomb.
    10. handlePlayerBlinking: Handles player blinking at a regular interval to provide a visual effect.
    11. handleRapidBlinking: Handles rapid blinking of the bomb at a faster interval, creating a visual effect for the bomb.

## Step 3: Finalizing the code

This Arduino code implements a simple LED matrix Bomberman type game where a player moves through a maze on an 8x8 LED matrix. The maze is randomly generated with a guaranteed path from the top-left to the bottom-right. The player's position is controlled by an analog joystick, and a bomb can be placed using the joystick button in order to destroy the walls of the maze; in case the player is in range of the bomb the game resets.

Code:

```
#include "LedControl.h"

// Constants
const int dPin = 12;
const int cPin = 11;
const int lPin = 10;
const byte matSize = 8;

// Controller
const int xPin = A0;
const int yPin = A1;
const int pinSW = A2;

// LedControl instance
LedControl lc = LedControl(dPin, cPin, lPin, 1);

// Game settings
byte gameMapBrightness = 2;
byte gameMap[8][8];

// Player position
byte playerPosX = 0;
byte playerPosY = 0;
byte lastPlayerPosX = 0;
byte lastPlayerPosY = 0;

//Joystick variables used to set its sensitivity
const int minThreshold = 200;
const int maxThreshold = 600;
const byte moveInterval = 100;
unsigned long long lastMoved = 0;
bool mapHasChanged = true;

// Button and Bomb state
boolean buttonState = false;
boolean bombIsPlaced = false;
int bombPosX = -1;
int bombPosY = -1;
const int explosionTime = 2000;
int lastPositionSetTime;

// Function prototypes
void generateRandomgameMap();
void setgameMap();
void resetgameMap();
void mapUpdate();
void playerPosUpdate();
void clearCell(int x, int y);
void clearAdjacentCells(int x, int y);
bool isValidPosition(int x, int y);
bool isPlayerInBombRange(int playerX, int playerY, int bombX, int bombY);
void handlePlayerBlinking(byte x, byte y);
void handleRapidBlinking(byte x, byte y);

void setup() {
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP);
  randomSeed(analogRead(A3));
  setgameMap();
  gameMap[playerPosX][playerPosY] = 1;
}

void loop() {
  boolean currBtnState = digitalRead(pinSW);

  // Toggle button state when button is pressed
  if (currBtnState == LOW) {
    buttonState = !buttonState;
  }

  // Update player position at a regular interval
  if (millis() - lastMoved >= moveInterval) {
    lastPlayerPosX = playerPosX;
    lastPlayerPosY = playerPosY;
    playerPosUpdate();

    // Place bomb if button is pressed
    if ((lastPlayerPosX != playerPosX || lastPlayerPosY != playerPosY) && buttonState == true) {
      gameMap[lastPlayerPosX][lastPlayerPosY] = 1;
      lc.setLed(1, lastPlayerPosX, lastPlayerPosY, gameMap[lastPlayerPosX][lastPlayerPosY]);

      lastPositionSetTime = millis();

      bombIsPlaced = true;
      bombPosX = lastPlayerPosX;
      bombPosY = lastPlayerPosY;
      buttonState = false;
    }
    lastMoved = millis();
  }

  // Handle rapid blinking if bomb is placed
  if (bombIsPlaced) {
    handleRapidBlinking(bombPosX, bombPosY);
  }

  // Handle bomb explosion after a certain time
  if (bombIsPlaced && millis() - lastPositionSetTime >= explosionTime) {
    // Clear bomb and adjacent cells
    clearCell(bombPosX, bombPosY);
    clearAdjacentCells(bombPosX, bombPosY);

    // Check if player is in bomb range and reset gameMap if true
    if (isPlayerInBombRange(playerPosX, playerPosY, bombPosX, bombPosY)) {
      resetgameMap();
    }
    bombIsPlaced = false;
    mapUpdate();
  }

  // Update LED matrix display
  if (mapHasChanged) {
    mapUpdate();
    mapHasChanged = false;
  }

  // Handle player blinking
  handlePlayerBlinking(playerPosX, playerPosY);
}

// Generate a random game map with a guaranteed path
void generateRandomgameMap() {
  // Initialize gameMap with a guaranteed path from top-left to bottom-right
  gameMap[0][0] = 0;
  gameMap[matSize - 1][matSize - 1] = 0;

  // Generate random walls with a guarantee of a path
  for (int row = 1; row < matSize - 1; row++) {
    for (int col = 1; col < matSize - 1; col++) {
      if (gameMap[row][col] != 1)
        gameMap[row][col] = random(2); // 0 or 1
    }
  }

  // Ensure a path from the top-left to bottom-right corners
  for (int row = 1; row < matSize - 1; row++) {
    for (int col = 1; col < matSize - 1; col++) {
      if (gameMap[row][col] == 1) {
        // Ensure there is at least one adjacent open cell
        if (!(gameMap[row - 1][col] == 0 || gameMap[row + 1][col] == 0 || gameMap[row][col - 1] == 0 || gameMap[row][col + 1] == 0)) {
          gameMap[row][col] = 0;
        }
      }
    }
  }
}

// Set up the initial LED matrix display
void setgameMap() {
  lc.shutdown(0, false);
  lc.setIntensity(0, gameMapBrightness);
  lc.clearDisplay(0);
  gameMap[playerPosX][playerPosY] = 1;
  generateRandomgameMap();
  mapUpdate();
}

// Reset the gameMap to the initial state or generate a new random gameMap
void resetgameMap() {
  randomSeed(analogRead(A3));
  setgameMap();
  delay(2000); // Delay to show the reset gameMap before continuing the game
}

// Update LED matrix display based on the gameMap array
void mapUpdate() {
  for (int row = 0; row < matSize; row++) {
    for (int col = 0; col < matSize; col++) {
      lc.setLed(0, row, col, gameMap[row][col]);
    }
  }
}

// Update player position based on analog joystick readings
void playerPosUpdate() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int hasMoved = false;
  byte newplayerPosX = playerPosX;
  byte newplayerPosY = playerPosY;

  // Move player based on joystick readings
  if (xValue < minThreshold && newplayerPosX > 0 && hasMoved == false) {
    newplayerPosX--;
    hasMoved = true;
  }else if (xValue > maxThreshold && newplayerPosX < matSize - 1 && hasMoved == false) {
    newplayerPosX++;
    hasMoved = true;
  }

  if (yValue < minThreshold && newplayerPosY < matSize - 1 && hasMoved == false) {
    newplayerPosY++;
    hasMoved = true;
  }else if (yValue > maxThreshold && newplayerPosY > 0 && hasMoved == false) {
    newplayerPosY--;
    hasMoved = true;
  }

  // Update gameMap if the new position is a valid open cell
  if (gameMap[newplayerPosX][newplayerPosY] == 0) {
    mapHasChanged = true;
    gameMap[playerPosX][playerPosY] = 0;
    gameMap[newplayerPosX][newplayerPosY] = 1;
    lastPlayerPosX = playerPosX;
    lastPlayerPosY = playerPosY;
    playerPosX = newplayerPosX;
    playerPosY = newplayerPosY;
  }
}

// Clear the specified cell in the gameMap and update the LED matrix
void clearCell(int x, int y) {
  if (isValidPosition(x, y)) {
    gameMap[x][y] = 0;
    lc.setLed(1, x, y, gameMap[x][y]);
  }
}

// Clear adjacent cells around the specified position
void clearAdjacentCells(int x, int y) {
  clearCell(x - 1, y);
  clearCell(x + 1, y);
  clearCell(x, y - 1);
  clearCell(x, y + 1);
}

// Check if the specified position is within the valid bounds of the gameMap
bool isValidPosition(int x, int y) {
  return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// Check if the player is in the bomb range based on their positions
bool isPlayerInBombRange(int playerX, int playerY, int bombX, int bombY) {
  return (abs(playerX - bombX) <= 1 && abs(playerY - bombY) <= 1);
}

// Handle player blinking at a regular interval
void handlePlayerBlinking(byte x, byte y) {
  static unsigned long lastBlinkTime = 0;
  static bool isOn = true;
  static unsigned blinkInterval = 400;
  if (millis() - lastBlinkTime >= blinkInterval) {
    isOn = !isOn;
    lc.setLed(0, x, y, isOn);
    lastBlinkTime = millis();
  }
}

// Handle rapid blinking of the bomb at a faster interval
void handleRapidBlinking(byte x, byte y) {
  static unsigned long lastBlinkTime = 0;
  static bool isOn = true;
  static unsigned blinkInterval = 200;
  if (millis() - lastBlinkTime >= blinkInterval) {
    isOn = !isOn;
    lc.setLed(0, x, y, isOn);
    lastBlinkTime = millis();
  }
}

```

Demo :
https://youtube.com/shorts/GJBOzU7dluU?feature=share
