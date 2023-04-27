#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <Wire.h>
//lcd I2C Lib.
#include <LiquidCrystal_I2C.h>
//Keyboard Lib.
#include <PS2Keyboard.h>
//EEPROM Lib.
#include <EEPROM.h>

//SPI Lib.
#include <SPI.h>
//SD card Lib.
#include <SD.h>

#include "private.h"
#include "pin_mapping.h"
#include "system_config.h"
#include "eeprom_interface.h"
#include "nodemcu_interface.h"
#include "sdcard_interface.h"
#include "parameters_interface.h"
#include "global_variables.h"

#endif