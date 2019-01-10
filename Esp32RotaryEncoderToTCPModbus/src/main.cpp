#include <Arduino.h>
#include "RotaryEncoder.h"
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>

RotaryEncoder encoder;
volatile unsigned int RotaryEncoder::encoderPos = 0;


void initRotaryEncoder()
{
  pinMode(encoder0PinA , INPUT_PULLUP);
  pinMode(encoder0PinB , INPUT_PULLUP);
  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt (digitalPinToInterrupt(encoder0PinA), encoder.doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt (digitalPinToInterrupt(encoder0PinB), encoder.doEncoderB, CHANGE);
}

// Modbus Registers Offsets (0-9999)
const int TEST_HREG = 0;

//ModbusIP object
ModbusIP mb;

void initializeModbus()
{
  mb.begin();
  mb.addHreg(TEST_HREG, 100);
}

void connectToWifi()
{
  WiFi.begin("khwelsABU8", "FKPIFURCSZKKXHRZKSV3");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() 
{
  #ifdef ESP8266
  Serial.begin(74880);
  #else
  Serial.begin(115200);
  #endif 
  
  initRotaryEncoder();
  initializeModbus();
}

void loop() 
{
  mb.task();
  mb.Hreg(TEST_HREG, RotaryEncoder::encoderPos);
  delay(200);
}