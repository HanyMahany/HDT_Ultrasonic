
#include "max485_interface.h"

void max485_init(void)
{
  #if MAX485_STATUS == ENABLE
  Serial2.begin(9600);
  Serial.println("Init Max485 line");
  #endif
}
void max485_send(void)
{
  #if MAX485_STATUS == ENABLE
  Serial2.println(String(dataToSendSave));
  Serial.println("To Max 485: "+String(dataToSendSave));
  #endif
}