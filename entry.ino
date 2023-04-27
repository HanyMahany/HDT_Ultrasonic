#include "entry_interface.h"

/*  Object for Entry  */
#if ENTRY == ENTRY_KEYBOARD
PS2Keyboard keyboard;

#elif ENTRY == ENTRY_KEYPAD
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {ROW_PINS}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {COL_PINS}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
#endif

void entry_init(void)
{

  #if ENTRY == ENTRY_KEYBOARD
  // Keyboard initialization
  keyboard.begin(KEYBOARD_DATA, KEYBOARD_CLK);
  #endif

}

/**************************  Tested  *******************/
char getChar (void) 
{
  #if ENTRY == ENTRY_KEYBOARD
  
  if (keyboard.available()) {
    return  (char)keyboard.read();
  }

  #elif ENTRY == ENTRY_KEYPAD

  char pressed = customKeypad.getKey();
  delay(100);
  return pressed;
  #endif
}
