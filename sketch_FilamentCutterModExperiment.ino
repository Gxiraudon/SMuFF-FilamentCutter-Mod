/****************************************************************

This is the ATMega 2560 demo sketch for the SMuFF filament cutter 
mod. the final sketch will be a modification of this. This sketch 
is only used for testing your SMuFF filament cutter mod.

*****************************************************************/

// include the relevant header files
#include <Servo.h> 
#include <AccelStepper.h>

// pin definitions:
#define motorPin1  8      // IN1 on the ULN2003 driver
#define motorPin2  9      // IN2 on the ULN2003 driver
#define motorPin3  10     // IN3 on the ULN2003 driver
#define motorPin4  11     // IN4 on the ULN2003 driver
#define ServoPin  3       // PWM input of the cutter servo
#define rest_position  98 // rest position of the cutter servo. might vary according to your setup
#define cut_position  67  // cutting position of the cutter servo. might vary according to your setup

// Define the AccelStepper interface type; 4 wire motor in half step mode:
#define MotorInterfaceType 8

// create servo object to control the cutter servo
Servo cutterservo;  

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:
AccelStepper ejectorstepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);
int ejectorabsolute_pos = 0;

int status = 1; 
int substatus = 2;

  /*
  The logic is that the Mega2560 will have a state machine algorithm 
  that has the following statuses. 

  status Meanings
  0 - Printer
  1 - Board
  substatus meanings:
  2 - cutter
  3 - ejector
  */

void setup() {
  // put your setup code here, to run once:

    cutterservo.attach(ServoPin);  // attaches the servo on pin 3 to the servo object, as defined by the prior definitions

    // some stepper motor settings
    // Set the speed limit in terms of maximum steps per second:
    ejectorstepper.setMaxSpeed(1500);
    // Set the maximum acceleration in steps per second^2:
    ejectorstepper.setAcceleration(500);


}

void loop() {
  // put your main code here, to run repeatedly:

  switch (status){
    case 0: // if status = printer
          // cutter should be in rest position
          cutterservo.write(rest_position);
          // on this demo, status 0 will end the demo, to restart the sketch, press the mega2560's reset button
    break;

    case 1: // if status = board
          switch (substatus){
              case 2: // if substatus = cutter
                  
                    // cutter should be in cutting position
                    cutterservo.write(cut_position);
                    substatus = 3; // change status to ejector

              break;

              case 3: // if substatus = ejector

                      // open and close ejector 
                      // Set target position:
                      ejectorstepper.moveTo(TranslatePos(ejectorabsolute_pos,341));
                      ejectorabsolute_pos += TranslatePos(ejectorabsolute_pos,341);
                      // Run to position 341 with set speed and acceleration:
                      ejectorstepper.runToPosition();                     
                      delay(200);

                      ejectorstepper.moveTo(TranslatePos(ejectorabsolute_pos,0));
                      ejectorabsolute_pos += TranslatePos(ejectorabsolute_pos,0);
                      // Run to position 0 with set speed and acceleration:
                      ejectorstepper.runToPosition();               
                      delay(200);

                      status = 0; // change the status to Printer       
              break;
    }
    break;
  }

}

int TranslatePos(int src_pos, int dest_pos){
     return (dest_pos-src_pos);

} 

