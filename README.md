# Industrial Project Work - Service prediction

Aim of this project is to bring with a new cheaper way how to bring data from sensors and actuators to OPC UA. 

It will be done by inerconnecting sensors to microcontroller (ESP8266) and then transfering data over Wi-fi to Raspberry PI, which will be running [Codesys Control for Raspberry PI](https://store.codesys.com/codesys-control-for-raspberry-pi-sl.html). 

Furthure development will be concerned about interpertting data from sensors and trying to make some clever predictions. 

This project is done by Petr Vávra and Cecilia Fili and supervised by Thomas Schichl in course of Industrial Project Work at the University of Applied Sciences Upper Austria in Winter semester 2018/19.


# Sensors and actuators:

* Rotary encoder - LPD-3086
* Temperature sensor - DHT11 (currenty in use) DS18B20 (considered replacement)
* Vibration sensor -

# Microcontroller

We switched from Arduino to ESP8266 because of its Wi-Fi capability. 

# Architecture
```
            Wi-Fi
            TCP/IP
ESP8266[0]  --->
ESP8266[1]  --->                        Codesys server
.           --->        Raspberry PI        --->    OPC UA
.           --->
ESP8266[n]  --->
```

# Programming enviroments

For programming microcontrollers we use mostly [PlatformIO IDE](https://platformio.org/platformio-ide).

For programming of Raspberry Pi is used [Codesys](https://store.codesys.com/codesys.html?___store=en) and [Rapsberry Pi extension](https://store.codesys.com/codesys-control-for-raspberry-pi-sl.html)

# Way to target:

* make sensors running with microcontroller,
* set up a communication protocol between microcontroller and Raspberry Pi,
    * decide, which connection is the best - Serial, I2C, consider others,
    * design a way how to transfer data (a binary protocol, ...), espacialy consider and test period,
        * who should measure period?,
* make a way how to transfer data to PLC,
* consider who should evaluate data, make evaluation and predictive algorithms and transfer to top level PLC,
    * this shoul probably be Raspberry PI, than there should be only tiny data line to PLC,
* consider adding other sensor,
    * new vibration sensor, misalignment sensor (ultrasonic).

# Current development architecture and source code description

We are back on the trees. 

We have found that there is a suitable protocol for communication between microcontroller and Raspberry PI - Modbus. It is implemented to Codesys enviroment - there is a [tutorial](https://www.youtube.com/watch?v=lKORELpasPM) how to run that on Raspberry Pi and also a [library](https://github.com/JhonControl/ESP8266_Industrial_ModbusTCP_V2) for ESP8266 microcontroller.

We have established the data transfer between ESP8266 and Raspberry Pi over Modbus.

