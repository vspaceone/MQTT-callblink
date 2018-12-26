#include <FS.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.h"

#define DEBUG // Comment out if compiling for ESP01 with full functionality (on all GPIOs)

#define output_Flash 0
#define output_EVS 1
#define output_BellLoud 2
#define output_BellQuiet 3

#define blinkMode_Off 0
#define blinkMode_Flash 1
#define blinkMode_EVS 2

#define bellMode_Off 0
#define bellMode_Quiet 1

WiFiClient espClient;
PubSubClient mqttClient(espClient);

int blinkMode = blinkMode_Off;
int bellMode = bellMode_Off;

void connectWiFi() {
  delay(10);

#ifdef DEBUG
  Serial.println("");
#endif

  WiFi.begin(wifi_ssid, wifi_password);

#ifdef DEBUG
  Serial.print("Connecting");
#endif
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    ESP.wdtFeed();
#ifdef DEBUG
    Serial.print(".");
#endif
  }
  
#ifdef DEBUG
  Serial.println("");
  Serial.print("WiFi connected ");
  Serial.println(WiFi.localIP());
  Serial.print("MQTT configured to ");
  Serial.print(mqtt_server);
  Serial.print(" with user ");
  Serial.println(mqtt_user);
#endif
}

void connectMQTT() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
#ifdef DEBUG
    Serial.print("Attempting MQTT connection...");
#endif
    // Attempt to connect
    if (mqttClient.connect("vspace.one.callblink", mqtt_user, mqtt_password)) {
#ifdef DEBUG
      Serial.println("connected");
#endif
    } else {
#ifdef DEBUG
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
#endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void checkMqtt(){
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
}

// ##############################
// Arduino "main" functions
// ##############################

void setup() {
#ifndef DEBUG
  pinMode(1, FUNCTION_3); 
  pinMode(3, FUNCTION_3); 
#endif

  // State switch
  pinMode(output_Flash, OUTPUT); 

#ifdef DEBUG
  Serial.begin(9600);

  while (!Serial){
    ESP.wdtFeed();
  };
#endif

  connectWiFi();

  mqttClient.setServer(mqtt_server, 1883);
  
  checkMqtt();

  mqttClient.subscribe("vspace/one/phone",1);
  mqttClient.setCallback([=](char* topic, byte* payload, unsigned int length){
    std::string s( reinterpret_cast<char const*>(payload), length);
    DynamicJsonBuffer jsonBuffer;
    JsonObject& response = jsonBuffer.parseObject(s.c_str());

    if (!response.success()) {
      return;
    }        

    const int kind = response["kind"];

    switch (kind){
      case 0:
        blinkMode = blinkMode_Off;
        break;
      case 1:
        blinkMode = blinkMode_Flash;
        break;
      case 2:
        blinkMode = blinkMode_Off;
        break;
      case 4:
        blinkMode = blinkMode_Off;
        break;
    }

  });
  checkMqtt();
}

void loop() {
  checkMqtt();

  switch (blinkMode){
    case blinkMode_Off:
      digitalWrite(output_Flash, LOW);
      //digitalWrite(output_EVS, LOW);
      break;
    case blinkMode_Flash:
      digitalWrite(output_Flash, HIGH);
      break;
    case blinkMode_EVS:
      //digitalWrite(output_EVS, HIGH);
      break;
  }

  switch (bellMode){
    case bellMode_Off:
      break;
    case bellMode_Quiet:
      break;
  }

  delay(500);
  ESP.wdtFeed();
}
