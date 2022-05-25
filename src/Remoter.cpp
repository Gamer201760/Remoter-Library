#include <Remoter.h>

RemoterCfn remotercfn;

void Remoter::begin(char* ip, char* serialKey, bool debug){
    _serialKey = serialKey;
    _debug = debug;
    WiFiClient client;
    HTTPClient http;

    http.begin(client, ip);
    int httpcode = http.GET();
    
    if (httpcode == HTTP_CODE_OK){
        _flag = true;
        _ip = ip;
        HTTPClient http;
        String url = _ip;
        url += "api/getmodulename/";
        url += _serialKey;
        url += "/";

        http.begin(client, url);
        http.addHeader("Content-Type", "text/html");
        int httpcode = http.GET();

        Serial.println(httpcode);
        _mod = http.getString();
    }
    
    if (_debug){
        Serial.println("");
        Serial.println("===================================");
        Serial.print("[DEBUG] HTTPCODE=");
        Serial.println(httpcode);
        Serial.print("[DEBUG] IP=");
        Serial.println(_ip);
        Serial.println("===================================");
    }
    http.end();
}

void Remoter::regModule(char* token){
    if (_flag){
        StaticJsonDocument<1024> data;

        WiFiClient client;
        HTTPClient http;
        String payload;

        String url = _ip;
        url += "api/module/register/";
        
        data["name"] = "Null";
        data["data"] = serialized("{\"temp\": 30}");
        data["controlling"] = serialized("{\"on\": []}");

        if(_mod == "powerStrip"){
            data["pins"] = serialized("[15, 13, 12, 14, 4]");
            Serial.print("is ");
            Serial.println(_mod);
        }
        else if (_mod == "socket"){
            data["pins"] = serialized("[12]");
            Serial.print("is ");
            Serial.println(_mod);
        }
        else {
            data["pins"] = serialized("[]");
            Serial.print("is ");
            Serial.println(_mod);
        }

        data["serialNum"] = String(_serialKey);
        serializeJson(data, payload);

        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", "Token " + String(token));

        int httpcode = http.POST(payload);

        if (_debug) {
            Serial.println("==================================="); 
            Serial.print("[DEBUG] TOKEN HTTPCODE=");
            Serial.println(httpcode); 
            Serial.print("[DEBUG] REG TOKEN");
            Serial.println(String(token));

            Serial.print("[DEBUG] REQUEST DATA ");
            Serial.println(http.getString());
            Serial.println("===================================");
        }
        http.end(); 
    }
}

void Remoter::sendData(StaticJsonDocument<1024> jsondata, char* token){
    if (_flag){
        WiFiClient client;
        HTTPClient http;

        String url = _ip;
        String payload = "data=";
        String data;        

        serializeJson(jsondata, data);
        payload+=data;
        url += "api/module/register/";
        url += _serialKey;
        url += "/";

        http.begin(client, url);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http.addHeader("Authorization", "Token " + String(token));

        int httpcode = http.PUT(payload);

        if (_debug){
            Serial.println("===================================");
            Serial.print("[DEBUG] TOKEN HTTPCODE=");
            Serial.println(httpcode);

            Serial.print("[DEBUG] DATA ");
            Serial.println(payload);

            Serial.print("[DEBUG] REQUEST DATA ");
            Serial.println(http.getString());
            Serial.println("===================================");
        }
        http.end();
    }
}

