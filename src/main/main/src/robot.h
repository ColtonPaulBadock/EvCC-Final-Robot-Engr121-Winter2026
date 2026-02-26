#include <Arduino.h>
/*
Here are variables, classes and resources for operating
our robot.

Colton Paul Badock - 2/25/2026
*/

//Vector 2 position/corrdinates supporting float
//type. Use this as a container to store any Vector2
//corrdinate!
struct Vector2f {

    float x;
    float y;

    Vector2f(float initial_x = 0.0f, float initial_y = 0.0f) : x(initial_x), y(initial_y) {}
};


//Intializes our robot, called from "setup()" inside main.cpp,
//but can be recalled for reintializing.
void initalizeRobot() {

    //Begin default serial communications to the master PC/Device
    //at 9600 bit/s
    Serial.begin(9600);

    //Start I2C communications
    Wire.begin();
}