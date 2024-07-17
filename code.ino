#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <Servo.h>

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 100
#define MICROSTEPS 1
#define DIR 8
#define STEP 9

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

// Servo motor configuration
const int servoPin = 10;
Servo doorServo;

// Define the floor positions
const int FLOOR1 = 0;
const int FLOOR2 = 1;
const int FLOOR3 = 2;
const int FLOOR4 = 3;

// Define the button pins for each floor
const int outsideButton1 = 6;
const int outsideButton2 = 7;
const int outsideButton3 = 12;
const int outsideButton4 = A0;
const int insideButton1 = 2;
const int insideButton2 = 3;
const int insideButton3 = 4;
const int insideButton4 = 5;

// IR sensor pins
const int irSensor1 = A4;
const int irSensor2 = A1;
const int irSensor3 = A2;
const int irSensor4 = A3;

// Variables to track the current floor and target floor
int currentFloor = FLOOR1;
int targetFloor = FLOOR1;

// Function to move the elevator to the target floor
void moveElevator() {
  
  int steps = targetFloor - currentFloor;
  int direction = (steps > 0) ? 1 : -1;
  steps = abs(steps);

  //Moving in the direction of target floor and checking the input from ir sensor
  
  int i;
  for(i=0;i<steps;i++){
    stepper.rotate(180*37*(-1)* direction);
    currentFloor = currentFloor + direction;
                                       // if(digitalRead(irSensor) == HIGH){
                                       //  targetFloor = currentFloor;
                                      // break;
                                      // }
  }
  
  
  //if(i == steps){
    // Open the elevator door
    //openDoor();

    // Wait for a few seconds with the door open
    //delay(5000);

    // Close the elevator door
    //closeDoor();
  //}
 
}

// Function to handle button presses and irsensor
void buttonPressed() {
  int irSensor;
  if (digitalRead(outsideButton1) == HIGH || digitalRead(insideButton1) == HIGH) {
     targetFloor = FLOOR1;
    irSensor = irSensor1;
  } else if (digitalRead(outsideButton2) == HIGH  || digitalRead(insideButton2) == HIGH) {
    targetFloor = FLOOR2;
    irSensor = irSensor2;
  } else if (digitalRead(outsideButton3) == HIGH || digitalRead(insideButton3) == HIGH) {
    targetFloor = FLOOR3;
    irSensor = irSensor3;
  } else if (digitalRead(outsideButton4) == HIGH || digitalRead(insideButton4) == HIGH) {
    targetFloor = FLOOR4;
    irSensor = irSensor4;
  }
} 

void openDoor() {
  // Rotate the servo motor to open the door
  doorServo.write(90);
}

void closeDoor() {
  // Rotate the servo motor to close the door
  doorServo.write(0);
}

void setup() {
   // Set push button pins as inputs
  pinMode(outsideButton1, INPUT);
  pinMode(outsideButton2, INPUT);
  pinMode(outsideButton3, INPUT);
  pinMode(outsideButton4, INPUT);
  pinMode(insideButton1, INPUT);
  pinMode(insideButton2, INPUT);
  pinMode(insideButton3, INPUT);
  pinMode(insideButton4, INPUT);

  // Set IR sensor pins as inputs
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(irSensor3, INPUT);
  pinMode(irSensor4, INPUT);
  
  stepper.begin(RPM, MICROSTEPS);

   // Attach the servo motor to the pin
  doorServo.attach(servoPin);
}

void loop() {
  buttonPressed();
 
  if (targetFloor != currentFloor) {
    moveElevator();
  }
}
