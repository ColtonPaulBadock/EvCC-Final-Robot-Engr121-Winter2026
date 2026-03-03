/**
 * 
 * 
 * Driver for our encoder system. This file holds utilities
 * to measure the position of our encoders while driving.
 * 
 * By Colton Paul Badock
 * 3/2026
 */

#include <Arduino.h> //Arduino default library


/**
 * 
 * Encoder controller!
 * "counter" increments twice per each tick
 */
class encoder {
    private:
        byte outputA;
        byte outputB;

    public:
        //The total rotations counted
        int counter = 0;    

        //
        int aState;
        int aLastState;  

        //The constructor to get the pins for the encoder
        //takes in the pins we will use to read the rotary encoder
        encoder(byte a, byte b) {
            
            //Set our pins for the rotary encoder
            outputA = a;
            outputB = b;

            //Setup the pins
            pinMode(outputA,INPUT);
            pinMode(outputB,INPUT);

            //Get the last state of the rotary encoder
            aLastState = digitalRead(outputA);
        }


        //Monitors the rotations of the encoder. Should be called in a main
        //loop (application loop) to keep updating encoders as we are
        //moving/driving around.
        void monitorRotations() {

            //Reads the current state, if the previous state and this state have
            //changed, then a pulse occured and the encoder likely moved
            aState = digitalRead(outputA); 
            if (aState != aLastState){     
                
                // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
                if (digitalRead(outputB) != aState) { 
                    //Increment encoder
                    counter ++;
                } else {
                    //Decrement the encoder if we went backwards
                    counter --;
                }
            }
            
            aLastState = aState; // Updates the previous state of the output.
        }

        //Returns the encoder position in total rotations
        //since our robot started.
        int getPosition() {
            return counter;
        }
};