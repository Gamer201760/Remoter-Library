#include <SimplePortal.h>
#include <ArduinoJson.h>
#include <EncButton.h>
#include <EEManager.h>
#include <Remoter.h>
#include "token.h"

unsigned long tmr1;
char* ip = "http://192.168.0.5/";
bool _removeflag = false;
Remoter remoter;
EEManager EEwifi(portalCfg);
EncButton<EB_TICK, 15> btn;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(2048);

  checkButton();
  Serial.println(portalCfg.mode);
  Serial.println(portalCfg.SSID[0]);
  Serial.print("UserToken = ");
  Serial.println(portalCfg.UserToken);
  if (portalCfg.mode == WIFI_STA || (portalCfg.mode == WIFI_STA && portalCfg.SSID[0] == '\0')) wifi_connect();
  
  
  
//
//  remoter.regModule();
//  StaticJsonDocument<200> data;
//
//  data["egg"] = 10;
//  remoter.sendData(data);


}

void loop() {
  if (millis() - tmr1 > 5000){
    tmr1 = millis();
    remoter.controlling(portalCfg.UserToken);
  }
}
