//Bibliotecas----------------------------------
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

//Credenciales Red-----------------------------
//const char* ssid = "Totalplay-3AAA";
//const char* password = "3AAAFF454wKvNXwP";

const char* ssid = "IncubotCISCO";
const char* password = "MSP430G2";

//LEDs GPIO-------------------------------------
const int LED[4] = {14, 27, 25, 33};
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";
//LEDs GPIO PWM-------------------------------------
const int ledPin = 26;
String sliderValue = "0";

//PWM propiedades------------------------------
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;
const char* PARAM_INPUT = "value";

//SENSOR GPIO----------------------------------
const int sensorPin = 34; //LDR

//potenciometro GPIO----------------------------------
const int pot = 35;



//Objeto AsyncWebServer, puerto 80-------------
AsyncWebServer server(80);

//Conexión WiFi--------------------------------
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  delay(2000);

  //Función para error 404
  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(404, "text/plain", "Error 404");
  });
}

//Leer el sensor-------------------------------
String ReadSensor() {
  float sensorVal = analogRead(sensorPin) * (3.3 / 4096.0);
  Serial.println(sensorVal);
  return String(sensorVal);
}

//Leer POT
String Readpot() {
  float potValue =  analogRead(pot);
  Serial.println(potValue);
  return String(potValue);
}


//Acciones a realizar    LEDS------------------------
String processor(const String& var) {
  Serial.println(var);
  if (var == "POTENCIOMETRO"){
    return Readpot();
  }
  if (var == "SLIDERVALUE") {
    return sliderValue;
  }
  if (var == "SENSOR") {
    return ReadSensor();
  }
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    buttons += "<h4>LED 1</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"14\" " + outputState(LED[0]) + "><span class=\"slider1\"></span></label>";
    buttons += "<h4>LED 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"27\" " + outputState(LED[1]) + "><span class=\"slider2\"></span></label>";
    buttons += "<h4>LED 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"25\" " + outputState(LED[2]) + "><span class=\"slider4\"></span></label>";
    buttons += "<h4>LED 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(LED[3]) + "><span class=\"slider5\"></span></label>";

    return buttons;
  }
  return String();
}

//Estado del led-------------------------------
String outputState(int output) {
  if (digitalRead(output)) {
    return "checked";
  }
  else {
    return "";
  }
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  //Leds---------------------------------------

  for (int i = 0; i < 4; i++) {
    pinMode(LED[i], OUTPUT);
  }

  for (int i = 0; i < 4; i++) {
    digitalWrite(LED[i], LOW);
  }

  pinMode(sensorPin, INPUT);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);

  //Activar PWM--------------------------------
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, sliderValue.toInt());

  //Comenzar el servidor-----------------------
  server.begin();

  //Confirmar archivos subidos
  //a la memoria flash-------------------------
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al leer SPIFFS");
    return;
  }

  //Ruta raíz del archivo HTML-----------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });



  //Archivo CSS--------------------------------
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  //Archivo JS--------------------------------
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/script.js", "text/js");
  });

  //Enviar dato sensor-------------------------
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", ReadSensor().c_str());
  });

  //Enviar dato POT-------------------------
  server.on("/pot", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", Readpot().c_str());
  });

  //PARA LOS LEDS SIN PWM
  //Enviar solicitud a <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage1;
    String inputMessage2;

    //Obtener valor de <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();

      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "Mensaje no enviado";
      inputMessage2 = "Mensaje no enviado";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - estado: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  //LED CON PWM

  //Enviar solicitud a <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    //Obtener valor de <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      ledcWrite(ledChannel, sliderValue.toInt());
    }
    else {
      inputMessage = "Mensaje no enviado";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });


}

void loop() {


}
