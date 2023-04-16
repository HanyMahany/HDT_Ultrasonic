
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define ENABLE 1
#define DISABLE 0

#define SD_CARD_STATUS ENABLE
#define CHIP_SELECT 53

#define USER_ADMIN_NAME       "ADMIN"
#define USER_OPERATION_NAME   "OPERATOR"

#define USER_ADMIN_PASS      "122333"
#define USER_OPERATION_PASS  "123"

#define TRIG_PIN  9
#define ECHO_PIN  8

#define KEYBOARD_DATA 4
#define KEYBOARD_CLK  3

#define DEALY_WELCOME_MS  3000
#define WAITING_PASS_TIME 5000
/*
 * 0 disable
 * 1 Enable
*/
#define NODE_MCU_STATUS 1
#define DNS "41.165.125.1.30"
#define CHANNEL 200
#endif
