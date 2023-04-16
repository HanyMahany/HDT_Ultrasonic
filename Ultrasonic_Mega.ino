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

#include "config.h"

#define ID_RECTANGLE     1
#define ID_CYLINDRICAL   2

#define SHAPE_ID          200
#define SHAPE_SECTOR_AREA 204
#define SHAPE_HIGHT       208

#define AREA  1
#define HIGHT 2

#define LCD_I2C_ADDRESS 0x27
long t;

String inputString;
unsigned long inputInt;
char key ;

String activeUser ;

uint8_t shape;
float   shapeSectorArea;
float   shapeHight;

double CalculatedVolume;
double CalculatedHight;

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 20, 4);
PS2Keyboard keyboard;
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("(Debug)_@setup");
  pinMode(TRIG_PIN, OUTPUT); // Initializing Trigger Output and Echo Input
  pinMode(ECHO_PIN, INPUT_PULLUP);
  keyboard.begin(KEYBOARD_DATA, KEYBOARD_CLK);
  checkSD();
  
#if NODE_MCU_STATUS == 1
  Serial3.begin(115200);
  Serial3.println("AT+RST");
  Serial.println("AT+RST");
#endif

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("    Hyber SCADA");
  lcd.setCursor(0, 2);
  lcd.print("Tech Makers HDTP0122");
  delay(DEALY_WELCOME_MS);

  EEPROM_readAnything(SHAPE_ID , shape );
  EEPROM_readAnything(SHAPE_SECTOR_AREA  , shapeSectorArea );
  EEPROM_readAnything(SHAPE_HIGHT        , shapeHight      );
  if ( (shape == ID_RECTANGLE ) || (shape == ID_CYLINDRICAL) ) {
    Serial.println("SHAPE_ID : " + String (shape));
  }
  else {
    shape = 0;
    Serial.println("SHAPE_ID : " + String (shape));
  }
  Serial.println("SHAPE_SECTOR_AREA : " + String (shapeSectorArea));
  Serial.println("SHAPE_HIGHT : " + String (shapeHight) );

  t = millis();
  while (getUser() && (millis() - t <=  WAITING_PASS_TIME)) {
    t = millis();
  }
  if (activeUser.equals(USER_ADMIN_NAME)) {

    /*  Admin
      1. config. Shape
        1. Rectangle
        2. Cylindrical
      2. Sector Area
      3. Hight
    */
    while (1) {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("1.Shape config.");
      lcd.setCursor(0, 1); lcd.print("2.Area config.");
      lcd.setCursor(0, 2); lcd.print("3.Hight config.");

      delay(1000);
      key = getChar();
      if (key == '1') {
        //Shape Config.
        lcd.clear();

        lcd.setCursor(0, 0); lcd.print("Shape: ");
        if ((shape == ID_RECTANGLE)) {
          lcd.print("Rectangle");
        }
        else if ((shape == ID_CYLINDRICAL)) {
          lcd.print("Cylindrical");
        }
        lcd.setCursor(0, 1); lcd.print("1.Rectangle");
        lcd.setCursor(0, 2); lcd.print("2.Cylindrical");
        while (1) {
          key = getChar();
          if (key == '1' ) {
            //while (getServerDNS(MAIN));
            shape = ID_RECTANGLE;
            EEPROM_writeAnything(SHAPE_ID, shape );
            Serial.println("Rectangle is Chosen");
            lcd.setCursor(0, 3); lcd.print("New: Rectangle");
            delay(1000);
            break;
          }
          else if (key == '2' ) {
            shape = ID_CYLINDRICAL   ;
            EEPROM_writeAnything(SHAPE_ID, shape );
            Serial.println("cylindrical is Chosen");
            lcd.setCursor(0, 3); lcd.print("New: cylindrical");
            delay(1000);
            break;
          }
          else if (key == '*' ) {
            break;
          }
        }
        continue;
      }
      else if (key == '2') {
        //Sector area
        lcd.clear();
        getSectorAreaHight(AREA );
        continue;
      }
      else if (key == '3') {
        //Hight
        lcd.clear();
        getSectorAreaHight(HIGHT);
        continue;
      }
      else if (key == '*') {
        //ESC
        activeUser = USER_OPERATION_NAME;
        break;
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  getDistanceCM();
  send2NodeData();
  delay(1500);
}

/**************************  Tested  *******************/
char getChar (void) {
  if (keyboard.available()) {
    return  (char)keyboard.read();
  }
}
/**************************  Tested  *******************/
#if NODE_MCU_STATUS == 1
void send2NodeData(void) {
  t = millis();
  Serial.println("(Debug)_@send2Node");
  String message  = "D " + String(CHANNEL) + String(" ") + String (CalculatedVolume);
  // UDP connection
  String cmd = "AT+CIPMUX=1";
  Serial3.println(cmd);
  Serial.println(cmd);
  delay(500);
  // alert user
  Serial3.println("AT+CIPCLOSE");
  Serial.println("AT+CIPCLOSE");
  delay(500);
  // H-SS WebApplication
  cmd = "AT+CIPSTART=4,\"UDP\",\"";
  cmd += DNS;
  cmd += "\",11000";
  Serial3.println(cmd);
  Serial.println(cmd);
  delay(2500);
  if (Serial3.find("ERROR"))  {
    Serial.println("AT+CIPSTART error");
  }
  else  {
    Serial.println("connected");
  }
  // send data length
  cmd = "AT+CIPSEND=4,";
  cmd += String(message.length());
  Serial3.println(cmd);
  Serial.println(cmd);
  delay(500);
  Serial.println("sending data");
  Serial3.println(message);
  Serial.println(message);
  delay(300);
}
#endif

/**************************  Tested  *******************/
bool getUser (void) {
  Serial.println("(Debug)_@getPassword");
  lcd.clear();
  lcd.setCursor(0, 1);    //lcd.print("Password:           ");
  lcd.print("User Password:");

  lcd.setCursor(0, 2);    lcd.print("               ");     lcd.setCursor(5, 2);
  char indi = 0;
  while (millis() - t <= WAITING_PASS_TIME) {

    key = getChar();
    if (((key <= '9' && key >= '0')) || ((key <= 'z' && key >= 'a') || ((key <= 'Z' && key >= 'A'))) ) {     // only act on numeric keys
      t = millis();
      inputString += key;               // append new character to input string
      Serial.print(key);
      lcd.setCursor(5 + indi, 2);
      lcd.print(key);
      indi++;
    }
    else if (key == 0xD) {
      t = millis();
      indi = 0;
      //Some char inserted
      if (inputString.length() > 0) {
        if (inputString.equals(USER_ADMIN_PASS)) {
          Serial.println("Accepted Password...");
          lcd.setCursor(5, 3);
          lcd.print("ADMIN Pass.");
          delay(2000);
          lcd.clear();
          lcd.setCursor(5, 2);
          lcd.print(USER_ADMIN_NAME);
          inputString = "";                 // clear input
          activeUser = USER_ADMIN_NAME;
          return false;
        }
        else if (inputString.equals(USER_OPERATION_PASS)) {
          Serial.println("Accepted Password...");
          lcd.setCursor(5, 3);
          lcd.print("Operation Pass.");
          delay(2000);
          lcd.clear();
          lcd.setCursor(5, 2);
          lcd.print(USER_OPERATION_NAME);
          inputString = "";                 // clear input
          activeUser = USER_OPERATION_NAME;
          return false;
        }
        else {
          indi = 0;
          Serial.println("Incorrect User Password...");
          Serial.println(inputString);
          inputString = "";                 // clear input
          lcd.setCursor(0, 3);    lcd.print("Incorrect Pass.");
          delay(1500);
          lcd.setCursor(0, 3);    lcd.print("                    "); lcd.setCursor(0, 3);
          lcd.setCursor(0, 2);    lcd.print("                    "); lcd.setCursor(0, 2);
          return true;
        }
      }
      //No char inserted
      else {
        indi = 0;
        Serial.println("Error...No Input");
        lcd.setCursor(5, 3);    lcd.print("No input...");
        delay(1500);
        lcd.setCursor(0, 3);    lcd.print("                    "); lcd.setCursor(0, 3);
        inputString = "";               // clear input
      }
    }
    else if (key == 0x7F) {
      t = millis();
      indi = ((indi == 0) ? (0) : (indi - 1));
      inputString = inputString.substring(0, (inputString.length()) - 1);              // clear input
      Serial.println("BackSpace...");
      lcd.setCursor(0, 2); lcd.print("                    "); lcd.setCursor(5, 2);
      lcd.print(inputString); lcd.setCursor( 5 + inputString.length() - 1, 2);
      Serial.println(inputString);
    }
  }
  return true;
}

bool getSectorAreaHight(uint8_t id) {

  uint32_t num = 0;
  lcd.clear();
  lcd.setCursor(0, 0);

  if (id == AREA ) {
    EEPROM_readAnything(SHAPE_SECTOR_AREA  , shapeSectorArea );
    Serial.print("Current Sector Area");
    Serial.println(shapeSectorArea);
    lcd.print("Sector Area:");
    lcd.setCursor(5, 1);
    lcd.print(shapeSectorArea);
  }
  else if (id == HIGHT ) {
    EEPROM_readAnything(SHAPE_HIGHT  , shapeHight );
    Serial.print("Current Shape Hight");
    Serial.println(shapeHight);
    lcd.print("Hight:");
    lcd.setCursor(5, 1);
    lcd.print(shapeHight);
  }
  lcd.setCursor(5, 2);

  int indi = 0;
  while (1) {
    key = getChar();
    if  ( ( key <= '9') && (key >= '0') ) {

      Serial.print(key);
      lcd.setCursor(5 + indi, 2);
      lcd.print(key);
      indi++;
      num = (num * 10) + (key - '0');
    }
    else if (key == 0xD) {
      indi = 0;
      if (num > 0 ) {
        if (id == AREA ) {
          Serial.print("A New Sector Area Approved...  ");
          shapeSectorArea = num;
          EEPROM_writeAnything(SHAPE_SECTOR_AREA  , shapeSectorArea );
        }
        else if (id == HIGHT ) {
          Serial.print("A New Hight Approved...  ");
          shapeHight = num;
          EEPROM_writeAnything(   SHAPE_HIGHT      , shapeHight );
        }
        Serial.println(String(num));

        lcd.setCursor(5, 3);    lcd.print("Approved ");
        delay(2000);
        num = 0;
        return true;
      }
    }
    else if (key == 0x7F) {
      indi = ((indi == 0) ? (0) : (indi - 1));
      num = (uint32_t)(num / 10);
      Serial.println("BackSpace...");
      lcd.setCursor(0, 2); lcd.print("                    "); lcd.setCursor(5, 2);
      lcd.print(num);
      Serial.println(num);
    }
    else if (key == '*') {
      return false;
    }
  }
}

void getDistanceCM(void) {
  digitalWrite(TRIG_PIN, LOW); // Set the trigger pin to low for 2uS
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH); // Send a 10uS high to trigger ranging
  delayMicroseconds(20);

  digitalWrite(TRIG_PIN, LOW); // Send pin low again
  float distance = pulseIn(ECHO_PIN, HIGH); // Read in times pulse

  CalculatedHight = distance / 58; //Convert the pulse duration to distance
  //You can add other math functions to calibrate it well
  CalculatedVolume = shapeSectorArea * (shapeHight - CalculatedHight );
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Volume:");
  lcd.setCursor(5, 2);
  lcd.print(CalculatedVolume);
  lcd.print(" m3");
  delay(500);
}

void saveToSD(void) {

  #if SD_CARD_STATUS == ENABLE
  Serial.println("(Debug)_@saveToSD");

  File LOC_fileHistory = SD.open("History.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (LOC_fileHistory) {

    LOC_fileHistory.print(activeUser + " Tank Volume :" + String(CalculatedVolume) + " Tank Height: " + String(CalculatedHight));
    LOC_fileHistory.close();
    
    // print to the serial port too:
    Serial.println(activeUser + " Tank Volume :" + String(CalculatedVolume) + " Tank Height: " + String(CalculatedHight));
  
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println(activeUser + " error opening History.txt");
  }
  #endif  //End SD_CARD_STATUS
}

void checkSD(void) {
  #if SD_CARD_STATUS == ENABLE
  /****************************SD check*******************************/
  //Check SD card Connectivity
  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  while (!SD.begin(CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // wait 3 sec. and retry connect to SD.
    lcd.setCursor(0, 2);
    lcd.print("SD card Error");
    delay(3000);
    continue;
  }
  Serial.println("card initialized sucessfully...");
  lcd.setCursor(0, 2);
  lcd.print("             ");
  /*******************************        *******************************/
  #elif SD_CARD_STATUS == DISABLE
  Serial.println("SD card disabled from Configurations...");
  lcd.setCursor(0, 2);
  lcd.print("SD card is disabled");
  #endif  //End SD_CARD_STATUS
}
