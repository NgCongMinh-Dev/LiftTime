#include <WiFi.h>
#include <MQTT.h>

// pins
int trigger = 7;
int echo = 6;

float duration = 0;
float distance = 0;

// https://components101.com/ultrasonic-sensor-working-pinout-datasheet
float MAX_DISTANCE_CM = 450;
float MAX_DURATION = MAX_DISTANCE_CM * 29.1 * 2;
float THRESHOLD_DOOR_OPEN = 5.0;

// wifi credentials
const char ssid[] = "VinaWLAN";
const char pass[] = "10052018";

WiFiClient net;
MQTTClient client;

// Beispiele
// http://blog.simtronyx.de/arduino-nano-und-der-hc-sr04-ultraschall-entfernungssensor/
// https://home.et.utwente.nl/slootenvanf/2019/11/19/connected-distance-sensor-esp32-module/
// https://funduino.de/nr-10-entfernung-messen
// https://www.smarthomeng.de/entfernungsmessung-auf-basis-eines-esp32-und-smarthomeng

void setup()
{
  Serial.begin (115200);

  // register pins
  //pinMode(trigger, OUTPUT);
  //pinMode(echo, INPUT);

  // connect to wifi
  WiFi.begin(ssid, pass);

  client.begin("broker.shiftr.io", net);

  connect();
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
}

void loop()
{
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  /*
    duration = get_duration();

    Serial.print("Dauer: ");
    Serial.print(duration);
    Serial.println();

    distance = get_distance();
    if (distance > MAX_DISTANCE_CM) {
    Serial.println("Invalid value");
    } else {
    Serial.print("Distanz: ");
    Serial.print(distance);
    Serial.println();
    }
  */

  // TODO remove
  distance = random(7);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println();

  client.publish("door", String(is_door_open(distance)));

  delay(3000);
}

float get_duration() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);

  noInterrupts();
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  duration = pulseIn(echo, HIGH);
  interrupts();

  return duration;
}

float get_distance(float duration) {
  return (duration / 2) / 29.1;
}

bool is_door_open(float distance) {
  return distance <= THRESHOLD_DOOR_OPEN;
}
