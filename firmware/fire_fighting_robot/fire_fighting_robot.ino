// Fire-Fighting Robot Code
// Uses flame sensors, an ultrasonic sensor, a relay-controlled water pump, and a motor driver for movement.

// Define flame sensor pins (Analog pins to detect fire intensity)
const int flameSensor1Pin = A0;
const int flameSensor2Pin = A1;
const int flameSensor3Pin = A2;

// Define ultrasonic sensor pins (For obstacle detection)
const int trigPin = 13;  // Trig pin for HC-SR04
const int echoPin = 12;  // Echo pin for HC-SR04

// Define relay pin (Controls water pump)
#define RELAY_PIN 7

// Define motor driver pins (L298N motor driver - Only PWM-supported pins)
const int IN1 = 5;  // Motor 1 Forward (PWM ✅)
const int IN2 = 6;  // Motor 1 Backward (PWM ✅)
const int IN3 = 9;  // Motor 2 Forward (PWM ✅)
const int IN4 = 10; // Motor 2 Backward (PWM ✅)

void setup() {
  Serial.begin(9600);  // Start Serial Monitor for debugging

  // Set up sensor and motor control pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // Read flame sensor values (Lower values indicate fire presence)
  int flame1 = analogRead(flameSensor1Pin);
  int flame2 = analogRead(flameSensor2Pin);
  int flame3 = analogRead(flameSensor3Pin);

  // Measure distance using the ultrasonic sensor (Obstacle detection)
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);  // Measure the time for the echo signal
  distance = duration * 0.034 / 2;    // Convert time to distance in cm

  // Print sensor values to the Serial Monitor for debugging
  Serial.print("Flame 1: "); Serial.print(flame1);
  Serial.print("\tFlame 2: "); Serial.print(flame2);
  Serial.print("\tFlame 3: "); Serial.print(flame3);
  Serial.print("\tDistance: "); Serial.println(distance);

  // Fire detection and movement control logic:

  // Condition 1: If fire is detected AND no obstacle is present, move forward
  if ((flame1 > 40 && flame2 > 45 && flame3 > 40) && (distance > 25))
 {
   
     moveBackward(150); // Move forward at speed 150 (PWM value)
  } 
  // Condition 2: If an obstacle is close (≤ 15 cm), stop and extinguish fire
  else if (distance <= 30) {
     moveForward(0); 
    // Stop movement

    // Activate the water pump while the fire is still detected
   while (flame1 <= 50 && flame2 <= 55 && flame3 <= 50)
 {
      digitalWrite(RELAY_PIN, LOW);  // Turn ON the pump (assuming active LOW)
      // Re-check flame sensor values
      flame1 = analogRead(flameSensor1Pin);
      flame2 = analogRead(flameSensor2Pin);
      flame3 = analogRead(flameSensor3Pin);
    }
    digitalWrite(RELAY_PIN, HIGH); // Turn OFF the pump after fire is gone
  }
  // Condition 3: No fire detected → Stop movement and turn off pump
  else {
    moveBackward(0);  // Stop motors
    digitalWrite(RELAY_PIN, HIGH); // Ensure the pump is OFF
  }

  delay(500); // Small delay for stability
}

// Function to move the robot forward with PWM speed
void moveForward(int speed) {
  analogWrite(IN1, speed);
  analogWrite(IN2, 0);
  analogWrite(IN3, speed);
  analogWrite(IN4, 0);
}

// Function to move the robot backward with PWM speed
void moveBackward(int speed) {
  analogWrite(IN1, 0);
  analogWrite(IN2, speed);
  analogWrite(IN3, 0);
  analogWrite(IN4, speed);
}