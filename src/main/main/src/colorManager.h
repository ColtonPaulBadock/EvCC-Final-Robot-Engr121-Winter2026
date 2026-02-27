/**
 * 
 * This system is used to detect color (RGB) values from beneth the robot so
 * we can detect which room it has entered.
 * Holds several utilities to manage and utilize our onboard color sensor
 */
#include <Arduino.h>


 class color_sensor {
    private:
        const int out = 9;
        const int s0 = 7;
        const int s3 = 8;
        const int s2 = 10;
        const int s1 = 11;
        const int e0 = 12;

    public:

    void initColorSensor() {

        //Log that we are intializing the color sensor
        Serial.println("Color Sensor intializing...");

        //Set the systems frequency/sensitivty to light
        digitalWrite(s0, LOW);
        digitalWrite(s1, LOW);

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

        //The color values detected by the
        //color sensor at the time of "getColor()" (this function)
        //is called
        byte red = 0;
        byte blue = 0;
        byte green = 0;

        //The color code we detect; Related to "COLOR CODES" at the
        //top of this function!
        int color_detected = 0;


        //Here we get color readings for RGB (Red, Green, Blue)
        //of the color sensor.
        digitalWrite(s2, LOW);
        digitalWrite(s3, LOW);
        red = pulseIn(out, LOW); // Reading RED component of color
        //--------
        digitalWrite(s2, HIGH);
        digitalWrite(s3, HIGH);
        green = pulseIn(out, LOW); // Reading GREEN component of color
        //--------
        digitalWrite(s2, LOW);
        digitalWrite(s3, HIGH);
        blue = pulseIn(out, LOW); // Reading BLUE component of color

        Serial.print(red);
        Serial.print(", ");
        Serial.print(green);
        Serial.print(", ");
        Serial.print(blue);
        Serial.println("");


        //Returns the color code we detected by the color
        //sensor on board. The color codes are declared at the top of
        //this funciton in a comment.
        return color_detected;
    }

 };