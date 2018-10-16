
//Humidity sensor: where is the 's' = DO, middle = V++, the another one = GNN

#include <dht.h>
#include <Wire.h>

dht DHT;
int temperature;
int humidity;
#define DHT11_PIN 7

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(temperature); // respond with message of 6 bytes as expected by master
  Wire.write(humidity);
  
}


void setup(){
  Serial.begin(9600);
   Wire.begin(8);                // join i2c bus with address #8
   Wire.onRequest(requestEvent); // register event
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature); 
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  temperature = DHT.temperature*100;
  humidity = DHT.humidity*100;
  delay(1000);
}
