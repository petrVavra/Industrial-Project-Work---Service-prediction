#include <Arduino.h>
#include <ESP8266WiFi.h>
#include ".\ModbusTCPSlave\ModbusTCPSlave.h"


//WIFI Settings
//byte ip[]      = { 192, 168, 1, 126};
//byte gateway[] = { 192, 168, 1, 1 };
//byte subnet[]  = { 255, 255, 255, 0 };

//Ticker Parada;
ModbusTCPSlave Mb;


void generate() {

      /// Send random - envio random
      for (int i = 0; i<10; i++)
      {
        Mb.MBInputRegister[i] = i+1;
        Mb.MBHoldingRegister[i] = i+1;

        Serial.print("MBInputRegister ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(Mb.MBInputRegister[i]);

        Serial.print("MBHoldingRegister ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(Mb.MBHoldingRegister[i]);
      }


}

void tell()
{
  for (int b = 0; b<10; b++)
      {
        Serial.print(b);
        Serial.print(" ");
        Serial.println(Mb.MBInputRegister[b]);

 
        Serial.print(b);
        Serial.print(" ");
        Serial.println(Mb.MBHoldingRegister[b]);
      }
}

void setup()
{

    Serial.begin(115200);  
    Mb.begin("khwelsABU8", "FKPIFURCSZKKXHRZKSV3");
//  Mb.begin("SSID", "PASSWORD", ip, gateway, subnet);
    generate();
    delay(100);

}

void loop()
{
  Mb.Run();
  delay(10);
 
  
}
