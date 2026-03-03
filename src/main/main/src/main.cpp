//Default arduino libraries/headers
#include <Arduino.h>
#include <Wire.h>

#include "robot.h" //Our robot system
#include "colorManager.h" //Color sensor system
#include "encoders.h" //Encoder system
/*
The main source code for a robot for Project 3 in ENGR121 @ EvCC.

AUTHORS:
Colton Paul Badock
Nolan McGuire
Jackson Black
Julio Martinez

Created: 2/25/2026
Version: 0
*/


//Easily make a Vector2f on the FLY!
//Vector2f test(10.0f, 124563.7f);

//Our color sensor
//color_sensor cs;

//The encoders (as if we are looking from the back of the tank/robot
//forward)
encoder leftEncoder(6, 5);
//encoder rightEncoder();


//Runs on intialization once
void setup() {
  //Initalizes the robot
  initalizeRobot();

  //Intializes the color sensor (if needed)
  //cs.initColorSensor();

}


int lastPos = 0;

//Main application loop, runs repeatidly
void loop() {

  //Monitor the rotations of the left encoder on the left tank
  //tread; Update the counter
  leftEncoder.monitorRotations();

}




