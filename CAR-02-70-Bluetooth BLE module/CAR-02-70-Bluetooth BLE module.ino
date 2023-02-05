// This sketch demonstrates how to use the AC-BT-V4 Bluetooth module
// with the ArduinoBlue library and mobile app.
// https://www.sunfounder.com/products/bluetooth-master-slave-module
// In this example, we will control the Arduino Uno LED on pin 13 with a button
// and exchange text messages.
// ArduinoBlue documentation: https://sites.google.com/stonybrook.edu/arduinoble/

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

const unsigned long BAUD_RATE = 9600;

// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX = 2;
// Bluetooth RX -> Arduino D7
const int BLUETOOTH_RX = 3;

#define RED_PIN 13

int button;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth);  // pass reference of bluetooth object to ArduinoBlue constructor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  bluetooth.begin(BAUD_RATE);
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  delay(100);

  Serial.println("setup complete");
}

void loop() {
  button = phone.getButton();  // get the ID of the button pressed

  if (button == 0) {
    Serial.println("LED ON");
    digitalWrite(RED_PIN, HIGH);
  }

  if (button == 1) {
    Serial.println("LED OFF");
    digitalWrite(RED_PIN, LOW);
  }

  String str = phone.getText();
  if (str != "") {
    Serial.println(str);
  }
  // Send string from serial command line to the phone. This will alert the user.
  if (Serial.available()) {
    Serial.write("send: ");
    String str = Serial.readString();
    phone.sendMessage(str);  // phone.sendMessage(str) sends the text to the phone.
    Serial.print(str);
    Serial.write('\n');
  }
}
