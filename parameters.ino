#include "parameters_interface.h"

/**************************  Tested  *******************/
bool getUser (void)
{
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

bool getSectorAreaHight(uint8_t id) 
{
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

void getDistanceCM(void) 
{
  digitalWrite(TRIG_PIN, LOW); // Set the trigger pin to low for 2uS
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH); // Send a 10uS high to trigger ranging
  delayMicroseconds(20);

  digitalWrite(TRIG_PIN, LOW); // Send pin low again
  double distance = pulseIn(ECHO_PIN, HIGH); // Read in times pulse

  CalculatedHight = distance / 58; //Convert the pulse duration to distance
  //You can add other math functions to calibrate it well
  CalculatedVolume = shapeSectorArea * (shapeHight - CalculatedHight );
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Volume:");
  lcd.setCursor(5, 2);
  lcd.print(CalculatedVolume,4);
  lcd.print(" m3");
  
  Serial.println("Height");
  Serial.println(CalculatedHight,3);
  Serial.println("shapeSectorArea");
  Serial.println(shapeSectorArea,3);
  Serial.println("shapeHight");
  Serial.println(shapeHight,3);
  delay(500);
}
