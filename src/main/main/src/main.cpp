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
//encoder leftEncoder(6, 5);
//encoder rightEncoder();

 //define trig pins
byte trigS = 9;
byte trigL = 3;
byte trigR = 6;

 //define echo pins
byte echoS = 8;
byte echoL = 2;
byte echoR = 5;

 //create distance variables
float distS;
float distL;
float distR;

 //create interval variables
float intervalS;
float intervalL;
float intervalR;

//Object detection limits
//In inches; Distance to detect objects
byte ahead = 10;
byte left = 5;
byte right = 5;

//COLTON PAUL BADOCK
//Runs on intialization once
void setup() {
  //Initalizes the robot
  initalizeRobot();

  //trig pinmodes
  pinMode(trigS, OUTPUT);
  pinMode(trigL, OUTPUT);
  pinMode(trigR, OUTPUT);
  //echo pinmodes
  pinMode(echoS, INPUT);
  pinMode(echoL, INPUT);
  pinMode(echoR, INPUT);

  //Intializes the color sensor (if needed)
  //cs.initColorSensor();

}

//NOLAN MCGUIRE
//straight distance function
float AheadDist(byte trig, byte echo, float interval)
{
  digitalWrite(trig, HIGH);               //Let out a pulse
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  interval = pulseIn(echo, HIGH);         //measure the echo time

  float dist = interval / 148;
  if (dist != 0)
  {
  return dist;                            //only return non-zero values
  }
}

//NOLAN MCGUIRE
//left distance function
float LeftDist(byte trig, byte echo, float interval)
{
  digitalWrite(trig, HIGH);               //send out ping
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  interval = pulseIn(echo, HIGH);         //listen for return ping

  float dist = interval / 148;            //convert to inches
  if (dist != 0)
  {
  return dist;                            //return only non-zero answers
  }
}

//NOLAN MCGUIRE
//right distance function
float RightDist(byte trig, byte echo, float interval)
{
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  interval = pulseIn(echo, HIGH);

  float dist = interval / 148;
  if (dist != 0)
  {
  return dist;
  }
}


int lastPos = 0;

//COLTON PAUL BADOCK
//Main application loop, runs repeatidly
void loop() {

  //Monitor the rotations of the left encoder on the left tank
  //tread; Update the counter
  //leftEncoder.monitorRotations();

  //call getdistance funcitons 
  distS = AheadDist(trigS, echoS, intervalS);
  /*Serial.print("ahead ");
  Serial.print(distS);*/

  distL = LeftDist(trigL, echoL, intervalL);
  /*Serial.print("   left ");
  Serial.print(distL);*/

  distR = RightDist(trigR, echoR, intervalR);
  /*Serial.print("   right ");
  Serial.println(distR);*/

  if (distS < ahead)
  {
    Serial.println("there's something in front!!!");
  }
  if (distL < left)
  {
    Serial.println("there's something to the left!!!");
  }
  if (distR < right)
  {
    Serial.println("there's something to the right!!!");
  }

}




