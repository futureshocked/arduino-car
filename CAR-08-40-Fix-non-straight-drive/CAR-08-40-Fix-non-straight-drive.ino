// This sketch contains an offset variable that you can use
// to calibrate the relative speed difference between the motors
// that results in the car being unable to drive straight.
// My car tends to drive along a right curve. This means that the
// left motor rotates slightly faster than the right motor.
// To compensate for the difference, I have experimentally determined
// a small speed offset that is added to the speed of the left motor so
// that the effect of straight drive is achieved.

const int in1 = 5; // in1,2 for right wheel
const int in2 = 6;
const int in3 = 9; // in3,4 for left wheel
const int in4 = 10;

const int motor_speed_offset = - 10; // This number derives experimentally.

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
  analogWrite(in4, speed + motor_speed_offset); // Add the offset value to the left motor 
                                                // speed to reduce the forward drive
                                                // right curve tendency.
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
}

void loop() {
  // put your main code here, to run repeatedly:
  Xvalue = analogRead(joystick_x);
  Yvalue = analogRead(joystick_y);  
  Serial.print("X: "); 
  Serial.print(Xvalue, DEC);  // print the value of VRX in DEC
  Serial.print("|Y: ");
  Serial.println(Yvalue, DEC);  // print the value of VRX in DEC

  // Get these values through experimentation .
  if ((Xvalue >= x_min && Xvalue <=x_max) && 
      (Yvalue >= y_min && Yvalue <= y_max))
  {
    stopMove();
  } else if (Xvalue < x_min) {
    moveA(low_speed); 
  } else if (Xvalue > x_max) {
    moveB(low_speed); 
  } else if (Yvalue > y_max) {
    turnA(low_speed);
  } else if (Yvalue < y_min) {
    turnB(high_speed); 
  }

  delay(100);
}