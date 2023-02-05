#include <IRremote.h>
const int recvPin = 12;
IRrecv irrecv(recvPin); //Create an instance of the ``IRrecv`` class, specifying the pins it's attached to.
decode_results results;

const int in1 = 5; // in1,2 for right wheel
const int in2 = 6;
const int in3 = 9; // in3,4 for left wheel
const int in4 = 10;

#define RED_PIN 7
#define GREEN_PIN  8

int speed = 150;

void setup() {
  Serial.begin(9600);

  //motor
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //UR remote
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("REMOTE CONTROL START");

  //LED
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {

  if (irrecv.decode(&results)) {
    //    Serial.println(results.value,HEX);
    String key = decodeKeyValue(results.value);
    if ( key != "ERROR")
    {
      Serial.println(key);
      blinkLED();
      if (key == "+") {
        speed += 50;
      } else if (key == "-") {
        speed -= 50;
      } else if (key == "2") {
        two_wheels_A(speed);
      } else if (key == "1") {
        one_wheel_A(speed);
      } else if (key == "3") {
        one_wheel_B(speed);
      } else if (key == "4") {
        two_wheels_oppposite_B(speed);
      } else if (key == "6") {
        two_wheels_oppposite_A(speed);
      } else if (key == "7") {
        one_wheel_C(speed); 
      } else if (key == "9") {
        one_wheel_D(speed); 
      } else if (key == "8") {
        two_wheels_B(speed);
      } else if (key == "5") {
        digitalWrite(RED_PIN, HIGH);
        delay(2000);
        digitalWrite(RED_PIN, LOW);
      } else if (key == "0") {
        digitalWrite(GREEN_PIN, HIGH);
        delay(2000);
        digitalWrite(GREEN_PIN, LOW);
      }

      if (speed >= 255) {
        speed = 255;
      }
      if (speed <= 0) {
        speed = 0;
      }
      delay(500);
      stopMove();
    }

    irrecv.resume(); // Receive the next value
  }
}

void two_wheels_oppposite_A(int speed) {
 analogWrite(in1, 0);
 analogWrite(in2, speed);
 analogWrite(in3, 0);
 analogWrite(in4, speed);
}

void two_wheels_oppposite_B(int speed) {
  analogWrite(in1, speed);
  analogWrite(in2, 0);
  analogWrite(in3, speed);
  analogWrite(in4, 0);
}

void two_wheels_B(int speed) {
 analogWrite(in1, 0);
 analogWrite(in2, speed);
 analogWrite(in3, speed);
 analogWrite(in4, 0);
}

void two_wheels_A(int speed) {
 analogWrite(in1, speed);
 analogWrite(in2, 0 );
 analogWrite(in3, 0);
 analogWrite(in4, speed);
}

void one_wheel_B(int speed) {
 analogWrite(in1, 0);
 analogWrite(in2, 0);
 analogWrite(in3, 0);
 analogWrite(in4, speed);
}

void one_wheel_D(int speed) {
  analogWrite(in1, 0);
  analogWrite(in2, speed);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void one_wheel_A(int speed) {
  analogWrite(in1, speed);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void one_wheel_C(int speed) {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, speed);
  analogWrite(in4, 0);
}

void stopMove() {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void blinkLED() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(RED_PIN, HIGH);
    delay(50);
    digitalWrite(RED_PIN, LOW);
    delay(50);
  }
}
