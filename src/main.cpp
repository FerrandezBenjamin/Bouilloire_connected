#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ArduinoJSON.h>

const int LEDJAUNE = 5;
const int LEDROUGE = 4;
const int LEDVERTE = 0;
const int LEDBLEU = 12;
const int oneWireBus = 14;
const int BAUDRATE = 115200;
const int PORT = 1883;

// WiFi Connection configuration
char ssid[] = "iPhone de Yassa"; //  le nom du reseau WIFI
char password[] = "yassa123@@";  // le mot de passe WIFI

// mqtt server
char mqtt_server[] = "localhost"; // adresse IP serveur
#define MQTT_USER ""
#define MQTT_PASS ""

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// mqtt
WiFiClient espClient;
PubSubClient MQTTclient(espClient);

// Fonctions mqtt

void MQTTsend()
{
  Serial.println("Dans MQTTsend");
  char buffer[512];
  DynamicJsonDocument docMqtt(512);
  docMqtt["temperature"] = "20";
  docMqtt["humidity"] = "40";
  size_t n = serializeJson(docMqtt, buffer);
  MQTTclient.publish("hello", buffer, n);
  Serial.println("FIN MQTTsend");
  delay(1000);
}

void MQTTcallback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Je suis dans MQTTCallBack");
  Serial.print("Message MQTT [");
  Serial.print(topic);
  Serial.print("] ");

  // DynamicJsonDocument docMqtt(512);
  // deserializeJson(docMqtt, payload, length);
  // String msg, title;
  // msg = docMqtt["msg"] | "";
  // title = docMqtt["title"] | "";
  // Serial.println("message reçu : " + msg + " et titre reçu " + title);
}

// void startled(float temp)
// {
//   if (temp >= 20 && temp < 25)
//   {
//     // start led bleu -- eteindre les autres
//     digitalWrite(LEDBLEU, HIGH);
//     // eteindre les autres
//     digitalWrite(LEDJAUNE, LOW);
//     digitalWrite(LEDROUGE, LOW);
//     digitalWrite(LEDVERTE, LOW);
//   }
//   else if (temp >= 25 && temp < 30)
//   {
//     // start vert -- eteindre les autres
//     digitalWrite(LEDVERTE, HIGH);
//     // eteindre les autres
//     digitalWrite(LEDROUGE, LOW);
//     digitalWrite(LEDBLEU, LOW);
//     digitalWrite(LEDJAUNE, LOW);
//   }
//   else if (temp >= 30 && temp < 35)
//   {
//     // start jaune -- eteindre les autres
//     digitalWrite(LEDJAUNE, HIGH);
//     // eteindre les autres
//     digitalWrite(LEDROUGE, LOW);
//     digitalWrite(LEDVERTE, LOW);
//     digitalWrite(LEDBLEU, LOW);
//   }
//   else if (temp >= 35 && temp < 40)
//   {
//     // start red -- eteindre les autres
//     digitalWrite(LEDROUGE, HIGH);
//     // eteindre les autres
//     digitalWrite(LEDVERTE, LOW);
//     digitalWrite(LEDBLEU, LOW);
//     digitalWrite(LEDJAUNE, LOW);
//   }
//   else
//   {
//     // start all led pour les faires clignoter
//     digitalWrite(LEDROUGE, HIGH);
//     digitalWrite(LEDVERTE, HIGH);
//     digitalWrite(LEDBLEU, HIGH);
//     digitalWrite(LEDJAUNE, HIGH);

//     delay(250);

//     digitalWrite(LEDROUGE, LOW);
//     digitalWrite(LEDVERTE, LOW);
//     digitalWrite(LEDBLEU, LOW);
//     digitalWrite(LEDJAUNE, LOW);
//     delay(250);
//   }
// }

void setup()
{
  Serial.begin(BAUDRATE);
  // sensors.begin();
  // pinMode(LEDJAUNE, OUTPUT);
  // pinMode(LEDROUGE, OUTPUT);
  // pinMode(LEDVERTE, OUTPUT);
  // pinMode(LEDBLEU, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println('Je suis avant la connexion WIFI');

  MQTTclient.setServer(mqtt_server, PORT);
  MQTTclient.setCallback(MQTTcallback);
  setup_wifi();
  // MQTTsend();
  Serial.println('Je suis après la connexion WIFI');
  Serial.println("Connected MQTT");
}

void loop()
{
  // sensors.requestTemperatures();
  // float temperatureC = sensors.getTempCByIndex(0);
  // Serial.print(temperatureC);
  // Serial.println(" C");

  // startled(temperatureC);
  MQTTsend();
}
