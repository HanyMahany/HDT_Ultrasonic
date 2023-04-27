#include "parameters_interface.h"

/**************************  Tested  *******************/
bool param_getUser (void)
{
  Serial.println("(Debug)_@getPassword");
  lcd.clear();
  lcd.setCursor(0, 0);    //lcd.print("Password:           ");
  lcd.print("User Password:");

  lcd.setCursor(0, 1);    lcd.print("               ");     lcd.setCursor(5, 1);
  char indi = 0;
  while (millis() - t <= WAITING_PASS_TIME) {

    key = getChar();
    if (((key <= '9' && key >= '0')) || ((key <= 'z' && key >= 'a') || ((key <= 'Z' && key >= 'A'))) ) {     // only act on numeric keys
      t = millis();
      inputString += key;               // append new character to input string
      Serial.print(key);
      lcd.setCursor(5 + indi, 1);
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
          lcd.setCursor(5, 1);
          lcd.print("ADMIN Pass.");
          delay(2000);
          lcd.clear();
          lcd.setCursor(5, 1);
          lcd.print(USER_ADMIN_NAME);
          inputString = "";                 // clear input
          activeUser = USER_ADMIN_NAME;
          return false;
        }
        else if (inputString.equals(USER_OPERATION_PASS)) {
          Serial.println("Accepted Password...");
          lcd.setCursor(5, 1);
          lcd.print("Operation Pass.");
          delay(2000);
          lcd.clear();
          lcd.setCursor(5, 1);
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
          lcd.setCursor(0, 1);    lcd.print("Incorrect Pass.");
          delay(1500);
          lcd.setCursor(0, 1);    lcd.print("                    "); lcd.setCursor(0, 1);
          return true;
        }
      }
      //No char inserted
      else {
        indi = 0;
        Serial.println("Error...No Input");
        lcd.setCursor(5, 1);    lcd.print("No input...");
        delay(1500);
        lcd.setCursor(0, 1);    lcd.print("                    "); lcd.setCursor(0, 1);
        inputString = "";               // clear input
      }
    }
    else if (key == 0x7F) {
      t = millis();
      indi = ((indi == 0) ? (0) : (indi - 1));
      inputString = inputString.substring(0, (inputString.length()) - 1);              // clear input
      Serial.println("BackSpace...");
      lcd.setCursor(0, 1); lcd.print("                    "); lcd.setCursor(5, 1);
      lcd.print(inputString); lcd.setCursor( 5 + inputString.length() - 1, 1);
      Serial.println(inputString);
    }
  }
  return true;
}

bool param_getVolumeHight(uint8_t id) 
{
  uint32_t num = 0;
  lcd.clear();
  lcd.setCursor(0, 0);

  if (id == VOLUME) {
    EEPROM_readAnything(SHAPE_FULL_VOLUME , shapeTotalVolume );
    Serial.print("Saved Full Volume");
    Serial.println(shapeTotalVolume);
    lcd.print("Volume:");
    lcd.setCursor(5, 1);
    lcd.print(shapeTotalVolume);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New Volume:");

  }
  else if (id == HIGHT ) {
    EEPROM_readAnything(SHAPE_HIGHT  , shapeHight );
    Serial.print("Saved Shape Hight");
    Serial.println(shapeHight);
    lcd.print("Hight:");
    lcd.setCursor(5, 1);
    lcd.print(shapeHight);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New Heigh:");
  }
  
  lcd.setCursor(5, 1);

  int indi = 0;
  while (1) {
    key = getChar();
    if  ( ( key <= '9') && (key >= '0') ) {

      Serial.print(key);
      lcd.setCursor(5 + indi, 1);
      lcd.print(key);
      indi++;
      num = (num * 10) + (key - '0');
    }
    else if (key == 0xD) {
      indi = 0;
      if (num > 0 ) {
        if (id == VOLUME ) {
          Serial.print("A New Volume Approved...  ");
          shapeTotalVolume = num;
          EEPROM_writeAnything(SHAPE_FULL_VOLUME  , shapeTotalVolume );
        }
        else if (id == HIGHT ) {
          Serial.print("A New Hight Approved...  ");
          shapeHight = num;
          EEPROM_writeAnything(   SHAPE_HIGHT      , shapeHight );
        }
        Serial.println(String(num));

        lcd.setCursor(5, 1);    lcd.print("Approved ");
        delay(2000);
        num = 0;
        return true;
      }
    }
    else if (key == 0x7F) {
      indi = ((indi == 0) ? (0) : (indi - 1));
      num = (uint32_t)(num / 10);
      Serial.println("BackSpace...");
      lcd.setCursor(0, 1); lcd.print("                    "); lcd.setCursor(5, 1);
      lcd.print(num);
      Serial.println(num);
    }
    else if (key == '*') {
      return false;
    }
  }
}

