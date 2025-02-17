#include <PID_v1.h>

// Pin definitions
const int trigPin = 9;
const int echoPin = 10;
const int pumpPin = 3;

// Variables for ultrasonic sensor
long duration;
float distance;
float waterLevel;
const float tankHeight = 100.0;  // Example tank height in cm

// PID variables
double setpoint = 50.0;    // Desired water level in cm
double input;              // Measured water level
double output;             // Output to control the pump (PWM or on/off)
double Kp = 2.0, Ki = 5.0, Kd = 1.0;  // PID constants

// Create PID object
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  // Pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);

  // Initialize the PID controller
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255);  // Output range for controlling pump via PWM
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Measure the current water level
  waterLevel = readWaterLevel();
  input = waterLevel;
  
  // Compute the PID output
  myPID.Compute();
  
  // Control the pump with the PID output
  controlPump(output);

  // Debugging information
  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.print(" cm, PID Output: ");
  Serial.println(output);

  delay(1000);  // Adjust loop delay as needed
}

// Function to read the water level using the ultrasonic sensor
float readWaterLevel() {
  // Send trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read echo pulse
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance to water surface (cm)
  distance = (duration * 0.0343) / 2;
  
  // Calculate water level (subtract distance from tank height)
  float level = tankHeight - distance;

  // Clamp the water level to valid range
  if (level < 0) level = 0;
  if (level > tankHeight) level = tankHeight;
  
  return level;
}

// Function to control the pump based on PID output
void controlPump(double pidOutput) {
  // If output is above a threshold, turn the pump ON
  if (pidOutput > 100) {
    digitalWrite(pumpPin, HIGH);
  } 
  // Otherwise, turn the pump OFF
  else {
    digitalWrite(pumpPin, LOW);
  }
}