void Remoter::removeModule(char* token){
    if (_flag){
        WiFiClient client;
        HTTPClient http;

        String url = _ip;
        url += "api/module/delete/";
        url += String(_serialKey) + "/";
    
        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", "Token " + String(token));


        int httpcode = http.sendRequest("DELETE");
        if (_debug) {
            Serial.println("==================================="); 
            Serial.print("[DEBUG] TOKEN HTTPCODE=");
            Serial.println(httpcode); 
            Serial.print("[DEBUG] TOKEN ");
            Serial.println(String(token)); 

            Serial.print("[DEBUG] REQUEST DELETE DATA ");
            Serial.println(http.getString());
            Serial.println("===================================");
        }
        http.end();

    }
}
void Remoter::controlling(char* token){
    if (_flag){
        StaticJsonDocument<1024> jsondata;
        WiFiClient client;
        HTTPClient http;

        String url = _ip;
        url += "api/module/getinfo/";
        url += String(_serialKey) + "/";
    
        http.begin(client, url);
        http.addHeader("Authorization", "Token " + String(token));

        int httpcode = http.GET();

        String payload = http.getString();
        deserializeJson(jsondata, payload);
        for (int thisPin = 0; thisPin < jsondata["controlling"]["on"].size(); thisPin++) {
            int en = jsondata["controlling"]["on"][thisPin];
            pinMode(en, OUTPUT);
            digitalWrite(en, 1);
            if (_debug){
                Serial.print("[INFO] Eneble pin ");
                Serial.println(en);
            }
        }
        for (int thisPin = 0; thisPin < jsondata["controlling"]["off"].size(); thisPin++) {
            int off = jsondata["controlling"]["off"][thisPin];
            pinMode(off, OUTPUT);
            digitalWrite(off, 0);
            if(_debug){
                Serial.print("[INFO] Disable pin ");
                Serial.println(off);
            }
        }
        for (int thisPin = 0; thisPin < jsondata["controlling"]["analog"]["pins"].size(); thisPin++) {
            int pin = jsondata["controlling"]["analog"]["pins"][thisPin];
            int value = jsondata["controlling"]["analog"]["values"][thisPin];
        
            pinMode(pin, OUTPUT);
            analogWrite(pin, value);

            if(_debug){
                Serial.print("[INFO] Analog pin: ");
                Serial.print(pin);
                Serial.print(" Value: ");
                Serial.println(value);
            }
        }

        if (_debug) {
            Serial.println("===================================");
            Serial.print("[DEBUG] TOKEN HTTPCODE=");
            Serial.println(httpcode); 
            Serial.print("[DEBUG] TOKEN ");
            Serial.println(token); 
            Serial.print("[DEBUG] DATA");
            String test;
            serializeJson(jsondata, test);
            Serial.println(test);
            Serial.println("===================================");
        }
        http.end();

    }
}


String Remoter::login(char* username, char* password){
    if (_flag){
        StaticJsonDocument<200> doc;

        WiFiClient client;
        HTTPClient http;

        String url = _ip;
        url += "api/login/";
        String payload = "username=";
        payload += username;
        payload += "&password=";
        payload += password;

        http.begin(client, url);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpcode = http.POST(payload);

        deserializeJson(doc, http.getString());
        String(doc["token"]).toCharArray(remotercfn.token, 41);
        if (_debug) {
            Serial.println("==================================="); 
            Serial.print("[DEBUG] TOKEN HTTPCODE=");
            Serial.println(httpcode); 
            Serial.println("===================================");
        }
        http.end();

        return doc["token"];
    }
}

StaticJsonDocument<1024> Remoter::getJsonData(char* token){
    if (_flag){
        StaticJsonDocument<1024> jsondata;
        WiFiClient client;
        HTTPClient http;

        String url = _ip;
        url += "api/module/getinfo/";
        url += String(_serialKey) + "/";
    
        http.begin(client, url);
        http.addHeader("Authorization", "Token " + String(token));

        int httpcode = http.GET();

        String payload = http.getString();
        deserializeJson(jsondata, payload);

        if (_debug) {
            Serial.println("===================================");
            Serial.print("[DEBUG] TOKEN HTTPCODE=");
            Serial.println(httpcode); 
            Serial.print("[DEBUG] TOKEN ");
            Serial.println(token); 
            Serial.print("[DEBUG] DATA");

            Serial.println("===================================");
        }
        http.end();
        return jsondata;
    }
}
