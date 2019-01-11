#include <Arduino.h>
#include "RotaryEncoder.h"
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#define COMMUNICATION_MODBUS
#ifdef COMMUNICATION_MODBUS
  #include <ModbusIP_ESP8266.h>
#endif
#ifdef COMMUNICATION_MQTT
  #include <PubSubClient.h>
#endif


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

#ifdef COMMUNICATION_MODBUS
// Modbus Registers Offsets (0-9999)
const int TEST_HREG = 0;

//ModbusIP object
ModbusIP mb;

void initializeModbus()
{
  mb.begin();
  mb.addHreg(TEST_HREG, 100);
}
#endif
#ifdef COMMUNICATION_MQTT
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[50];

const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_publishToTopic = "/BLABLABLBA/";

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
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
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

const char* ssid = "........";
const char* password = "........";

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
  #else
  Serial.begin(115200);
  #endif 
  
  initRotaryEncoder();
  initializeModbus();
  #ifdef COMMUNICATION_MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  #endif
}

void loop() 
{
  if(!WiFi.isConnected){
    WiFi.reconnect();
  }
  #ifdef COMMUNICATION_MODBUS
  mb.task();
  mb.Hreg(TEST_HREG, RotaryEncoder::encoderPos);
  #endif
  #ifdef COMMUNICATION_MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    snprintf(msg, 50, "%lu", RotaryEncoder::encoderPos);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_publishToTopic, msg);
  }
  #endif


  delay(200);
}