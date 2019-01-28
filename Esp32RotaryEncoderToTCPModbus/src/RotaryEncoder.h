#ifndef RotaryEncoder_h
#define RotaryEncoder_h
#include "Arduino.h"

const int encoder0PinA  = 25;
const int encoder0PinB  = 26;

union EncoderPosition
{
    long asLong;
    uint16_t asInt[sizeof(long)/2];
};


// Methods and variables in this class have to be static, because they have to be attached to an interrupt and the attachInterrupt() fuction does not support non-static methods and variables.
// For creating a new instance this class must be copied.
class RotaryEncoder
{
        
    public:
        static void (*function_callback)(void);

        static volatile EncoderPosition encoderPos;
       
        static void doEncoderA()
        {
            // look for a low-to-high on channel A
            if (digitalRead (encoder0PinA) == HIGH)
            {
                // check channel B to see which way encoder is turning
                if (digitalRead (encoder0PinB) == LOW)
                {
                encoderPos.asLong = encoderPos.asLong  + 1; // CW
                }
                else
                {
                encoderPos.asLong = encoderPos.asLong  - 1; // CCW
                }
            }
            else
            {
                // must be a high-to-low edge on channel A
                // check channel B to see which way encoder is turning
                if (digitalRead (encoder0PinB) == HIGH)
                {
                encoderPos.asLong = encoderPos.asLong  + 1; // CW
                }
                else
                {
                encoderPos.asLong = encoderPos.asLong  - 1; // CCW
                }
            }
            if(function_callback != NULL) {
		        function_callback();
            }
        } 
        static void doEncoderB()
        {
            // look for a low-to-high on channel B
            if (digitalRead (encoder0PinB) == HIGH)
            {
                // check channel A to see which way encoder is turning
                if (digitalRead (encoder0PinA) == HIGH)
                {
                encoderPos.asLong = encoderPos.asLong  + 1; // CW
                }
                else
                {
                encoderPos.asLong = encoderPos.asLong  - 1; // CCW
                }
            }
            // Look for a high-to-low on channel B
            else
            {
                // check channel B to see which way encoder is turning
                if (digitalRead (encoder0PinA) == LOW)
                {
                encoderPos.asLong = encoderPos.asLong  + 1; // CW
                }
                else
                {
                encoderPos.asLong = encoderPos.asLong  - 1; // CCW
                }
            }
            if(function_callback != NULL) {
		        function_callback();
            }      
        }
};

#endif /* RotaryEncoder_h */

