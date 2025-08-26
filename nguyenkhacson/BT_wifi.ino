#include "WiFi.h"

const char* ssid = "ABC";
const char* password = "12345678";
const char* ap_ssid = "ESP32_AP";
const char* ap_password = "12345678";

void setup(){
    Serial.begin(115200)
    Serial.println("connecting to WiFi");
    WiFi.mode(WIFI_STA_AP);
    connect();
    shareWiFi();
    Serial.println("WIFI DONE!!!")
}

void connect(){
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
        Serial.print("."):
        delay(500);
    }
    Serial.println("Connected to WiFi"):
}

void shareWiFi() {
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("Access hostpot started!");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
}

void loop() {
    Serial.println("Scanning...");
    int n = WiFi.scanNetworks(); // scan tìm kiếm wifi
    Serial.println("Scan done!");
    if (n == 0) {
        Serial.println("No networks found.");
    } else {
        Serial.println();
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i)); // tên wifi
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i)); // cường độ wifi (rssi)
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    delay(5000); // Wait a bit before scanning again
}