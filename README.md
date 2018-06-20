# autoclave
Design of an automatic autoclave.

This autoclave has the following alarms:
-open door
-water level (high/low)
-Internal Pressure (high/low)
-Temperature (high/low)

According to this variables they'll control the system.
e.g. The temperature controls the resistance which heat up the water
as a result we have vapour and then an increase in the pressure.


FILE STRUCTURE:

SRC FOLDER---- The main code, it was made in arduino.IDE.

ARDUINO.IDX---Library to add arduino to PROTEUS for the schematic design.

ARDUINO.LIB--- Library to add arduino to PROTEUS to simulate.

ARDUINO.DSN---This is the electronic schematic circuit to simulate the autoclave,
              arduino.PWI and LastLoadedarduino.DBK are archives created by the
              simulation software (PROTEUS)

Materials:
-mp3v5050gp pressure sensor
-LCD
-Arduino
-1KΩ (4),1MΩ(2),220Ω(6) resistors.
- 5v dc Relay.
-LM 35 temperature sensor.
-Ultrasonic sensor HC-SR04.
