// This sketch shows how to convert joystick x-y values to PWM values that 
// can be used to control speed. 
// In this example, because of my Arduino car's wiring, forward and backward
// motions are controlled in the X axis. Therefore, this sketch converts the
// joysticks X axis values into compatible (within PWM range) PWM values for
// the Arduino Uno, and passes those values to the motor via the 
// speed parameter of the moveB function.

const int in1 = 4;  // in1,2 for right wheel. Original pin: 5
const int in2 = 5;  // Original pin: 6
const int in3 = 12; // in3,4 for left wheel. Original pin: 9
const int in4 = 13; // Original pin: 10
const int ENA = 6;  // Pin 6 is PWM capable
const int ENB = 11; // Pin 11 is PWM capable

const int joystick_x = A0;
const int joystick_y = A1;

int Xvalue = 0;
int Yvalue = 0;

int deflection = 0;
int x_max_measured = 0;
int motor_speed = 0;

// X and Y max min values for no movement. 
// I have defined these values experimentally.
const int x_max = 530;
const int x_min = 500;
const int y_max = 530;
const int y_min = 500;
const int MINIMUM_MOTOR_SPEED = 15;

const int turning_speed = 100;

void moveA() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 1); // 1 replaces speed
  digitalWrite(in3, 1); // 1 replaces speed
  digitalWrite(in4, 0);
}

void moveB() {
  digitalWrite(in1, 1); // 1 replaces speed
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1); // 1 replaces speed
}

void turnB() {
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
}

void turnA() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
}

void stopMove() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(ENB, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
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
  motor_speed = map(deflection, 0, x_max_measured / 2, MINIMUM_MOTOR_SPEED, 255);

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
    analogWrite(ENA, motor_speed);
	  analogWrite(ENB, motor_speed);
    moveA();  // Move forward at variable speed
  } else if (Xvalue > x_max) {
    analogWrite(ENA, motor_speed);
	  analogWrite(ENB, motor_speed);
    moveB();  // Move backward at variable speed
  } else if (Yvalue > y_max) {
    analogWrite(ENA, turning_speed);
	  analogWrite(ENB, turning_speed);
    turnA();  // Turn at fixed speed
  } else if (Yvalue < y_min) {
    analogWrite(ENA, turning_speed);
	  analogWrite(ENB, turning_speed);
    turnB();  // Turn at fixed speed
  }

  delay(100);
}