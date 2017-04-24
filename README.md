# ESP8266 Capacitive Soil Moisture Sensor

## Table of Contents
* [Summary](#summary)
* [Hardware](#hardware)
* [Case](#case)
* [Code](#code)
* [Licenses](#licenses)

## Summary
This is a personal project to build a Capacitive Soil Moisture Sensor for my hobby of growing chillies.

The project consists of three parts: the hardware (PCB and Sensor(s) etc.(, the case to hold the sensor, and the software.

I wanted to design and build my own sensors with built in WiFi so they could operate "standalone" in my tunnel. I wanted them to run from a decent capacity battery such as an 18650 Li-ion and to be able to report back information at timely intervals to my home server and database. Ultimately I want to use this information to be able to automate the watering of my plants.

The Internet is a wonderful thing. I have spent many dark and long evenings through the 2016/17 winter, learning, reading and searching for information on Capacitive moisture sensors. There are numerous resources (and individual people behind each of them) I would like to thank for taking the time to design, document and discuss the various mechanisms for capacitive moisture measurement. Hopefully my own small contributions may help others in the future.

Why use capacitive sensors? 

* The cheap and traditional moisture sensor uses a resistive technique. That is, to put it simply, the probe usually consists of two electrically conductive "prongs" which are inserted into the soil with a known gap between them. As the moisture increases, the electrical resistance decreases. This technique works but is not ideal mostly due to electrolysis [1]. Essentially the probes deteriorate rapidly due to this phenomenom and hence the measurements also change and the probes need to be replaced quite frequently

* The resitive appraoch requires significantly more power (electric current) to operate than the capacitive technique, so battery life would be constrained

* The fundamental principle of the capacitive sensor is that you measure the time it takes for your capacitor (the probe) to charge. This charging period changes proportionately to the moisture level of the soil. It means there is no direct electrical contact between the soil and the probe (it is insulated) therefore should last a great deal longer

* It is harder to do; hence far more interesting to design and produce ;-)

Below is my list, in no particular order, of the online resources I found to be most helpful during my research (there were and are others; Google is your friend):

http://pcb.daince.net/doku.php?id=sensor_soil_2
http://zerocharactersleft.blogspot.co.uk/2011/11/pcb-as-capacitive-soil-moisture-sensor.html

https://www.dfrobot.com/wiki/index.php/Capacitive_Soil_Moisture_Sensor_SKU:SEN0193 (Bought one of these to test. It works)
https://github.com/Miceuz/i2c-moisture-sensor
and
https://wemakethings.net/chirp/ (Bought one of these - this works too)

http://www.instructables.com/id/Comparison-of-Capacitive-Soil-Probes/
and then
https://github.com/acolomitchi/cap-soil-moisture-v2 (I have used this probe design. Thanks acolomitchi)

Then I came across these few resources (thank you Google Translate)
* https://www.mikrocontroller.net/topic/335407

Which led me here:
* http://shop.thomasheldt.de/product_info.php?info=p90_giess-o-mat-sensor-kit.html (Bought a couple of these and they work)

Which then led me to:
* https://github.com/Zentris/erdfeuchtemessung

and
* https://github.com/lh84/moisture_sensor_esp12

I have also been following along with this project:
* https://luckyresistor.me/2017/02/08/how-to-design-a-cheap-plant-watering-sensor-part-1/
I really like the way this project has been analysed, designed and documented - much more so than mine which is a "try-something-and-see-if-it-works" approach.

These resources above gave me confidence that a custom design was possible so I set to work basing the Schematic and PCB on the work of the people above. The use of a Schmitt Trigger Inverter to "square" up the oscillations of the sensor probe means it can be read by pretty much any GPIO as long as the MCU is fast enough. The ESP8266 has some limitations in this regard so I considered using a half-wave rectifier to convert this to a simple analogue voltage but the work of Zentris and Lars(lh84) convinced me that the ESP8266 was capable of reading the frequency directly if it can be kept below about 150Khz.

This is _all_ still very much a work in progress. I have built a prototype board and have had it running on my desk but the results leave me rather confused :-(


## Hardware
The Schematic and PCB are designed in KiCAD. A version of this board has been fabricated but it is not the final version. Probably the most interesting part is the layout of the probe footprint itself. This came from acolomitchi as mentioned above. (I have modified the layout somewhat so it is more suited to an integrated PCB - I also made it a little longer as my target was for quite large pots).


## Case
The case is designed in OpenSCAD and is suitable for printing on a 3D Printer. The basic idea is that the PCB slides into the case. The PCB has about 1.5mm of empty space down each side of the board which should be enough to slot in place. The probe end of the PCB fits through the slot in the bottom of the case. There should be enough height to support the power connector (not tested) and the other half of the case holds an 18650 battery in a plastic holder. The "lid" screws on and has protrusions which should prevent the probe PCB and the battery from moving about too much. I plan to use something like silicon bathrooom sealant around the probe slot to prevent any moisture ingressing into the case. Please feel free to use and improve - within the terms of the license of course.


## Code
The software for the ESP8266 is in this directory. It should be fairly simple C code. The config.h file contains user-defined parameters. The ESP8266 code works like this: On power up the ESP attempts to connect to the WiFi network. If successful it then attempts to read the frequency of the capacitive sensor probe. If the Sensor probe is read successfully the data is formatted as a JSON object and sent to the MQTT broker. The ESP8266 also sends information about its supply voltage and the time it took to run this code loop. Finally the code puts the ESP into "Deep Sleep" for a preconfigured period.


## Licenses
* The software is GPL (http://www.gnu.org/licenses/) 
* The OpenSCAD case design, and the Schematic and PCB are Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License (https://creativecommons.org/licenses/by-nc-sa/4.0/)


[1] https://en.wikipedia.org/wiki/Electrolysis
