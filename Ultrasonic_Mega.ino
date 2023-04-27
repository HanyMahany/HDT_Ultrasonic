#include "includes.h"

void setup() 
{
  // Serial initialization
  Serial.begin(115200);
  max485_init();
  nodemcu_init();
  Serial.println("(Debug)_@setup");
  
  // Initializing Trigger Output and Echo Input
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT_PULLUP);
  
  // Keyboard initialization
  keyboard.begin(KEYBOARD_DATA, KEYBOARD_CLK);
  
  // SD card initialization
  SD_check();
  
  // LCD I2C initialization
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Hyber SCADA");
  lcd.setCursor(0, 1);
  lcd.print("Tech Makers HDTP0122");
  delay(DEALY_WELCOME_MS);

  EEPROM_scanParameters();

  if ( mode == MODE_DISTANCE ) 
  {
    Serial.println("MODE : Distance");
  }
  else if (mode == MODE_VOLUME)
  {
    Serial.println("MODE : Volume");
    Serial.println("SHAPE_FULL_VOLUME : " + String (shapeTotalVolume));
    Serial.println("SHAPE_HIGHT : " + String (shapeHight) );
  }
  
  t = millis();
  while (param_getUser() && (millis() - t <=  WAITING_PASS_TIME)) {
    t = millis();
  }
  if (activeUser.equals(USER_ADMIN_NAME)) {
    
    while (1) {
      lcd.setCursor(0, 0); lcd.print("1.System Mode");
      lcd.setCursor(0, 1); lcd.print("2.System Parameters.");
      delay(1000);
      key = getChar();
      if (key == '1') {
        //Shape Config.
        lcd.clear();
        while (1) {
          lcd.setCursor(0, 0); lcd.print("1.DISTANCE");
          lcd.setCursor(0, 1); lcd.print("2.VOLUME");
          key = getChar();
          if (key == '1' ) {
            mode = MODE_DISTANCE;
            EEPROM_writeAnything(MODE, mode );
            Serial.println("DISTANCE is Chosen");
            lcd.clear();
            lcd.setCursor(0, 1); lcd.print("Mode: DISTANCE");
            delay(1000);
            break;
          }
          else if (key == '2' ) {
            mode = MODE_VOLUME;
            EEPROM_writeAnything(MODE, mode );
            Serial.println("VOLUME is Chosen");
            lcd.clear();
            lcd.setCursor(0, 1); lcd.print("Mode: VOLUME");
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
        //Parameters
        lcd.clear();
        while (1) {
          lcd.setCursor(0, 0); lcd.print("1.Total Volume");
          lcd.setCursor(0, 1); lcd.print("2.Total Height");
          key = getChar();
          if (key == '1' ) 
          {
            lcd.clear();
            param_getVolumeHight(VOLUME);
            continue;
          }
          else if (key == '2' ) 
          {
            lcd.clear();
            param_getVolumeHight(HIGHT);
            continue;
          }
          else if (key == '*' ) {
            break;
          }
        }
        
        continue;
      }
      else if (key == '*') {
        //ESC
        activeUser = USER_OPERATION_NAME;
        break;
      }
    }
  }
  EEPROM_scanParameters();
  printParameters();
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 1);
  if ((mode == MODE_DISTANCE))
  {
    app_level_getDistanceCM();  
    dataToSendSave = CalculatedHight;
    lcd.print("Height: ");
  }
  
  else if ((mode  == MODE_VOLUME))
  {
    app_level_Volume(); 
    dataToSendSave = CalculatedVolume;
    lcd.print("Volume: ");
  }
  lcd.print(dataToSendSave, 3);
  SD_save();
  nodemcu_send();
  delay(1500);
}

/**************************  Tested  *******************/
char getChar (void) {
  if (keyboard.available()) {
    return  (char)keyboard.read();
  }
}
void printParameters (void){
  Serial.println("mode          : "+ String(mode));
  Serial.println("MODE_VOLUME   : "+ String(MODE_VOLUME));
  Serial.println("MODE_DISTANCE : "+ String(MODE_DISTANCE));
  Serial.println("VOLUME  : "+ String(VOLUME));
  Serial.println("HIGHT   : "+ String(HIGHT));
  Serial.println("MODE             : "+ String(MODE             ));
  Serial.println("SHAPE_FULL_VOLUME: "+ String(SHAPE_FULL_VOLUME));
  Serial.println("SHAPE_HIGHT      : "+ String(SHAPE_HIGHT      ));  
}



