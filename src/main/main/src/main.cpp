#include <Arduino.h>
#include <Wire.h>
#include "robot.h"
#include "colorManager.h"
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
color_sensor cs;


//Runs on intialization once
void setup() {
  //Initalizes the robot
  initalizeRobot();

  //Intializes the color sensor (if needed)
  cs.initColorSensor();

}



//Main application loop, runs repeatidly
void loop() {
  delay(100);
  cs.getColor();
}




