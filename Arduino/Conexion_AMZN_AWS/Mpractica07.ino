/*
  Universidad Nacional Autónoma de México
  Facultad de Ciencias
  Licenciatura en Ciencias de la Computación
  Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas

  Escrito por: M. en I. Valente Vázquez Velázquez
  Última modificación: 23-marzo-2023

  https://sites.google.com/ciencias.unam.mx/introduccion-iot-fc/inicio
*/


//Bibliotecas
#include "secrets.h"          //Variables de conexión, llaves y certificados
#include <WiFiClientSecure.h> //Conexión al WiFi, SSL
#include <PubSubClient.h>     //MQTT
#include <ArduinoJson.h>      //Tipo de dato Json
#include "WiFi.h"             //Conexión al WiFi
#include "DHT.h"              //Sensor de temperatura y humedad


//Definir pines
#define SW1 15 //Pull Down
#define SW2 4 //Pull Up

//Variables para mejorar le funcionamiento del botón
long timeCounter = 0;
int timeThreshold = 250;


#define DHTPIN 32             //Pin digital de DHT
#define DHTTYPE DHT11         //Tipo de sensor

const int PinLed[5] = {14, 27, 26, 25, 33};
#define PinADC 34 //Defino pin de fotoresistencia


#define AWS_IOT_PUBLISH_TOPIC_DHT_T  "ESP32_al4zka/Miguel/DHT/TEMPERATURA"    //Tópico de publicación
#define AWS_IOT_PUBLISH_TOPIC_DHT_H  "ESP32_al4zka/Miguel/DHT/HUMEDAD"    //Tópico de publicación
#define AWS_IOT_PUBLISH_TOPIC_POT  "ESP32_al4zka/Miguel/Pot"    //Tópico de publicación
#define AWS_IOT_SUBSCRIBE_TOPIC "ESP32_al4zka/Miguel/Leds"       //Tópico de suscripción

//Variables de humedad, temperatura y tiempo de lectura del sensor
float h;
float t;
float v;
long ti = millis();

//Objetos del sensor, WiFi SSL y MQTT
DHT dht(DHTPIN, DHTTYPE);
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

//Función de conexión al WiFi, AWS y MQTT
void connectAWS()
{
  WiFi.mode(WIFI_STA);                      //Tipo de conexión WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);     //Credenciales WiFi
  Serial.println("Conectando al Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED)     //Imprime puntos mientras se conecta
  {
    delay(500);
    Serial.print(".");
  }

  //Credenciales AWS
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  //Conexión vía MQTT
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  //Manejador del cliente
  client.setCallback(messageHandler);

  Serial.println("\nConectando a AWS IOT...");
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected())
  {
    Serial.println("¡No ha sido posible conectarse a AWS IoT!");
    return;
  }

  //Tópico de suscripción
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("¡Conectado establecida con AWS IoT!");
}

//Función de publicar mensaje vía MQTT
void publishMessage_Humedad()
{
  StaticJsonDocument<200> doc;    //Declarar variable "doc" tipo Json
  doc["humedad"] = h;            //humedad tipo Json
  char jsonBuffer[512];           //Arreglo de caracteres de 512 bytes
  serializeJson(doc, jsonBuffer); //Empaquetar variable
  client.publish(AWS_IOT_PUBLISH_TOPIC_DHT_H, jsonBuffer); //Publicar dato

  Serial.print("Humedad_AWS: ");
  Serial.print(h);
}
void publishMessage_Temperatura()
{
  StaticJsonDocument<200> doc;    //Declarar variable "doc" tipo Json
  doc["temperatura"] = t;         //temperatura tipo Json
  char jsonBuffer[512];           //Arreglo de caracteres de 512 bytes
  serializeJson(doc, jsonBuffer); //Empaquetar variable
  client.publish(AWS_IOT_PUBLISH_TOPIC_DHT_T, jsonBuffer); //Publicar dato

  Serial.print("%  Temperatura_AWS: ");
  Serial.print(t);
  Serial.println("°C ");
}
void publishMessage_Fotoresistencia()
{
  StaticJsonDocument<200> doc;    //Declarar variable "doc" tipo Json
  doc["Voltaje"] = v;         //Voltaje tipo Json
  char jsonBuffer[512];           //Arreglo de caracteres de 512 bytes
  serializeJson(doc, jsonBuffer); //Empaquetar variable
  client.publish(AWS_IOT_PUBLISH_TOPIC_POT, jsonBuffer); //Publicar dato
  Serial.print("Resistencia_AWS: ");
  Serial.print(v);
  Serial.println("V");
}

//Función de suscripción vía MQTT
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  StaticJsonDocument<200> doc;          //Declarar variable "doc" en tipo Json
  deserializeJson(doc, payload);        //Desepaquetar el mensaje
  const char* message = doc["message"]; //Guardar la carga en la variable mensaje
  Serial.println(message);

  //Los led se encienden de acuerdo al numero, y se apagan todos al mandar 0
  if (*message == '1') {
    digitalWrite(PinLed[0], HIGH);
    Serial.println("Led 1 encendido");
  }
  if (*message == '2') {
    digitalWrite(PinLed[1], HIGH);
    Serial.println("Led 2 encendido");
  }
  if (*message == '3') {
    digitalWrite(PinLed[2], HIGH);
    Serial.println("Led 3 encendido");
  }

  if (*message == '4') {
    digitalWrite(PinLed[3], HIGH);
    Serial.println("Led 4 encendido");
  }
  if (*message == '5') {
    digitalWrite(PinLed[4], HIGH);
    Serial.println("Led 5 encendido");
  }

  else if (*message == '0') {
    for (int i = 0 ; i < 5 ; i++) {
      digitalWrite(PinLed[i], LOW);
    }
    Serial.println("Led apagado");
  }
  else {
    Serial.println("Dato no válido");
  }
}

void setup()
{
  Serial.begin(115200);     //Conexión serial
  connectAWS();             //Conexión al WiFi, AWS y MQTT
  dht.begin();              //Inicializar sensor DHT
  for (int i = 0; i < 5; i++) {
    pinMode(PinLed[i], OUTPUT);
  }//pin led de salida
  pinMode(SW1, INPUT); //Pin de entrada
  pinMode(SW2, INPUT); //Pin de entrada
  //El sensor de luz como se señal de entrada
  pinMode(PinADC, INPUT);
}

void loop()
{
  int in_up = digitalRead(SW2);
  int in_down = digitalRead(SW1);

  h = dht.readHumidity();
  t = dht.readTemperature();
  v = analogRead(PinADC);  //Guardo en una variable la lectura

  if (isnan(h) || isnan(t) )  // Revisa el estado del sensor DHT
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  client.loop();
  if (in_up == LOW) {
    //Ajuste en el tiempo de respuesta del boton
    if (millis() > timeCounter + timeThreshold) {
      publishMessage_Humedad(); //Publico el mensaje
      timeCounter = millis();
    }
  }

  if (in_down == HIGH) {
    if (millis() > timeCounter + timeThreshold) {
      publishMessage_Temperatura(); //Publico el mensaje
      timeCounter = millis();
    }
  }
  //Se publica el voltaje de la fotoresistencia cada 1 segundo
if (millis() - ti >= 2000)
  {
    
      publishMessage_Fotoresistencia(); //Publico el mensaje de resistencia 
    Serial.print("Fotoresistencia_ESP: ");
    Serial.println(v);           //Imprimo el valor de la variable "sensor"
    Serial.print("Humedad_ESP: ");
    Serial.println(h);
    Serial.print("Temperatura_ESP: ");
    Serial.println(t);
       ti = millis();
    }

}
