#include <WiFi.h>
#include <WebServer.h>
#include <Ascon128.h>


const char* ssid = "Orange-477D";
const char* password = "M7J74L084QG";




const uint8_t key[16] = {
  'm', 'y', 's', 'e', 'c', 'r', 'e', 't', 'k', 'e', 'y', '1', '2', '3', '4', '5'
};
const uint8_t iv[16] = {
  'u', 'n', 'i', 'q', 'u', 'e', '_', 'n', 'o', 'n', 'c', 'e', '_', '1', '2', '\0'
};
const uint8_t authdata[] = "led_control"; 

Ascon128 ascon;

const int ledPin = 2;  
WebServer server(80);

String decryptData(uint8_t* cipherData, int sizeOfArray) {
    unsigned long StartTime = micros();
    uint8_t buffer1[32];
    
    ascon.setKey((uint8_t*)key, 16);
    ascon.setIV((uint8_t*)iv, 16);
    ascon.addAuthData((uint8_t*)authdata, 11); 
    
    ascon.decrypt(buffer1, cipherData, sizeOfArray);
    
    unsigned long ElapsedTime = micros() - StartTime;
    
    return String((char*)buffer1);
}

void handleLed() {

    if (server.method() != HTTP_POST) {
        server.send(405, "text/plain", "Method Not Allowed");
        return;
    }
    String body = server.arg("plain");
    if (body.length() == 0 || body.length() > 50) {
        server.send(400, "text/plain", "Bad payload");
        return;
    }
    uint8_t ciphertext[50];
    int len = body.length();
    for (int i = 0; i < len; i++) {
        ciphertext[i] = (uint8_t)body[i];
    }
    String cmd = decryptData(ciphertext, len);
    String cleanCmd = "";
    for (int i = 0; i < cmd.length(); i++) {
        char c = cmd.charAt(i);
        if (c == '\0' || c < 32 || c > 126) break;
        cleanCmd += c;
    }


    if (cleanCmd == "on") {
        digitalWrite(ledPin, HIGH);
        ok=true;
        server.send(200, "text/plain", "LED ON");
    } else if (cleanCmd == "off") {
        digitalWrite(ledPin, LOW);
        ok=false;
        server.send(200, "text/plain", "LED OFF");
    } else {
        server.send(400, "text/plain", "Unknown command");
    }

}

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    server.on("/led", HTTP_POST, []() {}, handleLed);
    server.begin();
}


void loop() {
    server.handleClient();
}
