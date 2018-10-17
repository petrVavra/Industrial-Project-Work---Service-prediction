#include <Arduino.h>

#include <DHT.h>
#include <Wire.h>

#define DHTPIN 7     


#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int8_t temperature = -1;
int8_t humidity = -1;


void requestEvent() 
{
  Wire.write(temperature); 
  Wire.write(humidity); 
}

void setup() 
{
  dht.begin();
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() 
{
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) 
  {
    temperature = -1;
    humidity = -1;
    return;
  }
  else 
  {
    temperature = t;
    humidity = h;
  }

}
