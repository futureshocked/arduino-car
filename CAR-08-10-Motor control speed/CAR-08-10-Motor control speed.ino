// This sketch shows how to convert joystick x-y values to PWM values that 
// can be used to control speed. 
// In this example, because of my Arduino car's wiring, forward and backward
// motions are controlled in the X axis. Therefore, this sketch converts the
// joysticks X axis values into compatible (within PWM range) PWM values for
// the Arduino Uno, and passes those values to the motor via the 
// speed parameter of the moveB function.

const int in1 = 5;  // in1,2 for right wheel
const int in2 = 6;
const int in3 = 9;  // in3,4 for left wheel
const int in4 = 10;

const int joystick_x = A0;
const int joystick_y = A1;

int Xvalue = 0;
int Yvalue = 0;

int deflection = 0;
int x_max_measured = 0;
int motor_speed = 0;

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
}

void loop() {
  // put your main code here, to run repeatedly:
  Xvalue = analogRead(joystick_x);
  Yvalue = analogRead(joystick_y);

  // Let's check the maximum value for the x axis.
  // The sketch will measure this each time it starts to accomodate for
  // joysticks with different output ranges.
  if (Xvalue > x_max_measured) {
    x_max_measured = Xvalue;
  }

  deflection = abs(x_max_measured / 2 - Xvalue);  // Because I hold the joystick with the headers pointing down, the x and y are alternated
  motor_speed = map(deflection, 0, x_max_measured / 2, 0, 255);

  Serial.print("X: ");
  Serial.print(Xvalue, DEC);  // print the value of VRX in DEC
  Serial.print(" | Y: ");
  Serial.print(Yvalue, DEC);  // print the value of VRX in DEC
  Serial.print("\t x_max_measured=");
  Serial.print(x_max_measured);
  Serial.print("\t def=");
  Serial.print(deflection);
  Serial.print("\t speed=");
  Serial.println(motor_speed);

  // Get these values through experimentation .
  if ( (Xvalue >= x_min && Xvalue <= x_max) && 
       (Yvalue >= y_min && Yvalue <= y_max)) {
    stopMove();
  } else if (Xvalue < x_min) {
    moveA(motor_speed);  // Move forward at variable speed
  } else if (Xvalue > x_max) {
    moveB(motor_speed);  // Move backward at variable speed
  } else if (Yvalue > y_max) {
    turnA(low_speed);  // Turn at fixed speed
  } else if (Yvalue < y_min) {
    turnB(high_speed);  // Turn at fixed speed
  }

  delay(100);
}