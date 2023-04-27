#include "sdcard_interface.h"

static bool sd_error = false;

void SD_save(void)
{

  #if SD_CARD_STATUS == ENABLE
  Serial.println("(Debug)_@saveToSD");
  SD_check();
  if (!sd_error){
    File LOC_fileHistory = SD.open("History.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (LOC_fileHistory) {

      static String smode = " Distance";
      if ( mode == MODE_DISTANCE)
      {
        smode = " Distance";
      }
      else if (mode == MODE_VOLUME)
      {
        smode = " Volume";
      }
      LOC_fileHistory.print(activeUser + smode + String(dataToSendSave) );
      LOC_fileHistory.close();
      
      // print to the serial port too:
      Serial.println(activeUser + smode + String(dataToSendSave) );
    
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println(activeUser + " error opening History.txt");
    }
  }
  
  #endif  //End SD_CARD_STATUS
}

void SD_check(void) 
{
  #if SD_CARD_STATUS == ENABLE
  /****************************SD check*******************************/
  //Check SD card Connectivity
  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // wait 3 sec. and retry connect to SD.
    lcd.setCursor(0, 0);
    lcd.print("SD card Error");
    sd_error = true;
    delay(3000);
  }

  else
  {
    Serial.println("card initialized sucessfully...");
    lcd.setCursor(0, 0);
    lcd.print("             ");
    sd_error = false;
  }
  /*******************************        *******************************/
  #elif SD_CARD_STATUS == DISABLE
  Serial.println("SD card disabled from Configurations...");
  lcd.setCursor(0, 0);
  lcd.print("SD card is disabled");
  #endif  //End SD_CARD_STATUS
}