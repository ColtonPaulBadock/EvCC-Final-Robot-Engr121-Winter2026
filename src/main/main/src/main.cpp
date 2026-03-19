/*Whole Code Set v5
 * Date: 3/18/2026
 * AUTHORS:
 * Colton Paul Badock
 * Nolan McGuire
 * Jackson Black
 * Julio Martinez
 * 
 * Description: Use fixed left dist. sensor. 
 */
//                                    Libraries/headers
#include <Arduino.h>
#include <Wire.h>
#include <AccelStepper.h> 

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

#define s0 49        //Module pins  wiring
#define s1 37
#define s2 25
#define s3 29
#define out 27

int  Red=0, Blue=0, Green=0;  //RGB values 

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
byte trigS = 52; //ORIGINAL: 9
byte trigL = 50;
byte trigR = 5; //ORGINAL: 6

 //define echo pins
byte echoS = 53; //ORIGINAL: 8
byte echoL = 51;
byte echoR = 4; //ORGINAL: 5

 //create distance variables
float distS;
float distL;
float distR;


//Object detection limits
//In inches; Distance to detect objects
byte ahead = 8;
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
int goFast = 120;

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

  //Intialize color sensor
  pinMode(s0,OUTPUT);    //pin modes
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);

  //The button for the front bumper inpact detection
  pinMode(bumperButton, INPUT_PULLUP);

  //ping pong ball launcher
  //stepper motor
  stepper.setMaxSpeed(Mxspd);       //max speed
  stepper.setAcceleration(accel);   //acceleration
  stepper.setSpeed(spd);            //actual speed

}

void GetColors()
{
  digitalWrite(s2,  LOW);
  digitalWrite(s3, LOW);
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);
  digitalWrite(s3, HIGH);
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW  : HIGH);
  delay(20);
  digitalWrite(s2, HIGH);
  Green = pulseIn(out,  digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);
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
      motorA(-1*goFast);
      motorB(goFast);
    }

    void rightPiv()
    {
      motorA(goFast);
      motorB(-1*goFast);
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




//Boolean for an auto action.
//We will skip checking distances if we are in an action in "runAuto()", (atleast for setting our actions).
//-------------
//VALUES:
//0 -> not in action
//1 -> in an action
bool auto_action_active = 0;
//-----------------
//-----------------
//-----------------
//List of our actions, these are the actions our robot can perform.
//-----
//"auto_action_turnLeft" -> If true, we are turning left, else if, we are not in the action
//"auto_action_turnRight" -> If true, we are turning right, else if, we are not in the action
bool auto_action_turnLeft = 0;
int auto_action_turnLeftTime = 500;
bool auto_action_turnRight = 0;
int auto_action_turnRightTime = 500;
//The timer for running actions
ColtonTimerSystem actionRunTime;
bool firstRun = 1;
//The threshold for detecting ourselves scraping against a wall while driving
//straight, we will pivot off of it if detected within this threashold
//from the left or right sensor
int threshhold_scraping_wall = 2;

/*
Colton Paul Badock; Nolan McQuire
------------------

Robots autonomous function, here we will run auto
for the robot based on sensor values like "distS", "distL", etc
*/
void runAuto() {

  //If we are actively in an action, we will run said action here
  if (auto_action_active == 1) {

    //If its action "auto_action_turnLeft"; We will run it right here
    if (auto_action_turnLeft == 1 && firstRun == 1) {
      //Pivot left until our timer is more than "action_turnLeft_time"
      leftPiv();
      actionRunTime.startTimer();
      firstRun = 0; //Set "firstRun" status to false, so we know
      //we already ran this action atleast once, we will set this back
      //to true for the next action once this one is complete.
      Serial.println("Started turning left");


    //Once the action is complete, reset us to keep checking for
    //driving forward at "ID: 78293474"
    } else if (auto_action_turnLeft == 1 && actionRunTime.getTime() > auto_action_turnLeftTime) {
      
      Serial.println("stopped turning left");
      Serial.println(actionRunTime.getTime());


      //Stop all motors, then set our specific action "auto_action_turnLeft" to false,
      //so we know we are no longer turning left. Then set our active action status
      //"auto_action_active" to false, so we know we are not running an action, and can
      //perform regular auto operations driving forward at "ID: 78293474".
      //Finally, set our first run status to true, so we can perform our first
      //run actions on the next action for the first time we run said action.
      halt();
      auto_action_turnLeft = 0;
      auto_action_active = 0;
      firstRun = 1;
    } else if (auto_action_turnRight == 1  && firstRun == 1) {
      //Pivot right until our timer is more than "action_turnRight_time"
      rightPiv();
      actionRunTime.startTimer();
      firstRun = 0;//Set "firstRun" status to false, so we know
      //we already ran this action atleast once, we will set this back
      //to true for the next action once this one is complete.
      Serial.println("Started turning right");


    //Once the action is complete, reset us to keep checking for
    //driving forward at "ID: 78293474"
    } else if (auto_action_turnRight == 1 && actionRunTime.getTime() > auto_action_turnRightTime) {

      Serial.println("stopped turning right");

      //Stop all motors, then set our specific action "auto_action_turnRight" to false,
      //so we know we are no longer turning right. Then set our active action status
      //"auto_action_active" to false, so we know we are not running an action, and can
      //perform regular auto operations driving forward at "ID: 78293474".
      //Finally, set our first run status to true, so we can perform our first
      //run actions on the next action for the first time we run said action.
      halt();
      auto_action_turnRight = 0;
      auto_action_active = 0;
      firstRun = 1;
    }

  }

  //"ID: 78293474"
  //If the front is clear for "ahead" amount of inches,
  //we perform this logic
  //------
  //If an auto action is active, we will not check our sensors, since
  //we are actively performing and action elsewere.
  if (distS > ahead && auto_action_active == 0) {

    //Travel in the straight direction while the front is clear
    //for "ahead" amount of inches
    goStraight();

    //If we are scraping any wall, get off the wall by pivoting
    //away from it
    if (distL < threshhold_scraping_wall) {
      rightPiv();
    } else if (distR < threshhold_scraping_wall) {
      leftPiv();
    }


    //If we see that the distS (distance in front) is less than the "ahead"
    //threshold, we will park the robot and make a descision.
  } else if (distS <= ahead && auto_action_active == 0) {

    //Park the robot 
    halt();

    //Check the side distances, depending on which on is 
    //greater, we will set our action to turn to the side with the
    //most persumed distance
    if (distL > distR) {
      auto_action_active = 1;
      auto_action_turnLeft = 1;
    } else if (distL < distR) {
      auto_action_active = 1;
      auto_action_turnRight = 1;
    //If the values are equal or its unclear, we will pick a random direction to turn
    } else {

      //Randomly pick a direction to turn//
      if (random(1, 10) > 5) {
        auto_action_active = 1;
        auto_action_turnLeft = 1;
      } else {
        auto_action_active = 1;
        auto_action_turnRight = 1;
      }
    }

  }

}


//Shoots the launcher during auto if applicable
void shootLauncher() {

  //Update the colors
  GetColors();

  //If we see red, shoot the launcher
  if (Red < 60) {
    ROTATE(HALF_ROTATION);
  }

}


//Main application loop, runs repeatidly
void loop() {

  //Checks all the sensors on board the robot, updating us with the lastest
  //values before all autonomous logic
  distS = getSensorDistance(trigS, echoS);
  distL = getSensorDistance(trigL, echoL);
  distR = getSensorDistance(trigR, echoR);
  

  //Run our autonomous
  runAuto();
  
  //Shoot the launcher during auto
  shootLauncher();

}



