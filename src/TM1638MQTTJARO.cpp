#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <TM1638plus_Model2.h>
#include "wifidata.h"

#ifndef TM1638_STB
#define TM1638_STB 4
#endif
#ifndef TM1638_CLK
#define TM1638_CLK 5
#endif
#ifndef TM1638_DIO
#define TM1638_DIO 6
#endif

static TM1638plus_Model2 tm(TM1638_STB, TM1638_CLK, TM1638_DIO, false, false);

static WiFiClient wifiClient;
static PubSubClient mqtt(wifiClient);

static char msgBuffer[51];
static uint8_t lastButtons = 0;
static bool shutterMoving[8] = {false, false, false, false, false, false, false, false};

static void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  tm.DisplayStr("WIFI");
  Serial.print("Connecting to Wi-Fi");

  uint32_t started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 30000) {
    delay(250);
    Serial.print('.');
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nConnected: %s\n", WiFi.localIP().toString().c_str());
    tm.DisplayStr("WIFI OK");
  } else {
    Serial.println("\nWi-Fi connection timed out");
    tm.DisplayStr("NO WIFI");
  }
}

static void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (length > 49) length = 49;

  if (strcmp(topic, "TM1638/message") == 0) {
    memcpy(msgBuffer, payload, length);
    msgBuffer[length] = '\0';
    tm.DisplayStr(msgBuffer, 0);
  } else if (strcmp(topic, "TM1638/brightness") == 0) {
    char value = '0';
    if (length > 0) value = static_cast<char>(payload[0]);
    int brightness = value - '0';
    if (brightness >= 0 && brightness <= 7) tm.brightness(brightness);
  }
}

static bool reconnectMqtt() {
  if (mqtt.connected()) return true;

  String clientId = "TM1638-C3-";
  clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

  Serial.print("Connecting to MQTT...");
  if (mqtt.connect(clientId.c_str())) {
    Serial.println("connected");
    mqtt.publish("TM1638/pub", "TM1638 ESP32-C3 ready.");
    mqtt.subscribe("TM1638/message");
    mqtt.subscribe("TM1638/brightness");
    tm.DisplayStr("MQTT OK");
    return true;
  }

  Serial.printf("failed, rc=%d\n", mqtt.state());
  tm.DisplayStr("NO MQTT");
  return false;
}

static void publishShutterButton(uint8_t shutter, const char* action) {
  char topic[40];
  snprintf(topic, sizeof(topic), "cmd/jarolift/shutter/%u", shutter);
  mqtt.publish(topic, action);
  Serial.printf("%s -> %s\n", topic, action);
}

static void handleButton(uint8_t button) {
  if (button < 1 || button > 16) return;

  uint8_t shutter = (button - 1) / 2;
  if (shutter > 7) return;

  bool downButton = ((button - 1) & 1) != 0;

  if (shutterMoving[shutter]) {
    publishShutterButton(shutter, "STOP");
    shutterMoving[shutter] = false;
  } else {
    publishShutterButton(shutter, downButton ? "DOWN" : "UP");
    shutterMoving[shutter] = true;
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  tm.displayBegin();
  tm.brightness(3);
  tm.reset();
  tm.DisplayStr("START");

  setupWifi();
  mqtt.setServer(mqtt_server, 1883);
  mqtt.setCallback(mqttCallback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setupWifi();
  }

  if (!mqtt.connected()) {
    reconnectMqtt();
  }
  mqtt.loop();

  uint8_t buttons = tm.ReadKey16();

  if (buttons != 0 && buttons != lastButtons) {
    tm.DisplayDecNum(buttons, 0, false);
    snprintf(msgBuffer, sizeof(msgBuffer), "%u", buttons);
    mqtt.publish("TM1638/button", msgBuffer);
    handleButton(buttons);
  }

  lastButtons = buttons;
  delay(200);
}
