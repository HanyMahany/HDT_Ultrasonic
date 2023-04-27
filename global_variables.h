#ifndef G_VARS_H_
#define G_VARS_H_

/*  variable for distance measurements */
long t;

/*  variables for receiving inputs from Keyboard/Keypad */
String inputString;
unsigned long inputInt;
char key ;

/*  variable for idntifiy the current user */
String activeUser ;

/*  variable for system state and parameters */
uint8_t mode;
float   shapeTotalVolume;
float   shapeHight;

/*  variable for system calculations */
double CalculatedVolume;
double CalculatedHight;

double dataToSendSave;
/*  Object for LCD  */
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

/*  Object for Entry  */
PS2Keyboard keyboard;

#endif