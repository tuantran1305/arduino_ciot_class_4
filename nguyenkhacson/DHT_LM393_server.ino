//call LIB
#include "WiFi.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include <DHT.h>
//setup pin
#define LED_PIN 5  //led pin
#define LED_LIGHT 17
bool ledState = false;   //led state
#define DHT_PIN 19       //DHT11 pin
#define LM393_D0_PIN 18  // LM393 D0 pin
#define LM393_A0_PIN 36  // LM393 A0 pin
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);
//wifi setup
#define WiFi_SSID "ABC"
#define WiFi_PASS "12345678"
//mqtt setup
#define MQTT_SERVER "thingsboard.cloud"  //đường dẫn đến máy chủ MQTT
#define MQTT_PORT 1883                   //port của thinhboard
#define TOKEN "qw8PTyFhn2NpiX0EHyVo"     //token device


static const char* TOPIC_TELEMETRY = "v1/devices/me/telemetry";
static const char* TOPIC_ATTRIBUTES = "v1/devices/me/attributes";

WiFiClient espClient;
PubSubClient mqtt(espClient);

void connectWiFi() {
  WiFi.mode(WIFI_STA);  //esp connect to wifi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WiFi_SSID, WiFi_PASS);        // id and pass of wifi
  while (WiFi.status() != WL_CONNECTED) {  //if while not connected yet, print....
    Serial.print(".");
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println("Setup_done!");
}

void connectMQTT() {
  while (!mqtt.connected()) {  //while mqtt not connected yet
    Serial.print("[MQTT] connecting to thingboard...");
    if (mqtt.connect("esp32", TOKEN, "")) {
      Serial.println("connected to thingboard!");
      //    mqtt.subscribe(TOPIC_TELEMETRY);
    } else {
      Serial.println("retry");
      delay(1000);
    }
  }
}
void sendTelemetry() {
  float temp = dht.readTemperature();
  Serial.println(temp);
  float humidity = dht.readHumidity();
  Serial.println(humidity);
  int lightLevel = analogRead(LM393_A0_PIN);  // Đọc giá trị analog từ A0 (GPIO36)
  bool lightLed = (digitalRead(LM393_D0_PIN) == HIGH);
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
  digitalWrite(LED_LIGHT, lightLed);
  StaticJsonDocument<256> doc;
  doc["temperature"] = temp;
  doc["humidity"] = humidity;
  doc["ledstate"] = ledState;
  doc["lightLevel"] = lightLevel;   // Giá trị ánh sáng
  doc["lightLedState"] = lightLed;  // Trạng thái LED ánh sáng

  char buf[256];
  size_t n = serializeJson(doc, buf, sizeof(buf));
  mqtt.publish(TOPIC_TELEMETRY, buf, n);
  Serial.println("send thingsboard success");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void setup() {
  Serial.begin(115200);
  Serial.println("start");
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_LIGHT, OUTPUT);
  digitalWrite(LED_LIGHT, LOW);
  pinMode(LM393_D0_PIN, INPUT);  // Đặt D0 là input
  pinMode(LM393_A0_PIN, INPUT);  // Đặt A0 là input cho ADC
  connectWiFi();

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  connectMQTT();

  // put your setup code here, to run once:
}

void loop() {
  sendTelemetry();
  delay(1000);
  // put your main code here, to run repeatedly:
}
