#include <Arduino.h>

const int LEDJAUNE = 5;
const int LEDROUGE = 4;
const int LEDVERTE = 0;
const int LEDBLEU = 12;

void setup()
{
  pinMode(LEDJAUNE, OUTPUT);
  pinMode(LEDROUGE, OUTPUT);
  pinMode(LEDVERTE, OUTPUT);
  pinMode(LEDBLEU, OUTPUT);
  // put your setup code here, to run once:
}

void loop()
{
  digitalWrite(LEDJAUNE, HIGH);
  digitalWrite(LEDROUGE, HIGH);
  digitalWrite(LEDVERTE, HIGH);
  digitalWrite(LEDBLEU, HIGH);

  delay(250);

  digitalWrite(LEDJAUNE, LOW);
  digitalWrite(LEDROUGE, LOW);
  digitalWrite(LEDVERTE, LOW);
  digitalWrite(LEDBLEU, LOW);

  delay(250);
  // put your main code here, to run repeatedly:
}