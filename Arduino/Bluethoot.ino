/*-------------------------------------------------------------------
  Comandos de Bluetooth:

  ESP_SPP_INIT_EVT: Cuando el modo SPP es inicializado
  ESP_SPP_UNINIT_EVT: Cuando el modo SPP es desinicializado
  ESP_SPP_DISCOVERY_COMP_EVT: Cuando se completa el descubrimiento de servicios
  ESP_SPP_OPEN_EVT: Cuando un cliente SPP abre una conexión
  ESP_SPP_CLOSE_EVT: Cuando se cierra una conexión SPP
  ESP_SPP_START_EVT: Cuando se inicializa el servidor SPP
  ESP_SPP_CL_INIT_EVT: Cuando un cliente SPP inicializa una conexión
  ESP_SPP_DATA_IND_EVT: Al recibir datos a través de una conexión SPP
  ESP_SPP_CONG_EVT: Cuando cambia el estado de congestión en una conexión SPP
  ESP_SPP_WRITE_EVT: Al enviar datos a través de SPP.
  ESP_SPP_SRV_OPEN_EVT: Cuando un cliente se conecta al servidor SPP
  ESP_SPP_SRV_STOP_EVT: Cuando el servidor SPP se detiene
  -------------------------------------------------------------------*/

//Incluir librería de bluetooth y DHT
#include "BluetoothSerial.h"
#include <DHT.h>

//Condición para habilitar el bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Definir pines
#define SW1 15 //Pull Down
#define SW2 4 //Pull Up

//Variables para mejorar le funcionamiento del botón
long timeCounter = 0;
int timeThreshold = 250;




// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 32
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11

// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

//Definir un pin para el LED
const int LEDS[5] = {14, 27, 26, 25, 33};
BluetoothSerial BT; //Objeto bluetooth

//Función de llamada haciendo referencia al objeto BT
void callback_function(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  switch (event) {
    case ESP_SPP_START_EVT:
      Serial.println("Inicializado SPP");
      break;
    case ESP_SPP_SRV_OPEN_EVT:
      Serial.println("Cliente conectado");
      break;
    case ESP_SPP_CLOSE_EVT:
      Serial.println("Cliente desconectado");
      break;
    case ESP_SPP_DATA_IND_EVT:
      Serial.println("Datos recibidos");
      while (BT.available()) { // Mientras haya datos por recibir
        int incoming = BT.read(); // Lee un byte de los datos recibidos
        Serial.print("Recibido: ");
        Serial.println(incoming);

        if (incoming == 49 ) { // 1 en ASCII
          digitalWrite(LEDS[0], HIGH);
          BT.println("LED encendido"); // Envío una confirmación de led encendido
        }
        if (incoming == 50) {
          digitalWrite(LEDS[1], HIGH);
          BT.println("LED encendido");
        }
        if (incoming == 51) {
          digitalWrite(LEDS[2], HIGH);
          BT.println("LED encendido");
        }
        if (incoming == 52) {
          digitalWrite(LEDS[3], HIGH);
          BT.println("LED encendido");
        }
        if (incoming == 53) {
          digitalWrite(LEDS[4], HIGH);
          BT.println("LED encendido");
        }
        else if (incoming == 48) { // 0 en ASCII
          for (int i = 0 ; i < 5 ; i++) {
            digitalWrite(LEDS[i], LOW);
          }
          BT.println("LEDS apagado"); // Envío una confirmación de led apagado
        }
      }
      break;
  }
}
void setup() {
  Serial.begin(115200);
  BT.begin("Dereck_bt"); // Nombre de tu Dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejar");
  BT.register_callback(callback_function); //Devolución de la función callback
  for (int i = 0; i < 5; i++) {
    pinMode(LEDS[i], OUTPUT);
  } // Pin LED como salida
  pinMode(SW1, INPUT); //Pin de entrada
  pinMode(SW2, INPUT); //Pin de entrada
  dht.begin();
}

void loop() {
  //Variables para los botones, saber si estan presionados o no
  int in_up = digitalRead(SW2);
  int in_down = digitalRead(SW1);

  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahrenheit
  float f = dht.readTemperature(true);


  // Calcular el índice de calor en Fahrenheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }


  if (in_up == LOW) {
    //Ajuste en el tiempo de respuesta del boton
    if (millis() > timeCounter + timeThreshold) {
      BT.print("Humedad: ");
      BT.print(h);
      BT.println(" %\t");

      timeCounter = millis();
    }
  }
    if (in_down == HIGH) {
     if (millis() > timeCounter + timeThreshold) {
      BT.print("Humedad: ");
      BT.print("Temperatura: ");
      BT.print(t);
      BT.print(" *C ");
      BT.print(f);
      BT.println(" *F\t");
       timeCounter = millis();
    }
      }
    }
