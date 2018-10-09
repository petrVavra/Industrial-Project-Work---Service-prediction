# Industrial Project Work - Service prediction

Aim of this project is to predict a failure of industrial comvia belt. On the way to make this thing real we will try to make a protytype of clever sensor, which can easily transfer data to OPC standard.

This project is done by Petr Vávra and Cecilia Fili under direction of Thomas Schichl in course of Industrial Project Work at the University of Applied Sciences Upper Austria in Winter semester 2018/19.

# Sensors and actuators:

* Rotary encoder - LPD-3086
* Temperature sensor - DHT11? DS18B20?
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
* consider who should evaluate data, make evaluation and predictive algorithms and transfer to top level PLC.,
    * this shoul probably be Raspberry PI, than there should be only tiny data line to PLC,
* consider adding other sensor,
    * new vibration sensor, misalignment sensor (ultrasonic).



