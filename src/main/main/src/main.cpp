//Default arduino libraries/headers
#include <Arduino.h>
#include <Wire.h>
#include <AccelStepper.h> 


//#include "robot.h" //Our robot system
//#include "colorManager.h" //Color sensor system
//#include "encoders.h" //Encoder system
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


//Allows the user to run a timer
//using the default Arduino timer system.
class ColtonTimerSystem {
  private:

    //This will hold the total time
    //that has passed since the timer
    //started.
    long timerStarted = 0;
  
  public:

    //Starts the timer;
    //Can be called to reset the timer
    void startTimer() {
      //Store the current milliseconds
      //since the board started.
      timerStarted = millis();
    }

    long getTime() {
      return millis() - timerStarted;
    }
};

//ping pong ball launcher
//ping pong ball launcher Nolan McGuire
//define motor driver pins
#define FULLSTEP 4
#define STEP_PER_REVOLUTION 2048 //sets the 

byte INONE = 22;
byte INTWO = 24;
byte INTHREE = 26;
byte INFOUR = 28;

//motor settings
int accel = 1000;
int Mxspd = 300.00;
int spd = 100;

//position variables
int FULL_ROTATION = 2048; //the number of steps/position value of one full rotation
int HALF_ROTATION = 1024; //the number of steps/position value of one half rotation
int NO_ROTATION = 0;      //the position value of start

int bumperButton = 41;

AccelStepper stepper(FULLSTEP, INONE, INTHREE, INTWO, INFOUR);  //telling the library which pins to use

//Timer for autonomous system
ColtonTimerSystem autoTimer;


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

//Autonomus variables.
bool turningAround = 0; //If we are turning around, this is 1, else 0.


//COLTON PAUL BADOCK
//Runs on intialization once
void setup() {
  //Initalizes the robot
  //Begin default serial communications to the master PC/Device
    //at 9600 bit/s
    Serial.begin(9600);

    //Start I2C communications
    Wire.begin();

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


  //The button for the front bumper inpact detection
  pinMode(bumperButton, INPUT_PULLUP);

  //Intializes the color sensor (if needed)
  //cs.initColorSensor();

  //ping pong ball launcher
  //stepper motor
  stepper.setMaxSpeed(Mxspd);       //max speed
  stepper.setAcceleration(accel);   //acceleration
  stepper.setSpeed(spd);            //actual speed

}


//Returns true or false if the front bumper is pressed.
bool detectFrontBumper() {

  //Read the digital bumperButton pin,
  //return true if we detect a standing voltage
  //from it.
  if (digitalRead(bumperButton) == 0) {
    return true;
  }

  //If we determined the front bumper was not pressed,
  //we will return false.
  return false;
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

//ping pong ball launcher Nolan McGuire

void ROTATE(int dist)
{
  stepper.move(dist);       //move the stepper 1/2 rotation 
}
void CORRECTPOS(int TPOS)
{
  int CPOS = stepper.currentPosition();
  if(CPOS > TPOS)           //if the motor overshoots target position
  {
    stepper.moveTo(TPOS);   //move back to target position
    stepper.run();
    stepper.setCurrentPosition(0);   //redefine position as 0
  } 
}


//NOLAN MCGUIRE + COLTON PAUL BADOCK
//straight distance function
float getSensorDistance(byte trig, byte echo)
{
  digitalWrite(trig, HIGH);               //send out ping
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  float interval = pulseIn(echo, HIGH);         //listen for return ping

  float dist = interval / 148;            //convert to inches
  if (dist != 0)
  {
  return dist;                            //return only non-zero answers
  }
}




//Runs the autonomous code so the robot can navigate.
/*void runAuto() {

  //If something is within 4 inches of the front of the robot, we will begin turning to the left
  if (distS < 4 && turningAround == 0) {

    //Stop the robot, log that we are turning around.
    halt();
    turningAround = 1;
    autoTimer.startTimer();

  } else if (turningAround == 1 && autoTimer.getTime() < 1000) {
    turnLeft();
  } else if (turningAround == 1) {
    halt();
    turningAround = 0;
  }
}*/



int lastPos = 0;

//Main application loop, runs repeatidly
void loop() {



  //Runs the auto system
  //runAuto();
  distS = getSensorDistance(trigS, echoS);
  Serial.print("ahead: ");
  Serial.println(distS);
  distL = getSensorDistance(trigL, echoL);
  Serial.print("left: "); 
  Serial.println(distL);
  distR = getSensorDistance(trigR, echoR);
  Serial.print("right: ");
  Serial.println(distR); 
  
  if (distS < 6 || detectFrontBumper())
  {
    leftPiv();
  }
  if (distS >= 6)
  {
  goStraight();
  }
  //Serial.println(getSensorDistance(trigL, echoL));
  //Serial.println(getSensorDistance(trigR, echoR));
  
  if (distS > 25)                     //if pressed
  {
  ROTATE(HALF_ROTATION); 
  }
  stepper.run();
  Serial.println(stepper.currentPosition());
  CORRECTPOS(HALF_ROTATION);


  //Detect front bumper impact
  if (detectFrontBumper() == true) {
    Serial.println("DETECTED FRONT BUMPER!");
  }
}

  