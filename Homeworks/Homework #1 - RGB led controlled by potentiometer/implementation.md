# Homework #1 - RGB Led controlled by potentiometers

This implementation involves three main steps. I'm using Arduino to control an RGB LED based on the values from potentiometers. Let me break down the process for you:

## Step 1: Find the limit of each potentiometer

First we place the potentiometer on the breadboard. Ensure the three pins of the potentiometer align with three different rows on the breadboard. Then we use a jumper wire to connect the center pin (wiper) of the potentiometer to an analog input pin on your Arduino. This pin will be used to read the variable resistance value. Lastly, connect the left pin to the ground and the right pin to the 5V rail. Connect each of the three potentiometers to the analog input pins of your Arduino. Once we have the circuit completed, we can implement the code and find the maximum value of each potentiometer.

![Circuit implementation](/Screenshots%20and%20pictures/step1-circuit.jpg)
![Code implementation](/Screenshots%20and%20pictures/step1-code.PNG)

## Step 2: Adding the RGB LED to the circuit

Place the RGB LED on the breadboard and connect the longer lead (common cathode/anode) to the ground and the shorter leads (for red, green, and blue) on the other side. Insert each lead into separate rows on the breadboard. Connect the resistors in series with each of the RGB pins (red, green, and blue). Connect the red, green, and blue leads of the RGB LED to digital output pins on your Arduino.

![Circuit implementation](/Screenshots%20and%20pictures/step2-circuit.jpg)
![Code implementation](/Screenshots%20and%20pictures/step2-code.PNG)

## Step 3: Finalizing the code

The code begins by defining the pin connections for three potentiometers (potMeterPinR, potMeterPinG, and potMeterPinB) and three LED output pins (ledPinR, ledPinG, and ledPinB).

![Data definition](/Screenshots%20and%20pictures/defining_data.PNG)

In the setup() function, the code initializes serial communication with a baud rate of 9600 for debugging purposes using Serial.begin(9600). It then sets the potentiometer pins (potMeterPinR, potMeterPinG, and potMeterPinB) as OUTPUT pins using pinMode().

![Setup](/Screenshots%20and%20pictures/setup.PNG)

The loop() function is where the main action occurs. It continuously reads the analog values from the three potentiometers using analogRead() and stores these values in variables potMeterValR, potMeterValG, and potMeterValB. Next, the code uses the map() function to map the potentiometer values to a range of values suitable for controlling LEDs. The mapped values are stored in pwmValueR, pwmValueG, and pwmValueB. Finally, the code uses analogWrite() to write the mapped PWM values (brightness levels) to the LED pins (ledPinR, ledPinG, and ledPinB). This controls the color and intensity of an RGB LED. The delay(1000) function introduces a 1-second delay, causing the LED color to update at one-second intervals.

![Final code](/Screenshots%20and%20pictures/step3-code.PNG)

Watch the demo:
https://www.youtube.com/shorts/h66co6thW88