#include "secrets.h"          //Variables de conexión, llaves y certificados
#include <WiFiClientSecure.h> //Conexión al WiFi, SSL
#include <PubSubClient.h>     //MQTT
#include <ArduinoJson.h>      //Tipo de dato Json
#include "WiFi.h"             //Conexión al WiFi
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22       // Configurable, see typical pin layout above
#define SS_PIN          5// Configurable, see typical pin layout above


#define AWS_IOT_PUBLISH_TOPIC1  "ESP32_al4zka/ASISTENCIA"    //Tópico de publicación

unsigned long lastMsg = 0;
unsigned long lastMsg1 = 0;

// Arreglo con los UID de las tarjetas permitidas en formato hexadecimal
byte allowedUIDs[][4] = {
  {0xCA, 0x51, 0x9B, 0xCB},  // UID tarjeta 1 en hexadecimal
  {0xA3, 0xDE, 0x65, 0x4D},  // UID tarjeta 2 en hexadecimal
  {0x03, 0xED, 0x2E, 0x1D},  // UID tarjeta 3 en hexadecimal
  {0x71, 0x3C, 0x93, 0x1C},  // UID tarjeta 4 en hexadecimal
  {0x23, 0x93, 0xE3, 0x1D},  // UID tarjeta 5 en hexadecimal
  {0x43, 0x59, 0x5B, 0x94},  // UID tarjeta 6 en hexadecimal
  {0x08, 0x2A, 0x20, 0x34}   // UID tarjeta 7 en hexadecimal
};

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance


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
  //client.setCallback(callback);

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
  //client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("¡Conectado establecida con AWS IoT!");
}



void setup() {
  Serial.begin(115200);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  connectAWS();             //Conexión al WiFi, AWS y MQTT
  Serial.println("Programa de asistencia mediante tarjetas RFID");
  Serial.println("Acerque la tarjeta o tag");

}

void loop() {

  if (mfrc522.PICC_IsNewCardPresent()) {

    Serial.println(F("**Tarjeta detectada:**"));

    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    mfrc522.PICC_ReadCardSerial();

    byte block;
    byte len;
    MFRC522::StatusCode status;

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

    byte currentUID[4];
    memcpy(currentUID, mfrc522.uid.uidByte, 4);  // Copiar UID actual

    // Verificar si el UID se encuentra en el arreglo de tarjetas permitidas
    bool accessGranted = false;
    for (int i = 0; i < sizeof(allowedUIDs) / sizeof(allowedUIDs[0]); i++) {
      if (memcmp(currentUID, allowedUIDs[i], 4) == 0) {
        accessGranted = true;
        break;
      }
    }

    // Imprimir el resultado en el monitor serial
    if (accessGranted) {


      Serial.println("xxxxxxxxxxxxxxxxxxxxxx");
      Serial.println(" ");
      Serial.println("Acceso válido");

      Serial.print(F("Numero de telefono: "));

      byte buffer1[18];
      String Numero = "";
      block = 4;
      len = 18;

      //------------------------------------------- GET number
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }

      status = mfrc522.MIFARE_Read(block, buffer1, &len);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }

      //PRINT numero de telefono
      for (uint8_t i = 0; i < 16; i++)
      {
        if (buffer1[i] != 32)
        {

          Serial.write(buffer1[i]);
          //Serial.write("NUMEROOOOOO");
          //Serial.println(buffer1[i]);
        }
      }

      for (int i = 0; i < sizeof(buffer1); i++) {  // Recorre el arreglo de bytes.
        Numero += (char)buffer1[i];  // Convierte cada byte a char y concaténalo al String.
      }

      String Numero_nuevo = Numero;

      String seleccion1 = "";
      for (int i = 0; i < Numero.length(); i++) {
        char caracter = Numero_nuevo.charAt(i);
        if (isAlphaNumeric(caracter)) {
          seleccion1 += caracter;
        }
      }

      Serial.println(seleccion1);

      Serial.println(F("Nombre y apellido : "));


      byte buffer2[18];
      block = 1;
      String Nombre = "";

      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }

      status = mfrc522.MIFARE_Read(block, buffer2, &len);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }

      //PRINT LAST NAME
      for (uint8_t i = 0; i < 16; i++) {
        Serial.write(buffer2[i] );

      }
      for (int i = 0; i < sizeof(buffer2); i++) {  // Recorre el arreglo de bytes.
        Nombre += (char)buffer2[i];  // Convierte cada byte a char y concaténalo al String.
      }



      String seleccion2 = "";
      for (int i = 0; i < Nombre.length(); i++) {
        char caracter = Nombre.charAt(i);
        if (isAlphaNumeric(caracter)) {
          seleccion2 += caracter;
        }
      }

      Serial.println(seleccion2);

      unsigned long now1 = millis();
      if (now1 - lastMsg1 > 2000) {
        lastMsg1 = now1;
        StaticJsonDocument<64> doc2;
        doc2["NUMERO"] = seleccion1;
        doc2["NOMBRE"] = seleccion2;
        String output;
        serializeJsonPretty(doc2, output);
        Serial.print("Publish message: ");
        Serial.println(output);
        client.publish(AWS_IOT_PUBLISH_TOPIC1 , output.c_str());
      }


      Serial.println(" ");
      Serial.println("xxxxxxxxxxxxxxxxxxxxxx");
      Serial.println(" ");
      Serial.println(F("\n**Lectura finalizada**\n"));
       Serial.println(F("\n**Espera 5 segundos antes de escanear la siguiente..**\n"));
      for (int i = 0; i < 5; i++) {
        Serial.print("."); // Imprime un punto
        delay(1000);        // Espera 100ms (un total de 1000ms para 10 puntos)
      }
      Serial.println(" ");
      Serial.println("Listo,puedes escanear la siguiente tarjeta");

    } else {
      Serial.println("xxxxxxxxxxxxxxxxxxxxxx");
      Serial.println(" ");
      Serial.println("Acceso inválido");
      Serial.println(" ");
      Serial.println("xxxxxxxxxxxxxxxxxxxxxx");


    }

    // Detener la tarjeta RFID
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();


    delay(1000);

  }
}
