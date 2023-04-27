#include "nodemcu_interface.h"

void send2NodeData(void) {
  #if NODE_MCU_STATUS == 1
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
#endif
}
