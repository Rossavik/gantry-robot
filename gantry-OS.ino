#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

// Define pin numbers for buttons
const int buttonPin1 = 3;
const int buttonPin2 = 2;

// Define stepper motor parameters
const int stepsPerRevolutionX = 200;  // Steps per revolution for X axis motor
const int stepsPerRevolutionY = 200;  // Steps per revolution for Y axis motor
const int stepsPerRevolutionZ = 200;  // Steps per revolution for Z axis motor
const int xStep = 60;
const int xDir = 61;
const int yStep = 54;
const int yDir = 55;
const int zStep = 56;
const int zDir = 57;  // Corrected to a different pin for Z direction
const int enaPin = 13;

// Initialize stepper motors
AccelStepper stepperX(AccelStepper::DRIVER, xStep, xDir);
AccelStepper stepperY(AccelStepper::DRIVER, yStep, yDir);
AccelStepper stepperZ(AccelStepper::DRIVER, zStep, zDir);

void setup() {
  // Initialize Serial communication at a baud rate of 9600
  Serial.begin(9600);
  
  // Configure the button pins as inputs
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(enaPin, OUTPUT);
  
  // Enable the stepper drivers by setting the enable pin LOW (or HIGH if required by your driver)
  digitalWrite(enaPin, HIGH);
  
  // Set the maximum speed and acceleration for the stepper motors
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(2000);
  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(2000);
  stepperZ.setMaxSpeed(10000);
  stepperZ.setAcceleration(2000);
}

void loop() {
  // Read the state of the buttons
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);
  
  // Control Z-axis stepper based on button presses
  if (buttonState1 == HIGH) {
    stepperZ.move(-stepsPerRevolutionZ); // Move Z-axis up
  } else if (buttonState2 == HIGH) {
    stepperZ.move(stepsPerRevolutionZ); // Move Z-axis down
  }
  
  // Read the position of the joystick
  int joystickX = analogRead(A15); // Read X-axis
  int joystickY = analogRead(A14); // Read Y-axis
  
  // Define the dead zone for the joystick
  const int deadZone = 15;
  const int centerValue = 511.5;

  // Calculate the distance from the center for both axes
  int deltaX = joystickX - centerValue;
  int deltaY = joystickY - centerValue;

  // Check if the joystick is outside the dead zone
  if (abs(deltaX) > deadZone) {
    // Map the joystick position to a speed value
    float speedX = map(abs(deltaX), deadZone, 512, 0, 2000);
    stepperX.setSpeed((deltaX > 0) ? speedX : -speedX);
    stepperX.runSpeed();
  }

  if (abs(deltaY) > deadZone) {
    // Map the joystick position to a speed value
    float speedY = map(abs(deltaY), deadZone, 512, 0, 2000);
    stepperY.setSpeed((deltaY > 0) ? speedY : -speedY);
    stepperY.runSpeed();
  }

  // Run the Z stepper motor to reflect the button presses
  stepperZ.run();
}
