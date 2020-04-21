
#include <WiFi.h>
#include <MQTTClient.h>
//#include <PubSubClient.h>

// Wi-Fi名とパスワード
const char ssid[] = "XXX";
const char pass[] = "YYY";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  // shiftrのkeyとSecretを入力
  Serial.print("\nconnecting...");
  while (!client.connect("ESP32_sub", "KEY", "SECRET")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // trun on LED
  //digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  //digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  // LED settings
  //pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
/*
  // publish a message roughly every second.
  if (millis() - lastMillis > 3000) {
    lastMillis = millis();
    client.publish("/hello", "world");
  }
*/
  client.onMessage(messageReceived);
}
