void portalRoutine() {
  unsigned long tmr3;
  portalStart();
  Serial.println("Portal start");

  while (!portalTick()) Serial.println("Wait user");
  if (portalStatus() <= 3) EEwifi.updateNow();

  Serial.print("Portal status: ");
  Serial.println(portalStatus());
  wifi_connect();
  if (millis() - tmr3 > 100) {
    if (_removeflag) {
      Serial.println("Remove");
      remoter.removeModule(portalCfg.UserToken);
      delay(10);
      String token = remoter.login(userportalCfg.username, userportalCfg.password);
      token.toCharArray(portalCfg.UserToken, 41);
      //      portalCfg.UserToken = remotercfn.token;
      //      strcpy(portalCfg.UserToken, remotercfn.token);
      delay(10);
      remoter.regModule(remotercfn.token);
    } else {
      String token = remoter.login(userportalCfg.username, userportalCfg.password);
      token.toCharArray(portalCfg.UserToken, 41);

      //      strcpy(portalCfg.UserToken, remotercfn.token);
      delay(10);
      remoter.regModule(remotercfn.token);
    }
  }
  //  remoter.login(userportalCfg.username, userportalCfg.password);
  //  token.toCharArray(portalCfg.UserToken, 40);
  portalCfg.mode = WIFI_STA;
  //  portalCfg.UserToken = remotercfn.token;
  EEwifi.updateNow();
  //  remoter.regModule();
  //  Serial.print("Mode = ");
  //  Serial.println(portalCfg.mode);
}

void wifi_connect() {

  if (WiFi.isConnected()) {
    Serial.println("WiFi already connected");
  }
  else {
    Serial.print("Wifi Connecting");
    WiFi.begin(portalCfg.SSID, portalCfg.password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.print("Wifi Name: ");
    Serial.print(portalCfg.SSID);
    Serial.print(" Wifi Password: ");
    Serial.print(portalCfg.password);
    Serial.print(" IP: ");
    Serial.println(WiFi.localIP());

  }

  remoter.begin(ip, serialKey, true);
}

void checkButton() {
  if (EEwifi.begin(0, 'a')) {
    portalRoutine();
  } else {
    Serial.println("Start checkbutton");
    uint32_t tmr = millis();
    while (millis() - tmr < 2000) {
      btn.tick();
      Serial.println(btn.state());
      if (btn.state() == 0) {
        _removeflag = true;
        portalRoutine();
      }
    }
  }
}
