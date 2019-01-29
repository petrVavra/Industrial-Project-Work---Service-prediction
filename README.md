# Industrial Project Work - Service prediction

Aim of this project is to bring with a new cheaper way how to bring data from sensors and actuators to OPC UA. 

It will be done by interconnecting sensors to microcontroller (ESP8266 or ESP32) and then transfering data over Wi-fi to Raspberry PI, which will be running [Codesys Control for Raspberry PI](https://store.codesys.com/codesys-control-for-raspberry-pi-sl.html) and/or  [Node-Red](https://nodered.org). 

This project is done by Petr Vávra and Cecilia Fili and supervised by Thomas Schichl in course of Industrial Project Work at the University of Applied Sciences Upper Austria in Winter semester 2018/19.

# Sensors and actuators:

* Rotary encoder - LPD-3086
* Temperature sensor - DS18B20 (considered replacement)

# Microcontroller (MCU)

* ESP8266
* ESP32

# Architecture
```
            Wi-Fi
    TCP/IP - Modbus and/or MQTT
MCU[0]  --->                            
MCU[1]  --->                            --->    Codesys visualization    
.       --->        Raspberry PI        --->    OPC UA (Codesys / Node-red)
.       --->                            --->    Node-Red Dashboard for visualization
MCU[n]  --->
```

# Programming enviroments

For programming microcontrollers we use [PlatformIO IDE](https://platformio.org/platformio-ide).

For programming of Raspberry Pi is used [Codesys](https://store.codesys.com/codesys.html?___store=en) and [Rapsberry Pi extension](https://store.codesys.com/codesys-control-for-raspberry-pi-sl.html)


# Folders

The project is devided into the following folders:

* MicrocotrollerSketch
    * sketch written in PlatformIO IDE handling the MCU level
* RaspberryPI
    * sketch written Codesys for Raspberry PI
* documentation
    * includes a link to a written documentation.
