#include "includes.h"

void setup() 
{
  // Serial initialization
  Serial.begin(115200);
  Serial.println("(Debug)_@setup");
  
  // Initializing Trigger Output and Echo Input
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT_PULLUP);
  
  // Keyboard initialization
  keyboard.begin(KEYBOARD_DATA, KEYBOARD_CLK);
  
  // SD card initialization
  checkSD();
  
  #if NODE_MCU_STATUS == ENABLE
  // NodeMCU Serial initialization
  Serial3.begin(115200);
  Serial3.println("AT+RST");
  Serial.println("AT+RST");
  #endif

  // LCD I2C initialization
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Hyber SCADA");
  lcd.setCursor(0, 1);
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



