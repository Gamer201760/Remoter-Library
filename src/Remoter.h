#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

struct RemoterCfn
{
    char token[41] = "";
};
extern RemoterCfn remotercfn;



class Remoter {
    public:
        void begin(char* ip, char* serialKey, bool debug);
        void regModule(char* token);
        void sendData(StaticJsonDocument<1024> jsondata, char* token);
        void removeModule(char* token);
        void controlling(char* token);

        String login(char* username, char* password);
        StaticJsonDocument<1024> getJsonData(char* token);

    private:
        bool _flag = false;
        bool _debug = true;

        char* _serialKey;
        char* _ip;
        String _mod;

};