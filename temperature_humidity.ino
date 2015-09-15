
#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to


#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// MAC address for your Ethernet shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x4A, 0xE0 };

// Your Xively key to let you upload data
char xivelyKey[] = "leP8dOvtDNNInxLMwNnaizHVHMLvzUnOnIlhwV7U78gZaRGj";

// Analog pin which we're monitoring (0 and 1 are used by the Ethernet shield)
//int sensorPin = 2;

// Define the strings for our datastream IDs
char sensorId[] = "temperature";
char sensorId1[] = "humidity";
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
  XivelyDatastream(sensorId1, strlen(sensorId1), DATASTREAM_FLOAT),
};

// Finally, wrap the datastreams into a feed
XivelyFeed feed(769498344, datastreams, 2 /* number of datastreams */);

EthernetClient client;
XivelyClient xivelyclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Starting single datastream upload to Xively...");
  Serial.println();
   Serial.println("DHTxx test!");

  dht.begin();

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
}

void loop() {
  delay(2000);
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.println(" *F\t");
  
  
  
  
 // int sensorValue = analogRead(sensorPin);
  datastreams[0].setFloat(t);

  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());
  
   datastreams[1].setFloat(h);

  Serial.print("Read sensor value ");
  Serial.println(datastreams[1].getFloat());

  Serial.println("Uploading it to Xively");
  int ret = xivelyclient.put(feed, xivelyKey);
  Serial.print("xivelyclient.put returned ");
  Serial.println(ret);

  Serial.println();
  delay(5000);
  
  
  
  
  
}
