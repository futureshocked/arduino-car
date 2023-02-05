// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include "credentials.h"
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

const int redLedPin          = 7;
const int greenConnectionPin = 8; // The LED on this pin will turn on the the car is connected to Blynk

/*car init*/

const int in1 = 5; // in1,2 for right wheel
const int in2 = 6;
const int in3 = 9; // in3,4 for left wheel
const int in4 = 10;

void moveBackward(int speed) {
  analogWrite(in1, 0);
  analogWrite(in2, speed);
  analogWrite(in3, speed);
  analogWrite(in4, 0);
}

void moveForward(int speed) {
  analogWrite(in1, speed);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, speed);
}

void turnLeft(int speed) {
  analogWrite(in1, speed);
  analogWrite(in2, 0);
  analogWrite(in3, speed);
  analogWrite(in4, 0);
}

void turnRight(int speed) {
  analogWrite(in1, 0);
  analogWrite(in2, speed);
  analogWrite(in3, 0);
  analogWrite(in4, speed);
}

void stopMove() {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V0 to a variable
  digitalWrite(redLedPin,pinValue);
}

int Xvalue = 0;
int Yvalue = 0;

BLYNK_WRITE(V1)
{
  Xvalue = param.asInt();
  Serial.print("X: ");
  Serial.println(Xvalue);
}

BLYNK_WRITE(V2)
{
  Yvalue = param.asInt();
  Serial.print("Y: ");
  Serial.println(Yvalue);  
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

  pinMode(redLedPin,OUTPUT);
  pinMode(greenConnectionPin,OUTPUT);
  digitalWrite(greenConnectionPin, LOW);

   //motor
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

}

void loop()
{
  Blynk.run();

  if (!Blynk.connected()) {
    digitalWrite(greenConnectionPin, LOW);
    Serial.print("offline!");
    bool result = Blynk.connect();
    Serial.println(result);
  } else {
    digitalWrite(greenConnectionPin, HIGH);
  }

  if (Yvalue >= 5) { // Actually forward
    moveForward(150); 
  } else if (Yvalue <= -5) { // Actually backward
    moveBackward(150);
  } else if (Xvalue >= 5) {
    turnRight(255);  // Actually a right turn
  } else if (Xvalue <= -5) {
    turnLeft(255); // Actually left turn
  } else {
    stopMove();
  }
}