//This is a description


#include <Arduino.h>
#include <Wire.h>

#define encoder0PinA 2 
#define encoder0PinB 3

volatile long encoder0Pos = 0;
volatile bool isThereANeedToTransfer = false;

void transferI2C();

void setup() 
{ 
    Wire.begin(10);
    Wire.onRequest(transferI2C);

    pinMode(encoder0PinA, INPUT);
    pinMode(encoder0PinB, INPUT);

    
    void doEncoderA();
    void doEncoderB();
    // encoder pin on interrupt 0(pin 2)
    attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, CHANGE);
    // encoder pin on interrupt 1(pin 3)
    attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderB, CHANGE); 
    
    Serial.begin(9600);
}

void loop()
{
    //delay(1);
    //Serial.println(micros());
    //Serial.print("\t");
    //Serial.println(encoder0Pos, DEC); 
    //angle = encoder0Pos *(3.0/20.0); 
    //Serial.println(angle);
}

void transferI2C()
{
    Wire.write(encoder0Pos);
    encoder0Pos = 0;
    //Serial.println("T"/*Transfer*/);
}

void doEncoderA()
{
    // look for a low-to-high on channel A 
    if(digitalRead(encoder0PinA) == HIGH) {
    // check channel B to see which way encoder is turning
        if(digitalRead(encoder0PinB) == LOW) 
        {
            encoder0Pos = encoder0Pos + 1; // CW
        }
        else 
        {
            encoder0Pos = encoder0Pos - 1; // CCW
        }    
    }
    else
    { // must be a high-to-low edge on channel A
    // check channel B to see which way encoder is turning 
        if(digitalRead(encoder0PinB) == HIGH) 
        {
        encoder0Pos = encoder0Pos + 1; // CW 
        }
        else 
        {
        encoder0Pos = encoder0Pos - 1; // CCW
        } 
    }
    isThereANeedToTransfer = true;
}

void doEncoderB()
{
    // look for a low-to-high on channel B 
    if(digitalRead(encoder0PinB) == HIGH) {
    // check channel A to see which way encoder is turning
        if(digitalRead(encoder0PinA) == HIGH) 
        { 
            encoder0Pos = encoder0Pos + 1; // CW
        }
        else 
        {
            encoder0Pos = encoder0Pos - 1; // CCW 
        }
    }
    // Look for a high-to-low on channel B
    else {
        // check channel B to see which way encoder is turning 
        if(digitalRead(encoder0PinA) == LOW) {
            encoder0Pos = encoder0Pos + 1; // CW 
        }
        else {
            encoder0Pos = encoder0Pos - 1; // CCW
        } 
    }
    isThereANeedToTransfer = true;
}

