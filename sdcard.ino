#include "sdcard_interface.h"

void saveToSD(void)
{

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

void checkSD(void) 
{
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