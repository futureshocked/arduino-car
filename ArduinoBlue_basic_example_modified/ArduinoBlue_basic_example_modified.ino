/*
ArduinoBlue example code to demonstrate the features of the app.
*/

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

const unsigned long BAUD_RATE = 9600;

// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.

const int BLUETOOTH_TX = 2;
const int BLUETOOTH_RX = 3;

int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering, button, sliderId;

const int in1 = 5; // in1,2 for right wheel
const int in2 = 6;
const int in3 = 9; // in3,4 for left wheel
const int in4 = 10;

#define RED_PIN 13

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor

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

// Setup code runs once after program starts.
void setup() {
    // Start serial communications.
    // The baud rate must be the same for both the serial and the bluetooth.
    Serial.begin(BAUD_RATE);
    bluetooth.begin(BAUD_RATE);
    pinMode(RED_PIN, OUTPUT);
    digitalWrite(RED_PIN, LOW);
       
    //motor
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
  
    delay(100);

    Serial.println("setup complete");
}

// Put your main code here, to run repeatedly:
void loop() {
    // ID of the button pressed pressed.
    button = phone.getButton();

    if (button == 0) {
    Serial.println("LED ON");
    digitalWrite(RED_PIN, HIGH);
  }

  if (button == 1) {
    Serial.println("LED OFF");
    digitalWrite(RED_PIN, LOW);
  }

    // Returns the text data sent from the phone.
    // After it returns the latest data, empty string "" is sent in subsequent.
    // calls until text data is sent again.
    String str = phone.getText();

    // Throttle and steering values go from 0 to 99.
    // When throttle and steering values are at 99/2 = 49, the joystick is at center.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    // Display throttle and steering data if steering or throttle value is changed
    if (prevThrottle != throttle || prevSteering != steering) {
        Serial.print("Throttle: "); Serial.print(throttle); Serial.print("\tSteering: "); Serial.println(steering);
        prevThrottle = throttle;
        prevSteering = steering;
    }

    if (throttle >= 40 && throttle <= 60)
    {
      stopMove();
    } else if ( throttle > 60 & (steering >= 40 && steering <= 60))
    {
      moveForward(150);
    } else if ( throttle < 40 & (steering >= 40 && steering <= 60))
    {
      moveBackward(150);
    } else if ( steering > 60 & throttle > 60)
    {
      turnRight(255);
    } else if ( steering < 40 & throttle < 40)
    {
      turnLeft(255);
    }
    
    // If a text from the phone was sent print it to the serial monitor
    if (str != "") {
        Serial.println(str);
         if (str == "1")
        {
          digitalWrite(RED_PIN, HIGH);
        } else if (str == "0") {
          digitalWrite(RED_PIN, LOW);
        }
    }

    // Send string from serial command line to the phone. This will alert the user.
    if (Serial.available()) {
        Serial.write("send: ");
        String str = Serial.readString();
        phone.sendMessage(str); // phone.sendMessage(str) sends the text to the phone.
        Serial.print(str);
        Serial.write('\n');
       
    }
}
