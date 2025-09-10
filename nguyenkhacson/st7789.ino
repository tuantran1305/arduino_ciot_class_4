#include "WiFi.h"
#include "PubSubClient.h"
#include <Arduino_GFX_Library.h>
#include <Arduino.h>
#include "ArduinoJson.h"

#include <SPI.h>
#include <DHT.h>

#define LCD_CS 5
#define LCD_DC 2
#define LCD_RST 4
#define LCD_SDA 23
#define LCD_SCK 18
#define RELAY_PIN 35
bool relayState = false;
#define DHT_PIN 19
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);
#define LM393_D0_PIN 39
#define LM393_A0_PIN 36
#define LED_LIGHT 26

#define WiFi_SSID "ABC"
#define WiFi_PASS "12345678"
#define MQTT_SERVER "thingsboard.cloud"  //đường dẫn đến máy chủ MQTT
#define MQTT_PORT 1883                   //port của thinhboard
#define TOKEN "qw8PTyFhn2NpiX0EHyVo"

static const char* TOPIC_TELEMETRY = "v1/devices/me/telemetry";
static const char* TOPIC_ATTRIBUTES = "v1/devices/me/attributes";
// Khởi tạo SPI_TFT
Arduino_DataBus* bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_SDA, -1);
Arduino_GFX* gfx = new Arduino_ST7789(bus, LCD_RST, 2 /* rotation */, true /* IPS */, 128, 128, 0, 0);
// Khởi tạo WiFi và MQTT
WiFiClient espClient;
PubSubClient mqtt(espClient);


//Thiết lập WiFi
void connectWiFi() {
  WiFi.mode(WIFI_STA);  // esp connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WiFi_SSID, WiFi_PASS);  //ID và Mật khẩu WiFi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1500);
  }
  Serial.println(WiFi.localIP());
  Serial.println("Success Connection!");
}
//Thiết lập MQTT
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.println("Connecting to MQTT Thingsboard....");
    if (mqtt.connect("ESP32", TOKEN, "")) {
      Serial.println("MQTT connected to Thingsboard");
    } else {
      Serial.print("retry");
      delay(2000);
    }
  }
}
//Thiết lập SPI
void SPI_TFT(float temperature, float humidity, bool lightLed, bool relayState, int LightRange) {
  gfx->fillScreen(BLACK);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2);

  gfx->setTextColor(YELLOW);
  gfx->setCursor(10, 10);
  gfx->printf("T:%.1fC", temperature);

  gfx->setTextColor(BLUE);
  gfx->setCursor(10, 35);
  gfx->printf("H:%.1f%%", humidity);

  gfx->setTextColor(RED);
  gfx->setCursor(10, 60);
  gfx->printf("LED:%s", lightLed ? "ON" : "OFF");

  gfx->setTextColor(GREEN);
  gfx->setCursor(10, 85);
  gfx->printf("Relay:%s", relayState ? "HIGH" : "LOW");

  gfx->setTextColor(WHITE);
  gfx->setCursor(10, 110);
  gfx->printf("Light:%d%%", LightRange);
}
//Thiết lập Telementry
void sendTelemetry() {
  float temperature = dht.readTemperature();
  Serial.println(temperature);
  float humidity = dht.readHumidity();
  Serial.println(humidity);
  int raw = analogRead(LM393_A0_PIN);          // Đọc giá trị từ chân A0
  int LightRange = map(raw, 0, 4095, 0, 100);  // Chuyển đổi giá trị ADC sang từ 0-100
  Serial.println(LightRange);
  bool lightLed = (digitalRead(LM393_D0_PIN) == HIGH);
  Serial.println(lightLed);
  digitalWrite(LED_LIGHT, lightLed);
  // Hiển thị lên màn hình TFT
  SPI_TFT(temperature, humidity, lightLed, relayState, LightRange);
  // Gửi dữ liệu lên ThingsBoard
  StaticJsonDocument<256> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["LightRange"] = LightRange;
  doc["lightLed"] = lightLed;
  doc["relay"] = relayState;  // Gửi trạng thái relay lên ThingsBoard
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

  // Điều khiển relay qua RPC
  if (strstr(topic, "rpc/request")) {
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    if (!error && doc["method"] == "setRelay") {
      bool relayOn = doc["params"];
      digitalWrite(RELAY_PIN, relayOn ? HIGH : LOW);
      relayState = relayOn;
      Serial.print("Relay set to: ");
      Serial.println(relayOn ? "ON" : "OFF");
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("start");
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_LIGHT, OUTPUT);
  pinMode(LM393_D0_PIN, INPUT);
  pinMode(LM393_A0_PIN, INPUT);  // Đặt A0 là input cho ADC
  digitalWrite(LED_LIGHT, HIGH);
  digitalWrite(RELAY_PIN, LOW);
  // Khởi tạo màn hình TFT
  gfx->begin(40000000);    // SPI tốc độ cao
  gfx->fillScreen(BLACK);  // Xóa màn hình
  connectWiFi();

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  connectMQTT();
  // Hiển thị giá trị mặc định lên màn hình khi khởi động
  SPI_TFT(0.0, 0.0, false, false, 0);
  // put your setup code here, to run once:
}

void loop() {
  sendTelemetry();
  delay(1000);
  // put your main code here, to run repeatedly:
}
