/**
 * 
 * This system is used to detect color (RGB) values from beneth the robot so
 * we can detect which room it has entered.
 * Holds several utilities to manage and utilize our onboard color sensor
 */
#include <Arduino.h>


 class color_sensor {
    public:

    void initColorSensor() {

        //Log that we are intializing the color sensor
        Serial.println("Color Sensor intializing...");


        return;
    }

    //Returns the color seen by the color sensor.
    //
    //COLOR CODES:
    // 0 -> Unregonized color
    // 1 -> red
    // 2 -> blue
    // 3 -> green
    int getColor() {

        //The color code we detect; Related to "COLOR CODES" at the
        //top of this function!
        int color_detected = 0;

        //Returns the color code we detected by the color
        //sensor on board. The color codes are declared at the top of
        //this funciton in a comment.
        return color_detected;
    }

 };