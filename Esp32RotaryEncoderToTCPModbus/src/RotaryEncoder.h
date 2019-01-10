#ifndef RotaryEncoder_h
#define RotaryEncoder_h


const int encoder0PinA  = 25;
const int encoder0PinB  = 26;

class RotaryEncoder
{
    public:
        static volatile unsigned int encoderPos;
       
        static void doEncoderA()
        {
            // look for a low-to-high on channel A
            if (digitalRead (encoder0PinA) == HIGH)
            {
                // check channel B to see which way encoder is turning
                if (digitalRead (encoder0PinB) == LOW)
                {
                encoderPos = encoderPos + 1; // CW
                }
                else
                {
                encoderPos = encoderPos - 1; // CCW
                }
            }
            else
            {
                // must be a high-to-low edge on channel A
                // check channel B to see which way encoder is turning
                if (digitalRead (encoder0PinB) == HIGH)
                {
                encoderPos = encoderPos + 1; // CW
                }
                else
                {
                encoderPos = encoderPos - 1; // CCW
                }
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
                encoderPos = encoderPos + 1; // CW
                }
                else
                {
                encoderPos = encoderPos - 1; // CCW
                }
            }
            // Look for a high-to-low on channel B
            else
            {
                // check channel B to see which way encoder is turning
                if (digitalRead (encoder0PinA) == LOW)
                {
                encoderPos = encoderPos + 1; // CW
                }
                else
                {
                encoderPos = encoderPos - 1; // CCW
                }
            }      
        }
};

#endif /* RotaryEncoder_h */

