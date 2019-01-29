#include <Arduino.h>
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif

// Configuration of devices in use. Comment appropriate defines, when the device is not in use.
#define DEVICE_ROTARY_ENCODER
#define DEVICE_THERMOMETER_DS18B20

// Configuration of communication protocols in use. Comment appropriate defines, when the protocol is not in use.
#define COMMUNICATION_MODBUS
#define COMMUNICATION_MQTT

#ifdef DEVICE_ROTARY_ENCODER
  #include "RotaryEncoder.h"
#endif
#ifdef DEVICE_THERMOMETER_DS18B20
  #include <OneWire.h>
  #include <DallasTemperature.h>
#endif
#ifdef COMMUNICATION_MODBUS
  #include <ModbusIP_ESP8266.h>
#endif
#ifdef COMMUNICATION_MQTT
  #include <PubSubClient.h>
#endif

// Handling possible non-sense configuration.
#ifndef DEVICE_ROTARY_ENCODER
  #ifndef DEVICE_THERMOMETER_DS18B20
    #error There is no device to use
  #endif
#endif

#ifndef COMMUNICATION_MQTT
  #ifndef COMMUNICATION_MODBUS
    #error There is not defined communication protocol
  #endif
#endif

#ifdef DEVICE_ROTARY_ENCODER
void rotaryEncoderHasMoved()
{
  //TODO: possibly introduce some realtime data storage to recollect exact move of encoder.
}

volatile EncoderPosition RotaryEncoder::encoderPos = { .asLong = 0 };
void (*RotaryEncoder::function_callback)() = rotaryEncoderHasMoved;
#endif

#ifdef DEVICE_THERMOMETER_DS18B20
// Data wire is plugged into port D2, whis is equal to GPIO 4
const int oneWireBusPin = 4;
// Resolution of the thermometer could be in range 9 - 12
const int thermometerDS18B20Resolution = 9;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(oneWireBusPin);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

unsigned long lastTempRequest = 0;

// In purpose of sending data over modbus we introduce this union as a data storage.
union TemperatureDataStorage
{
  float dataAsFloat;
  uint16_t dataAsInt[sizeof(float)/2];
}temperatureDS18B20;

void initThermoterDS18B20()
{
  sensors.begin();
  DeviceAddress tempDeviceAddress;
  sensors.getAddress(tempDeviceAddress, 0);
  sensors.setResolution(tempDeviceAddress, thermometerDS18B20Resolution);
  sensors.setWaitForConversion(false);  // makes it async
}
#endif

#ifdef COMMUNICATION_MODBUS
// Modbus Registers Offsets (0-9999)
const int RotaryEncoderRegister = 0;
const int TemperatureSensorDS18B20Register = 0;
//ModbusIP object
ModbusIP mb;

void initializeModbus()
{
  mb.begin();
  // add registers for each device
  #ifdef DEVICE_ROTARY_ENCODER
  mb.addHreg(RotaryEncoderRegister, 0, sizeof(EncoderPosition)/2);
  #endif
  #ifdef DEVICE_THERMOMETER_DS18B20
  mb.addHreg(TemperatureSensorDS18B20Register, 0, sizeof(TemperatureDataStorage)/2);
  #endif
}
#endif

#ifdef COMMUNICATION_MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastMsg = 0;
char msg[50];

const int mqttMessagesSendingPeriod = 300;

// Adjust the mqtt server adress
const char* mqtt_server = "192.168.1.127";
#ifdef DEVICE_THERMOMETER_DS18B20
// Adjust the mqtt topics
const char* mqttTopicDS18B20Encoder = "/Temperature/";
#endif
#ifdef DEVICE_ROTARY_ENCODER
const char* mqttTopicRotaryEncoder = "/RotaryEncoder/";
#endif

// This function is blocking - when is MQTT in use make sure that the server is available, otherwise other protocols will not work
void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random mqttClient ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //mqttClient.publish("outTopic", "hello world");
      // ... and resubscribe
      //mqttClient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
#endif

// fill in wi-fi credentials
const char* ssid = "";
const char* password = "";

void connectToWifi()
{
  WiFi.begin(ssid, password);

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
  #else //ESP32
  Serial.begin(115200);
  #endif
  connectToWifi();
  #ifdef DEVICE_ROTARY_ENCODER
  RotaryEncoder::initRotaryEncoder();
  #endif
  #ifdef DEVICE_THERMOMETER_DS18B20
  initThermoterDS18B20();
  #endif
  #ifdef COMMUNICATION_MODBUS
  initializeModbus();
  #endif
  #ifdef COMMUNICATION_MQTT
  mqttClient.setServer(mqtt_server, 1883);
  #endif
}

void loop()
{
  if(!WiFi.isConnected()){
    WiFi.reconnect();
  }
  #ifdef COMMUNICATION_MODBUS
  mb.task();
  #ifdef DEVICE_ROTARY_ENCODER
  mb.Hreg(RotaryEncoderRegister, RotaryEncoder::encoderPos.asInt[0]);
  mb.Hreg(RotaryEncoderRegister+1, RotaryEncoder::encoderPos.asInt[1]);
  #endif
  #ifdef DEVICE_THERMOMETER_DS18B20
  mb.Hreg(TemperatureSensorDS18B20Register, temperatureDS18B20.dataAsInt[0]);
  mb.Hreg(TemperatureSensorDS18B20Register+1, temperatureDS18B20.dataAsInt[1]);
  #endif
  #endif
  #ifdef COMMUNICATION_MQTT
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  // periodically send messages to mqtt
  if (millis() - lastMsg >= mqttMessagesSendingPeriod) {
    #ifdef DEVICE_ROTARY_ENCODER
    snprintf(msg, 50, "%ld", RotaryEncoder::encoderPos.asLong);
    mqttClient.publish(mqttTopicRotaryEncoder, msg);
    #endif
    #ifdef DEVICE_THERMOMETER_DS18B20
    snprintf(msg, 50, "%f", temperatureDS18B20.dataAsFloat);
    mqttClient.publish(mqttTopicDS18B20Encoder, msg);
    #endif
    lastMsg = millis();
  }
  #endif

  #ifdef DEVICE_THERMOMETER_DS18B20
  // periodically measure the temperature
  if (millis() - lastTempRequest >= 750/ (1 << (12-thermometerDS18B20Resolution))) // waited long enough??
  {
    // pick up the the value from last measurement
    temperatureDS18B20.dataAsFloat = sensors.getTempCByIndex(0);
    // ask for new one
    sensors.requestTemperatures();
    lastTempRequest = millis();
  }
  #endif

  delay(2);
}