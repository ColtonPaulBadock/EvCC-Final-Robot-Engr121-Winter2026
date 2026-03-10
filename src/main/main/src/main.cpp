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
byte trigS = 7; //ORIGINAL: 9
byte trigL = 3;
byte trigR = 5; //ORGINAL: 6

 //define echo pins
byte echoS = 6; //ORIGINAL: 8
byte echoL = 2;
byte echoR = 4; //ORGINAL: 5

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


/*
JACKSONS Driver Variables (Motor driver)
*/
const byte bin1 = 8;
const byte bin2 = 9;
const byte pnwb = 10; 
const byte al1 = 13;
const byte al2 = 12;
const byte pnwa = 11;  
// button variables - temp input before sensors are inplace
const byte leftb = 2;
const byte rightb = 3;
// potentiometer analog pin
const byte pot = 0;
unsigned int potVal;
int goFast;

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

  //JACKSONS Motor Driver Intialization
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(pnwb, OUTPUT);
  pinMode(al1, OUTPUT);
  pinMode(al2, OUTPUT);
  pinMode(pnwa, OUTPUT);
  pinMode(leftb, INPUT_PULLUP);
  pinMode(rightb, INPUT_PULLUP);

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


//Nolan McGuire + Colton Paul Badock
//Updates the radar positions of each radar axis so we know whats around us
void updateRadarPositions() {
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



int lastPos = 0;

//Main application loop, runs repeatidly
void loop() {

  //Monitor the rotations of the left encoder on the left tank
  //tread; Update the counter
  //leftEncoder.monitorRotations();

  //Update all our radar data, so we know whats around
  updateRadarPositions();
}



