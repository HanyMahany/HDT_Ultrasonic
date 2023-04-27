# Ultrasonic_Mega

## Notes

- All pins are 5V logic.
- If Keyboard is used wire pins 3, 4
- If Keypad is used wire pins 24, 25, 26 and 27 to Rows 0,1,2,3 of the Keypad and wire 28, 29, 30 and 31 to Columns 0,1,2,3 of the Keypad.

## Pin Mapping for Arduino Mega

| Arduino Mega Pin Number(Name)	| Target|
|-------------------------------|-------|
|3	|CLK_Keyboard **_Discard if Keypad is used_** |  
|4	|DATA_Keyboard **_Discard if Keypad is used_** | 
|8	|ECHO pin|
|9	|TRIG pin|
|14(Tx3)	|(Rx) NodeMCU|
|15(Rx3)	|(Tx) NodeMCU|
|16(Tx2)	|(DI) of max 485|
|17(Rx2)	|(RO) of  max 485|
|20 (SDA)	|SDA of I2C module|
|21 (SCL)	|SCL of I2C module|
|22	|DE of  max 485|
|23	|RE of  max 485|
|24, 25, 26, 27 | Rows 0,1,2,3 **_Discard if Keyboard is used_** | 
|28, 29, 30, 31 | Cols 0,1,2,3 **_Discard if Keyboard is used_** | 
|50 (MISO)	|MISO of SD card Module|
|51 (MOSI)	|MOSI of SD card Module|
|52 (SCK)	|SCK of SD card Module|
|53 ((SS) ̅)	|CS of SD card Module|
--------------

