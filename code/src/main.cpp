/*
                 GNU GENERAL PUBLIC LICENSE
                  Version 3, 29 June 2007
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 Original code sources from:
  Erdfeuchtemessung mit ESP8266-01 / ESP8266-12(e)
  Measure soil moisture with ESP8266-01 / ESP8266-12(e)
  (2016)
  @Autors:
    - Ralf Wießner (aka Zentris)
  https://github.com/Zentris/erdfeuchtemessung
*/
#include "config.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Expose Espressif SDK functionality
extern "C" {
    #include "user_interface.h"
}

// Set up ESP8266 ADC for voltage read
ADC_MODE(ADC_VCC);
float vcc;

// Sensor average
unsigned long soilMoistAveraged;

volatile unsigned long counter = 0; // interrupt loop counter

// Timer placeholders
unsigned long startMills = 0;

// Function Prototypes
void readSensor();
void setup_wifi();
void reconnect();
void publishJSON();
void goingToSleep();
// End Prototypes

// Begin code
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  startMills = millis();
  vcc = ESP.getVcc();
  Serial.begin(115200);

  digitalWrite(POWER_PIN, HIGH);  // switch sensor power on

  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
}

void setup_wifi() {
  // We start by connecting to a WiFi network
  if (DEBUG_PRINT) {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - startMills > WIFI_TIMEOUT) {
      if (DEBUG_PRINT) {
        Serial.println("Taken too long to connect to WiFi. Going to sleep");
      }
      goingToSleep();
    }
  }

  if (DEBUG_PRINT) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  readSensor();

  if(soilMoistAveraged) {
    if (DEBUG_PRINT) {
      Serial.print("Read Sensor. Publish message: ");
    }
    publishJSON();
  }

  goingToSleep();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    if (DEBUG_PRINT) {
      Serial.print("Attempting MQTT connection...");
    }
    // Attempt to connect
    if (client.connect("CapSensor")) {
      if (DEBUG_PRINT) {
        Serial.println("Connected");
      }
    } else {
      if (DEBUG_PRINT) {
        Serial.print("Failed to connect. State = ");
        Serial.print(client.state());
        Serial.println(" Retry in 2 seconds");
      }
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

void publishJSON() {
  if (client.connected()) {
    StaticJsonBuffer<150> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonArray& payload = root.createNestedArray("payload");
    JsonObject& data = payload.createNestedObject();
    data["sensorid"] = SENSORNAME;
    data["moisture"] = soilMoistAveraged;
    data["vcc"] = vcc/1000;
    data["cycletime"] = millis() - startMills;
    if (DEBUG_PRINT) {
      Serial.print("Sending payload: ");
      root.prettyPrintTo(Serial);
    }

    char buffer[150];
    root.printTo(buffer, sizeof(buffer));
    if (client.publish(TOPIC, buffer));
      if (DEBUG_PRINT) {
        Serial.print("Published OK  --> ");
        Serial.println(millis() - startMills);
      }
    } else {
      if (DEBUG_PRINT) {
        Serial.print("Publish failed --> ");
        Serial.println(millis() - startMills);
      }
    }
}

/**
 * Simple median/average calculation
 * ------------------------------------
 * Getting array will be sorted and now get the average over the middle values
 */
unsigned long median(unsigned long *values, size_t arraySize) {
  unsigned long tmp = 0;     // set to 0, make the compiler happy :-)
  const size_t relVal = 2;   // +- 2 Werte + 1 für die Mittelwertberechnung

  for (size_t i=0; i < arraySize-2; i++) {
    for (size_t j=arraySize-1; j > i; j--) {
      if ( values[j] < values[j-1] ) {
        tmp = values[j];
        values[j] = values[j-1];
        values[j-1] = tmp;
      }
    }
  }

  if (DEBUG_PRINT) {
    for (size_t x=0; x<arraySize; x++) {
      Serial.print("Values: ");
      Serial.print(values[x]);
    }
  }

  tmp = 0;
  for (size_t i=arraySize/2-relVal; i<arraySize/2+relVal+1; tmp +=values[i++]) {}
  return tmp/(relVal*2+1) * 1000/MEASURING_TIME;
}

/**
 * Interrupt function: increment the counter
 */
void intfunc() {
  counter++;
}

void readSensor() {
  unsigned long dataArray[MEASURING_INTERVALS];

  for (size_t i=0; i < MEASURING_INTERVALS; i++) {
    //Serial << F("[bodenfeuchtemessung] SensorID: ") << sensordata.sensorId << F(" - Measuring No.: ") << i << F(" on GPIO-Pin ") << sensordata.intGPIO << F(" = ");
    counter = 0;

    attachInterrupt(SENSOR_PIN, intfunc, RISING);
    delay(MEASURING_TIME);
    detachInterrupt(SENSOR_PIN);
    dataArray[i] = counter;
    delay(100);         // wait a small time
  }

  digitalWrite(POWER_PIN, LOW);   // switch sensor power off

  soilMoistAveraged = median(dataArray, MEASURING_INTERVALS);

  if (DEBUG_PRINT) {
    Serial.print("Sensor Average: ");
    Serial.print(soilMoistAveraged);
  }
}

void goingToSleep() {
  // All Good
  if (DEBUG_PRINT) {
    Serial.print("About to go to sleep: Time taken to complete cycle: ");
    Serial.println(millis() - startMills);
  }
  client.disconnect();
  yield();
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  ESP.deepSleep(deep_sleep, DS_MODE);
  //ESP.deepSleep(0);
  delay(250);
}
