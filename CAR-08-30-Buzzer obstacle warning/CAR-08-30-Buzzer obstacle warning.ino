// This sketch builds on the one from lecture 8.20 and adds a buzzer.
// When an obstacle is detected within the specified range,
// the buzzer will emit a warning tone.

const int in1 = 5;  // in1,2 for right wheel
const int in2 = 6;
const int in3 = 9;  // in3,4 for left wheel
const int in4 = 10;

const int joystick_x = A0;
const int joystick_y = A1;

int Xvalue = 0;
int Yvalue = 0;

// X and Y max min values for no movement. I have define these values experimentally.
const int x_max = 530;
const int x_min = 500;
const int y_max = 530;
const int y_min = 500;

const int high_speed = 255;
const int low_speed = 150;

const int obstacle_distance = 10; // Stop the car's forward movement within this range

#define trigPin 12
#define echoPin 13
#define obstacleDetectedPin 11

const int buzzerPin = 3;

void moveA(int speed) {
  analogWrite(in1, 0);
  analogWrite(in2, speed);
  analogWrite(in3, speed);
  analogWrite(in4, 0);
}

void moveB(int speed) {
  analogWrite(in1, speed);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, speed);
}

void turnB(int speed) {
  analogWrite(in1, speed);
  analogWrite(in2, 0);
  analogWrite(in3, speed);
  analogWrite(in4, 0);
}

void turnA(int speed) {
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(obstacleDetectedPin, OUTPUT);
  digitalWrite(obstacleDetectedPin, LOW);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  Xvalue = analogRead(joystick_x);
  Yvalue = analogRead(joystick_y);


  Serial.print("X: ");
  Serial.print(Xvalue, DEC);  // print the value of VRX in DEC
  Serial.print("| Y: ");
  Serial.print(Yvalue, DEC);  // print the value of VRX in DEC

  // Get these values through experimentation .
  if ((Xvalue >= x_min && Xvalue <= x_max) && (Yvalue >= y_min && Yvalue <= y_max)) {
    stopMove();
  } else if (Xvalue < x_min) {  // No forward move allowed if obstacle is detected
    moveA(low_speed);
  } else if (Xvalue > x_max) {
    if (detectObstacle() == 0)  // During forward move instruction, check for obstacles.
    {
      moveB(low_speed);  // If there is none, move.
    } else {
      makeNoise();
      stopMove();  // or moveB(0);   // If obstacle is detected, stop.
    }
  } else if (Yvalue > y_max) {
    turnA(low_speed);
  } else if (Yvalue < y_min) {
    turnB(high_speed);
  }

  delay(100);
}

int detectObstacle() {
  long duration, distance;
  byte obstacleDetected = 0;  // If an obstacle is detected, change to 1

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance <= obstacle_distance) {
    obstacleDetected = 1;
    digitalWrite(obstacleDetectedPin, HIGH);
  } else {
    obstacleDetected = 0;
    digitalWrite(obstacleDetectedPin, LOW);
  }

  Serial.print("| D: ");
  Serial.print(distance, DEC);
  Serial.print("| O: ");
  Serial.println(obstacleDetected, DEC);

  return obstacleDetected;
}

void makeNoise() {
  for (int i = 0; i < 50; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(3);
    digitalWrite(buzzerPin, LOW);
    delay(3);
  }
}