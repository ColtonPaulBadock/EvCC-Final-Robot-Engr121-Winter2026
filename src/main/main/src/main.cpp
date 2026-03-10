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
//unsigned int potVal;
int goFast = 60;

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


void motorA(int motorSpeed)           //  function for driving motor A; input -255 to 255
{
  if (motorSpeed > 0)                 //  if the motor is given a positive speed; "put the car in drive"
  {
    digitalWrite(bin1, HIGH);         //  set pin 1 to high; drive forward
    digitalWrite(bin2, LOW);          //  set pin 2 to low; don't drive backward
  }
  else if (motorSpeed < 0)            //  if the motor is given a negative speed; "put the car in reverse"
  {
    digitalWrite(bin1, LOW);          //  set pin 1 to low; don't dive forward
    digitalWrite(bin2, HIGH);         //  set pin 2 to high; drive backward
  }
  else                                //  if the motor is given a speed of zero; "put the car in park"
  {
    digitalWrite(bin1, LOW);          //  set pin 1 to low;  don't drive forwad
    digitalWrite(bin2, LOW);          //  set pin 2 to low; don't drive backward
  }
  analogWrite(pnwb, abs(motorSpeed)); //  now that the motor direction is set, drive it at the entered speed
}

void motorB(int motorSpeed)                           
{
  if (motorSpeed > 0)                 //  sets the motor controller to forward direction motion                             
  {
    digitalWrite(al1, HIGH);                         
    digitalWrite(al2, LOW);                         
  }
  else if (motorSpeed < 0)            //  sets motor controller to backwards direction               
  {
    digitalWrite(al1, LOW);                          
    digitalWrite(al2, HIGH);                         
  }
  else                                //  sets motor controller to no motion                
  {
    digitalWrite(al1, LOW);                          
    digitalWrite(al2, LOW);                          
  }
  analogWrite(pnwa, abs(motorSpeed)); //  sends voltage to motor to make spin                
}

// These are functions to control the motors (movement) of the robot

     void halt()
     {
      motorA(0);
      motorB(0);
     }

     void goStraight()
    {
      motorA(goFast);
      motorB(goFast);
    }

    void reverse()
    {
      motorA(goFast);
      motorB(goFast);
    }

    void turnLeft()
    {
      motorA(goFast);
      motorB(goFast);
    }

    void turnRight()
    {
      motorA(goFast);
      motorB(goFast);
    }

    void leftPiv()
    {
      motorA(goFast);
      motorB(-1*goFast);
    }

    void rightPiv()
    {
      motorA(-1*goFast);
      motorB(goFast);
    }


// This function reads the potentiometer and uses it to set speed or reverse
/*unsigned int selectSpd()
{
 potVal = analogRead(pot);

  // "Forward" Moving Gears
   if(potVal >= 171 && potVal < 341)
   {
    goFast = -51;
   }
   if(potVal >= 341 && potVal < 511)
   {
    goFast = -102;
   }
   if(potVal >=511 && potVal < 682)
   {
    goFast = -153;
   }
   if(potVal >=682 && potVal < 852)
   {
    goFast = -204;
   }
   if(potVal >=852 && potVal <= 1023)
   {
    goFast = -255;
   }

  // Reverse Gear "one speed reverse"
   if(potVal < 171)
   {
    goFast = 255;
   }

   return(goFast);
}*/



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

  //Drive straight for test
  if (distS < 4) {
    halt();
    motorA(goFast);
    delay(1000);
    halt();
  } else {
    goStraight();
  }
}



