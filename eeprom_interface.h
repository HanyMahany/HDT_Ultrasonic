
#ifndef _EEPROM_INTERFACE_H_
#define _EEPROM_INTERFACE_H_

template <class T> int EEPROM_writeAnything(int ee, const T& value);
template <class T> int EEPROM_readAnything(int ee, T& value);

#endif
