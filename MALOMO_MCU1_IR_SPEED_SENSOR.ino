#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>

#define WIFI_SSID "PDP Atas"
#define WIFI_PASSWORD "kumahaaa"
#define INFLUXDB_URL "http://10.133.226.235:8086/"
#define INFLUXDB_TOKEN "I_E9baHRmDDWhPqK6aHWQky_GDZcXrc0CeDwjvxZjVTG1Rw5eQ8XSpbMA70WO2VHCcPzcqiigOqWLQ-7PjJ6Aw=="
#define INFLUXDB_ORG "2156806bb2bcc045"
#define INFLUXDB_BUCKET "pama-hev"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

Point rpm_sensor("rpm_sensor");

#define sensorPin 2
volatile unsigned int counter;
static uint32_t previousMillis;
int rpm;

void setup() {
  Serial.begin(115200);
  
  Serial.println("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, RISING);
}

void loop() {

  if (millis() - previousMillis >= 1000) {
    rpm = (counter / 20) * 60;
    counter = 0;
  
    COMMUNICATION();
    previousMillis += 1000;
  }
}
