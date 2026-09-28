//Joystick Pins
int x_key = A0;
int y_key = A1;
int x_pos;
int y_pos;

//Motor Pins
int EN_A = 11; //Enable pin for first motor
int IN1 = 9; //control pin for first motor
int IN2 = 8; //control pin for first motor
int IN3 = 7; //control pin for second motor
int IN4 = 6; //control pin for second motor
int EN_B = 10; //Enable pin for second motor
//Initializing variables to store data
int motor_speed;
int motor_speed1;

void setup ( ) {
Serial.begin (9600); //Starting the serial communication at 9600 baud rate
//Initializing the motor pins as output
pinMode(EN_A, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(EN_B, OUTPUT);

//Initializng the joystick pins as input
pinMode (x_key, INPUT) ;
pinMode (y_key, INPUT) ;
}

void loop () {
x_pos = analogRead (x_key) ; //Reading the horizontal movement value
y_pos = analogRead (y_key) ; //Reading the vertical movement value

if (x_pos < 400){ //Rotating the left motor in clockwise direction
motor_speed = map(x_pos, 400, 0, 0, 255); //Mapping the values to 0-255 to move the motor
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
analogWrite(EN_A, motor_speed);
}

else if (x_pos>400 && x_pos <600){ //Motors will not move when the joystick will be at center
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
}

else if (x_pos > 600){ //Rotating the left motor in anticlockwise direction
motor_speed = map(x_pos, 600, 1023, 0, 255);
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
analogWrite(EN_A, motor_speed);
}

if (y_pos < 400){ //Rotating the right motor in clockwise direction
motor_speed1 = map(y_pos, 400, 0, 0, 255);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(EN_B, motor_speed1);
}

else if (y_pos>400 && y_pos <600){
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}

else if (y_pos > 600){ //Rotating the right motor in anticlockwise direction
motor_speed1 = map(y_pos, 600, 1023, 0, 255);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(EN_B, motor_speed1);
}
}