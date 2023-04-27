
#include "eeprom_interface.h"

/***********************************  EEPROM_writeAnything   *************************************/
template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

/***********************************  EEPROM_readAnything   *************************************/
template <class T> int EEPROM_readAnything(int ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}

void EEPROM_scanParameters (void)
{
  EEPROM_readAnything(MODE , mode );
  EEPROM_readAnything(SHAPE_FULL_VOLUME  , shapeTotalVolume );
  EEPROM_readAnything(SHAPE_HIGHT        , shapeHight      );
  Serial.println("MODE: " + String((mode==MODE_VOLUME)? ("VOLUME"): ("DISTANCE")));
  Serial.println("SHAPE_FULL_VOLUME: "+String(shapeTotalVolume, 3));
  Serial.println("SHAPE_HIGHT: "+ String(shapeHight ,3));
}

void EEPROM_resetParameters (void)
{
  uint32_t clear = 0;
  EEPROM_writeAnything(MODE , clear );
  EEPROM_writeAnything(SHAPE_FULL_VOLUME  , clear );
  EEPROM_writeAnything(SHAPE_HIGHT        , clear );
}