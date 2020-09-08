#######################################
#                                     #
#           Code by TekkieBros        #
#                                     #
#             Visit us on             #
#       https://www.tekkiebros.de     #
#                                     #
#######################################

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
//###########################################################################
#define button 12     // Button for Webupdate
#define mqtt_user "mqtt-hassio"
#define mqtt_password "SHit!2018#"
//###########################################################################
// Wifi
const char* ssid = "WLAN-TiloLeoNils";
const char* password = "4677339572444545";
// MQTT
const char* mqtt_server = "192.168.2.57";
const char* mqtt_clientId = "dash";
const char* outTopicMsg = "tekkiebros/dashbutton1/message";
const char* outTopicVCC = "tekkiebros/dashbutton1/vcc";
// Misc
char msg[50];                       // message for mqtt publish
//###########################################################################

ADC_MODE (ADC_VCC);                 // VCC Read

WiFiClient espClient;
PubSubClient client(espClient);

void setup(){ 
  pinMode(button, INPUT_PULLUP);                          
  Serial.begin(115200);  

// Setup Wifi
  setup_wifi();

// Setup MQTT
  client.setServer(mqtt_server, 1883);
} 

void setup_wifi() {
  int wifiCounter = 0;
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if(wifiCounter < 10){
      delay(500);
      Serial.print(".");
    }else{ESP.deepSleep(0, WAKE_RFCAL);}
    wifiCounter++;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Client ID connected
    if (client.connect(mqtt_clientId, mqtt_user, mqtt_password)) {
      Serial.print(mqtt_clientId);
      Serial.println(" connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//###########################################################################

void loop(){ 
  // MQTT Client
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
// Read the VCC from Battery
  float vcc = ESP.getVcc() / 1000.0;
  vcc = vcc - 0.12;     // correction value from VCC

  if(digitalRead(button == HIGH)){
    client.publish(outTopicMsg, "ON");
    delay(50);
    client.publish(outTopicMsg, "OFF");
    delay(50);
    dtostrf(vcc, sizeof(vcc), 2, msg);
    client.publish(outTopicVCC, msg);
    delay(50);
    ESP.deepSleep(0, WAKE_RFCAL);
  }

}
