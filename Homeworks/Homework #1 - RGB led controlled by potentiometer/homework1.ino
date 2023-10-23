//Potentiometer pins
const int potMeterPinR = A0;
const int potMeterPinG = A1;
const int potMeterPinB = A2;

//Potentiometer values
int potMeterValR = 0;
int potMeterValG = 0;
int potMeterValB = 0;

//LED pins
const int ledPinR = 9;
const int ledPinG = 10;
const int ledPinB = 11;

void setup() {
  Serial.begin(9600);
  //Potentiometer pin initialization
  pinMode(potMeterPinR, OUTPUT);
  pinMode(potMeterPinG, OUTPUT);
  pinMode(potMeterPinB, OUTPUT);
}

void loop() {
  //Reading the potentiometer values
  potMeterValR = analogRead(potMeterPinR);
  potMeterValG = analogRead(potMeterPinG);
  potMeterValB = analogRead(potMeterPinB);

  //Mapping the potentiometer values to the range of values of the led
  int pwmValueR = map(potMeterValR,0, 800, 0 , 255);
  int pwmValueG = map(potMeterValM,0, 880, 0 , 255);
  int pwmValueB = map(potMeterValL,0, 780, 0 , 255);

  //Printing the potentiometer values
  Serial.println("potR = " + String(potMeterValR));
  Serial.println("potG = " + String(potMeterValG));
  Serial.println("potB = " + String(potMeterValB));

  //Writting the potentiometer mapped values to the led
  analogWrite(ledPinR,pwmValueR);
  analogWrite(ledPinG,pwmValueG);
  analogWrite(ledPinB,pwmValueB);
  delay(1000);
}
