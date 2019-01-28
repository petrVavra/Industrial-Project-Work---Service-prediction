#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ModbusTCPSlave.h"


ModbusTCPSlave Mb;

// D0
#define encoder0PinA 4
// D1
#define encoder0PinB 5

volatile long encoder0Pos = 0;

void setup()
{
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);


  void doEncoderA();
  void doEncoderB();
  // encoder pin on interrupt 0(pin 2)
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, CHANGE);
  // encoder pin on interrupt 1(pin 3)
  attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderB, CHANGE);

  Serial.begin(115200);
  Mb.begin("khwelsABU8", "FKPIFURCSZKKXHRZKSV3");

  delay(100);

}

void loop()
{
  Mb.Run();
  delay(10);
  Serial.print(encoder0Pos);
  Serial.print(" ");
  Serial.print(Mb.MBInputRegister[0]);
  Serial.print(" "); 
  Serial.println(Mb.MBInputRegister[1]);
}

void doEncoderA()
{
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW)
    {
      encoder0Pos = encoder0Pos + 1;
      Mb.MBInputRegister[0] = Mb.MBInputRegister[0] + 1;
    }
    else
    {
      encoder0Pos = encoder0Pos - 1;
      Mb.MBInputRegister[1] = Mb.MBInputRegister[1] + 1;
    }
  }
  else
  { // must be a high-to-low edge on channel A
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == HIGH)
    {
      encoder0Pos = encoder0Pos + 1;
      Mb.MBInputRegister[0] = Mb.MBInputRegister[0] + 1;
    }
    else
    {
      encoder0Pos = encoder0Pos - 1;
      Mb.MBInputRegister[1] = Mb.MBInputRegister[1] + 1;
    }
  }
}

void doEncoderB()
{
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {
    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH)
    {
      encoder0Pos = encoder0Pos + 1;
      Mb.MBInputRegister[0] = Mb.MBInputRegister[0] + 1;
    }
    else
    {
      encoder0Pos = encoder0Pos - 1;
      Mb.MBInputRegister[1] = Mb.MBInputRegister[1] + 1;
    }
  }
  // Look for a high-to-low on channel B
  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {
      encoder0Pos = encoder0Pos + 1;
      Mb.MBInputRegister[0] = Mb.MBInputRegister[0] + 1;
    }
    else {
      encoder0Pos = encoder0Pos - 1;
      Mb.MBInputRegister[1] = Mb.MBInputRegister[1] + 1;
    }
  }

}

