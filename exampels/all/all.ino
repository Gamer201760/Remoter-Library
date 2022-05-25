#include <Remoter.h>
/*
  Библиотеки которые нам нужны для работы, уже импортированны в Remoter
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <ArduinoJson.h>
*/

char* ip = "http://192.168.0.5/"; // Адрес сервера. Обязательно в char*
char* serialKey = ""; // Сирийный ключ должен обязательно быть в бд или регистрацию не пройдёт

char* username = "";
char* password = "";

unsigned long tmr1;
unsigned long tmr2;
StaticJsonDocument<1024> test;
Remoter remoter;
void setup() {
  Serial.begin(115200);

  WiFi.begin("", ""); // Инициализация Wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  remoter.begin(ip, serialKey, true); // Инициализация библиотеки, последние это ключ debug
  String userToken = remoter.login(username, password); // Получение токена пользователя
  remoter.regModule(remotercfn.token);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - tmr1 >= 2000) { // Таймер срабатывает каждую секунду
    test["temp"] = 12;
    remoter.sendData(test, remotercfn.token);
    tmr1 = currentTime;
  }
  if (currentTime - tmr2 >= 10000) { // Таймер срабатывает каждую секунду    
    remoter.controlling(remotercfn.token);
    tmr2 = currentTime;
  }




  // Теперь у нас каждую секунду отправлются данные на сервер
}
