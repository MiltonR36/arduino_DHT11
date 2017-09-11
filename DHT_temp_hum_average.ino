// Temperature and Humidity
// DHT11 sensor

#include "DHT.h"

// Init DHT sensor
DHT dht;

const byte dht_pin = 2;  // DHT sensor analog pin
const byte probe = 12;   // reading probes

// globals
int index = 0;
int baselineTemp;
int baselineHum;
int tempArr[probe];
int humArr[probe];

void readSensors() {
  // DHT reading
  tempArr[index] = dht.getTemperature();
  humArr[index]  = dht.getHumidity();

  // get average
  baselineTemp = average(tempArr, probe);
  baselineHum  = average(humArr, probe);
}

void setup() {
  dht.setup(2);

  Serial.begin(9600);
  Serial.println("Calibrating sensors...");

  // calibration
  for (index = 0; index < probe; index = index + 1) {
    readSensors();

    delay(dht.getMinimumSamplingPeriod());
  }

  Serial.println("Ready");
  index = 0;
}

// assuming array is int
int average (int * array, int len) {
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return ((int) sum) / len ;  // average will be fractional, so float may be appropriate.
}

void loop() {
  readSensors();

  char msg[128];
  sprintf(msg, "temp: %d, hum: %d", baselineTemp,baselineHum);
  Serial.println(msg);

  // index++
  index = index + 1;

  if (index > probe-1){
    index = 0;
  }

  delay(dht.getMinimumSamplingPeriod());
}
