#include <Arduino.h>
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif

#define DEVICE_ROTARY_ENCODER
#define DEVICE_THERMOMETER_DS18B20
#define COMMUNICATION_MODBUS

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
RotaryEncoder encoder;
void rotaryEncoderHasMoved()
{
  Serial.println(RotaryEncoder::encoderPos);
  #ifdef COMMUNICATION_MQTT
    // TODO: make a list of records
    
  #endif

}
// Initiate static variables
volatile long RotaryEncoder::encoderPos = 0;
void (*RotaryEncoder::function_callback)() = rotaryEncoderHasMoved;

void initRotaryEncoder()
{
  
  pinMode(encoder0PinA , INPUT_PULLUP);
  pinMode(encoder0PinB , INPUT_PULLUP);
  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt (digitalPinToInterrupt(encoder0PinA), encoder.doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt (digitalPinToInterrupt(encoder0PinB), encoder.doEncoderB, CHANGE);
}
#endif

#ifdef DEVICE_THERMOMETER_DS18B20
  // Data wire is plugged into port D2, whis is equal to GPIO 4
const int oneWireBusPin = 4;
  // Resolution of the thermometer could be c
const int thermometerDS18B20Resolution = 9;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(oneWireBusPin);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

unsigned long lastTempRequest = 0;
float temperatureDS18B20 = 0.0;


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
const int TemperatureSensorDS18B20Register = 20;
//ModbusIP object
ModbusIP mb;

void initializeModbus()
{
  mb.begin();
  #ifdef DEVICE_ROTARY_ENCODER
  mb.addHreg(RotaryEncoderRegister, 0);
  #endif
  #ifdef DEVICE_THERMOMETER_DS18B20
  mb.addHreg(TemperatureSensorDS18B20Register, 0);
  #endif
}
#endif

#ifdef COMMUNICATION_MQTT
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[50];

const char* mqtt_server = "broker.mqtt-dashboard.com";
#ifdef DEVICE_THERMOMETER_DS18B20
#endif
#ifdef DEVICE_ROTARY_ENCODER
const char* mqttTopicRotaryEncoder = "/BLABLABLBA/";
#endif

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
#endif

const char* ssid = "khwelsABU8";
const char* password = "FKPIFURCSZKKXHRZKSV3";

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
  initRotaryEncoder();
  #endif
  #ifdef DEVICE_THERMOMETER_DS18B20
  initThermoterDS18B20();
  #endif
  #ifdef COMMUNICATION_MODBUS
  initializeModbus();
  #endif
  #ifdef COMMUNICATION_MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
  mb.Hreg(RotaryEncoderRegister, (uint16_t) RotaryEncoder::encoderPos);
  #endif 
  #endif
  #ifdef COMMUNICATION_MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if (millis() - lastMsg >= 2000) {
    
    #ifdef DEVICE_ROTARY_ENCODER
    snprintf(msg, 50, "%lu", RotaryEncoder::encoderPos);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqttTopicRotaryEncoder, msg);
    #endif
    lastMsg = millis();
  }
  #endif

  #ifdef DEVICE_THERMOMETER_DS18B20
  if (millis() - lastTempRequest >= 750/ (1 << (12-thermometerDS18B20Resolution))) // waited long enough??
  {
    temperatureDS18B20 = sensors.getTempCByIndex(0);
    sensors.requestTemperatures(); 
    lastTempRequest = millis();
  }
  #endif

  delay(200);
}