#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int LEDJAUNE = 4;
const int LEDROUGE = 5;
const int LEDVERTE = 12;
const int LEDBLEU = 0;
// GPIO where the DS18B20 is connected to
const int oneWireBus = 14;
const int BAUDRATE = 115200;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void startled(float temp)
{
  if (temp >= 20 && temp < 25)
  {
    // start led vert -- eteindre les autres
    digitalWrite(LEDVERTE, HIGH);
    // eteindre les autres
    digitalWrite(LEDBLEU, LOW);
    digitalWrite(LEDJAUNE, LOW);
    digitalWrite(LEDROUGE, LOW);
  }
  else if (temp >= 25 && temp < 30)
  {
    // start bleu -- eteindre les autres
    digitalWrite(LEDBLEU, HIGH);
    // eteindre les autres
    digitalWrite(LEDVERTE, LOW);
    digitalWrite(LEDROUGE, LOW);
    digitalWrite(LEDJAUNE, LOW);
  }
  else if (temp >= 30 && temp < 35)
  {
    // start jaune -- eteindre les autres
    digitalWrite(LEDJAUNE, HIGH);
    // eteindre les autres
    digitalWrite(LEDROUGE, LOW);
    digitalWrite(LEDVERTE, LOW);
    digitalWrite(LEDBLEU, LOW);
  }
  else if (temp >= 35 && temp < 40)
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

void setup()
{
  Serial.begin(BAUDRATE);
  sensors.begin();
  pinMode(LEDJAUNE, OUTPUT);
  pinMode(LEDROUGE, OUTPUT);
  pinMode(LEDVERTE, OUTPUT);
  pinMode(LEDBLEU, OUTPUT);
  // put your setup code here, to run once:
}

void loop()
{
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");

  startled(temperatureC);
  // put your main code here, to run repeatedly:
}
