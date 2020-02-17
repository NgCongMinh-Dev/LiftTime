#include <WiFi.h>
#include <MQTT.h>

// wifi credentials
const char ssid[] = "VinaWLAN";
const char pass[] = "10052018";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

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
    if (millis() - lastMillis > 1000) {
      lastMillis = millis();
      client.publish("/hello", "world");
    }
  */

  /*
    if (millis() - lastMillis > 5000) {
      int sensorValue_1=random(100);  // replace with your sensor value
      int sensorValue_2=random(100);  // replace with your sensor value
      client.publish("temperature", String(sensorValue_1),true,1);
      client.publish("humidity", String(sensorValue_1),true,1);
      lastMillis = millis();
    }
  */
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "df1c422c", "caafed03af1b983e")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/door");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "/door") {
    processDoorTopicMessage(payload);
  }
}

void processDoorTopicMessage(String message) {
  if (message.toInt() == 1) {
    Serial.println("Door opened");

  } else {
    Serial.println("Door closed");

  }
}
