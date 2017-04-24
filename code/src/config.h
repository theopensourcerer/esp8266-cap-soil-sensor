/*
Capacitive Soil Moisture Sensor

For the Espressif ESP8266

Uses MQTT and custom PCB Sensor to read and transmit a frequency to an MQTT
Broker. The Frequency varies depending on the moisture level of the medium
surrounding the probe.

This code was written and tested using the PlatformIO IDE and the ESP8266-12F
module.

Ideas and code from various sources:
https://github.com/acolomitchi/cap-soil-moisture-v2
https://github.com/Zentris/erdfeuchtemessung
https://github.com/lh84/moisture_sensor_esp12

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

This code is licensed under the GNU GPL and is open for ditrbution
and copying in accordance with the license.
This header must be included in any derived code or copies of the code.

(C) Alan Lord 2017
*/

// Debugging
#define DEBUG_PRINT 1

// Change these to suit your own application and network.
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";
const char* MQTT_SERVER = "192.168.1.2";

// SensorName
const char* SENSORNAME = "soilpot1";

// Topic
const char* TOPIC = "PT/Soil";

// Deep Sleep (uSec) Debug = 10000000 (10 Seconds). Production: 6000000000 (1 hour)
long deep_sleep = 10000000;

// Timout if WiFi doesn't connect (15000000 = 15seconds)
long WIFI_TIMEOUT = 15000000;

// Deep Sleep mode.
#define DS_MODE WAKE_RF_DEFAULT

#define POWER_PIN 14

#define SENSOR_PIN 12

#define MEASURING_TIME 100 // how long a sample lasts
#define MEASURING_INTERVALS 9 // how many samples are taken?
