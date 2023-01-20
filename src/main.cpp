#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ArduinoJSON.h>

const int LEDJAUNE = 4;
const int LEDROUGE = 5;
const int LEDVERTE = 12;
const int LEDBLEU = 0;
const int LEDNOIR = 15;

const int oneWireBus = 14;
const int BAUDRATE = 115200;

const int PORT = 1883;

// WiFi Connection configuration
#define wifi_ssid "Redmi Note 10 Pro"
#define wifi_password "chipouille"

#define mqtt_server "192.168.241.215"
#define mqtt_user "fidel"   // s'il a été configuré sur Mosquitto
#define mqtt_password "123" // idem

#define temp "temp"
#define blacktea "blacktea"

// function declartion
void callback(char *topic, byte *payload, unsigned int length);
void setup_wifi();
void reconnect();

// Buffer qui permet de décoder les messages MQTT reçus
char message_buff[100];

long lastMsg = 0; // Horodatage du dernier message publié sur MQTT
long lastRecu = 0;
bool debug = false; // Affiche sur la console si True

// char ssid[] = "iPhone de Yassa"; //  le nom du reseau WIFI
// char password[] = "yassa123@@";  // le mot de passe WIFI

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// mqtt
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(BAUDRATE); // Facultatif pour le debug
  // sensors.begin();
  pinMode(LEDJAUNE, OUTPUT);
  pinMode(LEDROUGE, OUTPUT);
  pinMode(LEDVERTE, OUTPUT);
  pinMode(LEDBLEU, OUTPUT);
  // String msgtest = "coucou";
  setup_wifi();                        // On se connecte au réseau wifi
  client.setServer(mqtt_server, 1883); // Configuration de la connexion au serveur MQTT
  client.setCallback(callback);
}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connexion a ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connexion WiFi etablie ");
  Serial.print("=> Addresse IP : ");
  Serial.print(WiFi.localIP());
}

// Connexion MQTT
// Reconnexion
void reconnect()
{
  Serial.println("Reconnect...");
  Serial.print("Etat du client : ");
  Serial.println(client.connected());
  // Boucle jusqu'à obtenur une reconnexion
  if (!client.connected())
  {
    Serial.print("Connexion au serveur MQTT...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password))
    {
      Serial.println("OK");
    }
    else
    {
      Serial.print("KO, erreur : ");
      Serial.print(client.state());
      Serial.println(" On attend 5 secondes avant de recommencer");
      delay(5000);
    }
    Serial.print("Fin reconnect. Etat du client : ");
    Serial.println(client.connected());
  }
}

void startled(float tempp)
{
  if (tempp >= 20 && tempp < 25)
  {
    // start led vert -- eteindre les autres
    digitalWrite(LEDVERTE, HIGH);
    // eteindre les autres
    digitalWrite(LEDBLEU, LOW);
    digitalWrite(LEDJAUNE, LOW);
    digitalWrite(LEDROUGE, LOW);
  }
  else if (tempp >= 25 && tempp < 30)
  {
    // start bleu -- eteindre les autres
    digitalWrite(LEDBLEU, HIGH);
    // eteindre les autres
    digitalWrite(LEDVERTE, LOW);
    digitalWrite(LEDROUGE, LOW);
    digitalWrite(LEDJAUNE, LOW);
  }
  else if (tempp >= 30 && tempp < 35)
  {
    // start jaune -- eteindre les autres
    digitalWrite(LEDJAUNE, HIGH);
    // eteindre les autres
    digitalWrite(LEDROUGE, LOW);
    digitalWrite(LEDVERTE, LOW);
    digitalWrite(LEDBLEU, LOW);
  }
  else if (tempp >= 35 && tempp < 40)
  {
    // start red -- eteindre les autres
    digitalWrite(LEDROUGE, HIGH);
    // eteindre les autres
    digitalWrite(LEDVERTE, LOW);
    digitalWrite(LEDBLEU, LOW);
    digitalWrite(LEDJAUNE, LOW);
  }
  else
  {
    // start all led pour les faires clignoter
    digitalWrite(LEDROUGE, HIGH);
    digitalWrite(LEDVERTE, HIGH);
    digitalWrite(LEDBLEU, HIGH);
    digitalWrite(LEDJAUNE, HIGH);

    delay(250);

    digitalWrite(LEDROUGE, LOW);
    digitalWrite(LEDVERTE, LOW);
    digitalWrite(LEDBLEU, LOW);
    digitalWrite(LEDJAUNE, LOW);
    delay(250);
  }
}

void sendTempToMqtt(float temperatureC)
{
  Serial.println("Envoie du message...");
  client.publish(temp, String(temperatureC).c_str(), true); // Publie la température sur le topic temperature_topic
  Serial.println("OK");
  Serial.println("Envoie du prochain message dans 5s...");
  delay(5000);
}

void loop()
{
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print("Temperature :");
  Serial.println(temperatureC);

  if (!client.connected())
  {
    Serial.println("client pas connecte...");
    reconnect();
  }

  startled(temperatureC);
  sendTempToMqtt(temperatureC);

  client.loop();

  client.subscribe(blacktea);
}

void callback(char *topic, byte *payload, unsigned int length)
{

  int i = 0;
  Serial.println("Message recu =>  topic: " + String(topic));
  Serial.print(" | longueur: " + String(length, DEC));

  // create character buffer with ending null terminator (string)
  for (i = 0; i < length; i++)
  {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';

  String msgString = String(message_buff);
  Serial.println("Payload: " + msgString);

  if (msgString == "ON")
  {
    digitalWrite(LEDNOIR, HIGH);
  }
  else if (msgString == "OFF")
  {
    digitalWrite(LEDNOIR, LOW);
  }
}
