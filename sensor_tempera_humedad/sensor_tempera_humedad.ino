//Humidity sensor: where is the 's' = DO, middle = V++, the another one = GNN

#include <dht.h>
#include <Wire.h>

#define DHT11_PIN 7

dht DHT;
volatile int temperatureAndHumidity[2];


void requestEvent() {
  Wire.write((const uint8_t *)temperatureAndHumidity, 4); 
}


void setup(){
    
  temperatureAndHumidity[0] = -1;
  temperatureAndHumidity[1] = -1;
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  

    temperatureAndHumidity[0] = DHT.temperature*10;
    temperatureAndHumidity[1] = DHT.humidity*10;
  delay(1000);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
  Serial.print("Temperature = ");
  Serial.println(temperatureAndHumidity[0]); 
  Serial.print("Humidity = ");
  Serial.println(temperatureAndHumidity[1]);
}
