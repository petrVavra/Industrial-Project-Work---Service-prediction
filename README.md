# Industrial Project Work - Service prediction

Aim of this project is to predict a failure of industrial comvia belt. On the way to make this thing real we will try to make a protytype of clever sensor, which can easily transfer data to OPC standard.

This project is done by Petr Vávra and Cecilia Fili under direction of Thomas Schichl in course of Industrial Project Work at the University of Applied Sciences Upper Austria in Winter semester 2018/19.

# Sensors and actuators:

* Rotary encoder - LPD-3086
* Temperature sensor - DHT11 (currenty in use) DS18B20 (considered replacement)
* Vibration sensor - 

# Architecture
```
            I2C?
Arduino1    --->
Arduino2    --->                          Codesys 
.           --->        Raspberry PI        --->    PLC
.           --->
ArduinoX    --->
```
# Way to target:

* make sensors running with Arduino,
* set up a communication protocol between Arduino and Raspberry Pi,
    * decide, which connection is the best - Serial, I2C, consider others,
    * design a way how to transfer data (a binary protocol, ...), espacialy consider and test period,
        * who should measure period?,
* make a way how to transfer data to PLC,
    * try a ne
* consider who should evaluate data, make evaluation and predictive algorithms and transfer to top level PLC,
    * this shoul probably be Raspberry PI, than there should be only tiny data line to PLC,
* consider adding other sensor,
    * new vibration sensor, misalignment sensor (ultrasonic).

# Current development archutecture and source code description
```
                    I2C             Codesys 
Rotary Enc  --->    Arduino1    --->                                    (localhost)
Temp+Hud    --->    Arduino2    --->    Raspberry Pi      --->    WebBroser Visualization       
```
## Source code

* Arduino1 - RotaryEncoder folder source - written in PlatformIO IDE
* Arduino2 - TemperatureDHT11 folder source - written in PlatformIO IDE
* Raspberry Pi - Rasbperry Pi - written in Codesys

## I2C tranfer data

* Arduino1 
    * 4 bytes - type C long (counted movement of encoder, sign of the value marks the direction)
    * Whenever is the value transfered, then the counter of movement is reseted to 0
* Arduino2 
    * 2 bytes - type C uint8_t (temperature [°C]), type C uint8_t (hudimity [%])


