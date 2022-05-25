#include <Remoter.h>
/*
Библиотеки которые нам нужны для работы, уже импортированны в Remoter
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
*/

char* url = "http://192.168.0.5/"; // Адрес сервера. Обязательно в char*
char* serialKey = ""; // Сирийный ключ должен обязательно быть в бд или регистрацию не пройдёт

char* username = "Имя пользователя";
char* password = "Пароль пользователя";

unsigned long tmr1;

Remoter remoter;
void setup() {
  Serial.begin(115200);

  WiFi.begin("WiFi SSID", "Wifi password"); // Инициализация Wifi
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  
  remoter.begin(url, serialKey, true); // Инициализация библиотеки, последние это ключ debug
  String userToken = remoter.login(username, password); // Получение токена пользователя
}

void loop() {
  StaticJsonDocument<1024> data;
  data["temp"] = 10.23;
  if (millis() - tmr1 > 1000){ // Таймер срабатывает каждую секунду
    tmr1 = millis();
    remoter.sendData(data); // Данные обязательно в Json формате, рекомендую к изучению библиотеку ArduinoJson
  }
  // Теперь у нас каждую секунду отправлются данные на сервер
}
