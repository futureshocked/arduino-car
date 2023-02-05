// This sample sketch shows how to take joystick x-y values 
// and convert them to a new value within the Arduino Uno's
// PWM range. The PWM value can be used to control speed.
// I have prepared a separate sample sketch in this lecture 
// where the calculated PWM value is applied to the forward
// motion.

const byte x_pin      = A0;
const byte y_pin      = A1;

int deflection      = 0;
int x_max_measured  = 0;
int motor_speed     = 0;

int x;
int y;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the joystick inputs
  x = analogRead(x_pin);
  y = analogRead(y_pin);

  // Let's check the maximum value for the x axis. 
  // The sketch will measure this each time it starts to accomodate for 
  // joysticks with different output ranges.
  if (x > x_max_measured)
    { 
      x_max_measured = x;
    }
  // Because I hold the joystick with the headers pointing down, 
  // the x axis controls the forward/backward motion.
  deflection  = abs(x_max_measured/2 - x); 
  motor_speed = map(deflection, 0, x_max_measured/2, 0, 255);

  // print out the values captured above
  Serial.print("x=");
  Serial.print(x);
  Serial.print("\t y=");
  Serial.print(y);
  Serial.print("\t x_max_measured=");
  Serial.print(x_max_measured);
  Serial.print("\t def=");
  Serial.print(deflection);
  Serial.print("\t speed=");
  Serial.print(motor_speed);
  Serial.println();

  delay(10);
}
