#include "CTBot.h"

const int LED[5] = {14, 27, 26, 25, 33};
#define pinPhoto 34
int aux = 1;
CTBot myBot;

//String ssid = "pcpuma";
//String pass = "mango2022";

String ssid = "IncubotCISCO";
String pass = "MSP430G2";

//String ssid = "Totalplay-3AAA";
//String pass = "3AAAFF454wKvNXwP";

String token = "6163119404:AAHMmb9a6fIznfCEL8GxYgJhdYiQbr4OCMk";

boolean state = false;
float umbral = 0.0;
int id = 0;


// Definición de la función isFloat
bool isFloat(String str) {
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (!isDigit(c) && c != '.') {
      return false;
    }
  }
  return true;
}


void setup() {
  Serial.begin(115200);
  Serial.print("Comenzando TelegramBot...  ");
  // pinMode(pinPhoto, INPUT);
  for (int i = 0; i < 5; i++) {
    pinMode(LED[i], OUTPUT);
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED[i], LOW);
  }

  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  delay(2000);

  if (myBot.testConnection())
    Serial.println("Conectado con Bot");
  else
    Serial.println("Error en la conexión");
}

void loop() {
  TBMessage msg;



  float sensor = analogRead(pinPhoto) * (3.3 / 4096.0);

  if (myBot.getNewMessage(msg)) {

    //Serial.println("Chat ID: " + msg.sender.id);
    //Serial.println("Text: " + msg.text);

    if (aux == 1) {
      myBot.sendMessage(msg.sender.id, "Bienvenido a Al4zka bot /start para comenzar");
      aux = 0;
    }


    if (msg.text.equalsIgnoreCase("/start")) {
      Serial.print("Nueva interacción de: ");
      Serial.println(msg.sender.username);
      myBot.sendMessage(msg.sender.id, "Este es el menu:\n/start para comenzar\n/leds para encender o apagar\n/datos para monitorear\n/alarma Crea una alarma para el LDR");
    }
    else if (msg.text.equalsIgnoreCase("/leds")) {
      Serial.print("Se accedio a leds ");
      myBot.sendMessage(msg.sender.id, "¿Que led deseas encender?\n/1\n/2\n/3\n/4\n/5\n/0 Apagar Leds\nRegresar al incio: /start");
    }
    else if (msg.text.equalsIgnoreCase("/1")) {
      digitalWrite(LED[0], HIGH);
      Serial.println("Led 1 encendido");
      myBot.sendMessage(msg.sender.id, "Led 1 encendido exitosamente");
    }
    else if (msg.text.equalsIgnoreCase("/2")) {
      digitalWrite(LED[1], HIGH);
      Serial.println("Led 2 encendido");
      myBot.sendMessage(msg.sender.id, "Led 2 encendido exitosamente");
    }
    else if (msg.text.equalsIgnoreCase("/3")) {
      digitalWrite(LED[2], HIGH);
      Serial.println("Led 3 encendido");
      myBot.sendMessage(msg.sender.id, "Led 3 encendido exitosamente");
    }
    else if (msg.text.equalsIgnoreCase("/4")) {
      digitalWrite(LED[3], HIGH);
      Serial.println("Led 4 encendido");
      myBot.sendMessage(msg.sender.id, "Led 4 encendido exitosamente");
    }
    else if (msg.text.equalsIgnoreCase("/5")) {
      digitalWrite(LED[4], HIGH);
      Serial.println("Led 5 encendido");
      myBot.sendMessage(msg.sender.id, "Led 5 encendido exitosamente");
    }
    else if (msg.text.equalsIgnoreCase("/0")) {
      for (int i = 0; i < 5; i++) {
        digitalWrite(LED[i], LOW);
      }
      Serial.println("Leds apagados");
      myBot.sendMessage(msg.sender.id, "Leds apagados");
    }
    else if (msg.text.equalsIgnoreCase("/datos")) {
      Serial.print("Monitoreo: ");
      float sensorVal = analogRead(pinPhoto) * (3.3 / 4096.0);
      Serial.println(sensorVal);
      String mensaje1 = (String)"El valor actual es: " + sensorVal + "V";
      myBot.sendMessage(msg.sender.id, mensaje1);
      Serial.println("Dato enviado");
    }



    else if (msg.text.equalsIgnoreCase("/alarma")) {

      id = msg.sender.id;
      String mensaje = "Alarma activada";
      myBot.sendMessage(msg.sender.id, mensaje);
      myBot.sendMessage(msg.sender.id, "¿Cual es el voltaje en que deseas acrivarla?");
      myBot.sendMessage(msg.sender.id, "Solo numeros! en formato con punto decimal");

    }
    else if (isFloat(msg.text)) {
      umbral = msg.text.toFloat();
      Serial.println("El umbral sera de: ");
      Serial.println(umbral);
      String textoaux = "La alarma sonara cuando el voltaje sea " + (String)umbral;
      myBot.sendMessage(msg.sender.id,textoaux );

    }
    else if (msg.text.equalsIgnoreCase("/desactivar")) {
      state = false;
      id = msg.sender.id;
      String mensaje = "Alarma desactivada";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println(mensaje);
      id = 0;
    }

    else {
      myBot.sendMessage(msg.sender.id, "Mensaje no válido, intenta de nuevo con: /start");
    }
  }

  if (umbral > 0) {
    state = true;
  }
  if ((state == true) && (sensor > umbral)) {
    Serial.println(sensor);
    Serial.println(id);
    String mensaje = "¡¡ALERTA!! El nivel aumentó a: " + (String)sensor;
    myBot.sendMessage(id, mensaje);
  }
  delay(10);
}
