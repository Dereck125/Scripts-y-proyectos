/*
   Write personal data of a MIFARE RFID card using a RFID-RC522 reader
   Uses MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT.
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

   More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout

   Hardware required:
   Arduino
   PCD (Proximity Coupling Device): NXP MFRC522 Contactless Reader IC
   PICC (Proximity Integrated Circuit Card): A card or tag using the ISO 14443A interface, eg Mifare or NTAG203.
   The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com.
*/
#include "secrets.h"          //Variables de conexión, llaves y certificados
#include <WiFiClientSecure.h> //Conexión al WiFi, SSL
#include <PubSubClient.h>     //MQTT
#include <ArduinoJson.h>      //Tipo de dato Json
#include "WiFi.h"             //Conexión al WiFi
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22       // Configurable, see typical pin layout above
#define SS_PIN          5// Configurable, see typical pin layout above


#define AWS_IOT_PUBLISH_TOPIC1  "ESP32_al4zka/NOMBRE"    //Tópico de publicación
#define AWS_IOT_PUBLISH_TOPIC2  "ESP32_al4zka/TELEFONO"    //Tópico de publicación
//#define AWS_IOT_SUBSCRIBE_TOPIC "ESP32_al4zka/MENSAJES"       //Tópico de suscripción


unsigned long lastMsg = 0;
unsigned long lastMsg1 = 0;



#define BUILTIN_LED 14


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
  client.setCallback(callback);

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



//Función de suscripción vía MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void setup() {
  pinMode( BUILTIN_LED, OUTPUT);
  Serial.begin(115200);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  connectAWS();             //Conexión al WiFi, AWS y MQTT
  Serial.println("Programa de asistencia mediante tarjetas RFID");
  Serial.println("Acerque la tarjeta o tag");
}



void menu() {
  bool exitMenu = false;
  bool menuShown = false;  // Variable para controlar si el menú ya se ha mostrado


  while (!exitMenu) {
    if (!menuShown) {
      Serial.println("Menú interactivo:");
      Serial.println("1. Escribir nombre y número de teléfono en RFID");
      Serial.println("2. Leer los datos de la tarjeta RFID");
      Serial.println("3. Validar acceso de la tajeta ");
      Serial.println("Ingrese el número de la opción deseada:");
      menuShown = true;  // Marcar que el menú se ha mostrado
    }
    while (!Serial.available()) {
      // Esperar hasta que haya datos disponibles en el monitor serial
    }

    int option = Serial.parseInt();  // Leer el valor ingresado por el usuario
    Serial.println("OPCION DADA");
    Serial.println(option);

    bool validOption = true;  // Variable para verificar si la opción es válida

    Serial.flush();

    if (option == 1) {

      // Lógica para escribir nombre y número de teléfono en RFID
      // ...




      Serial.println("Opción 1 seleccionada");
      Serial.println("Debes acercar la tarjeta");
      // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
      MFRC522::MIFARE_Key key;
      for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;


      // Select one of the card
      mfrc522.PICC_ReadCardSerial();


      Serial.print(F("Card UID:"));    //Dump UID
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.print(F(" PICC type: "));   // Dump PICC type
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));

      byte buffer[34];
      byte block;
      MFRC522::StatusCode status;
      byte len;

      Serial.setTimeout(20000L) ;     // wait until 20 seconds for input from serial
      // Ask personal data: Family name
      Serial.println(F("Coloca tu nombre y un apellido con un # al final, porfaavor"));
      len = Serial.readBytesUntil('#', (char *) buffer, 30) ; // read family name from serial
      for (byte i = len; i < 30; i++) buffer[i] = ' ';     // pad with spaces

      block = 1;
      //Serial.println(F("Authenticating using key A..."));
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }
      else Serial.println(F("PCD_Authenticate() success: "));

      // Write block
      status = mfrc522.MIFARE_Write(block, buffer, 16);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }
      else Serial.println(F("MIFARE_Write() success: "));

      block = 2;
      //Serial.println(F("Authenticating using key A..."));
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }

      // Write block
      status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }
      else Serial.println(F("MIFARE_Write() success: "));

      // Ask personal data: First name
      Serial.println(F("Coloca tu numero de telefono con un  # al final"));
      len = Serial.readBytesUntil('#', (char *) buffer, 20) ; // read first name from serial
      for (byte i = len; i < 20; i++) buffer[i] = ' ';     // pad with spaces

      block = 4;
      //Serial.println(F("Authenticating using key A..."));
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }

      // Write block
      status = mfrc522.MIFARE_Write(block, buffer, 16);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }
      else Serial.println(F("MIFARE_Write() success: "));

      block = 5;
      //Serial.println(F("Authenticating using key A..."));
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }

      // Write block
      status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }
      else Serial.println(F("MIFARE_Write() success: "));
      delay(1000);

      Serial.println(" ");
      mfrc522.PICC_HaltA(); // Halt PICC
      mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD


      Serial.println("Regresando al menu...");
      menuShown = false;


    } else if (option == 2) {
      Serial.println("Opcion 2 seleccionada");

      // Lógica para leer los datos de la tarjeta RFID
      // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
      MFRC522::MIFARE_Key key;
      for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;


      // Select one of the card
      mfrc522.PICC_ReadCardSerial();

      //some variables we need
      byte block;
      byte len;
      MFRC522::StatusCode status;

      //-------------------------------------------

      Serial.println(F("**Tarjeta detectada:**"));

      //-------------------------------------------

      mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

      //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

      //-------------------------------------------

      Serial.print(F("Numero de telefono: "));

      byte buffer1[18];
      //String Numero = "";
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



      String Numero = "";




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

      Serial.println("NUMERO YA DECODIFICADO ");

      Serial.println(" ");

      Serial.println( Numero_nuevo );
      unsigned long now = millis();
      if (now - lastMsg > 2000) {
        lastMsg = now;
        StaticJsonDocument<64> doc1;
        //doc["NOMBRE"] = Nombre;
        doc1["NUMERO"] = Numero_nuevo;
        String output;
        serializeJsonPretty(doc1, output);
        Serial.print("Publish message: ");
        Serial.println(output);
        client.publish(AWS_IOT_PUBLISH_TOPIC2 , output.c_str());
      }


      //Serial.println(" ");

      //---------------------------------------- GET name

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

      Serial.println("NOOMBRE YA DECODIFICADO ");

      Serial.println(" ");

      Serial.println(Nombre);

      unsigned long now1 = millis();
      if (now1 - lastMsg1 > 2000) {
        lastMsg1 = now1;
        StaticJsonDocument<64> doc2;
        doc2["NOMBRE"] = Nombre;
        String output;
        serializeJsonPretty(doc2, output);
        Serial.print("Publish message: ");
        Serial.println(output);
        client.publish(AWS_IOT_PUBLISH_TOPIC1 , output.c_str());
      }



      Serial.println(" ");




      //----------------------------------------

      Serial.println(F("\n**Lectura finalizada**\n"));

      delay(1000); //change value if you want to read cards faster

      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();

      Serial.println("Regresando al menu...");
      menuShown = false;


    } else if (option == 3) {

      //if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
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
        Serial.println(" ");
        Serial.println("xxxxxxxxxxxxxxxxxxxxxx");


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
      //}
      menuShown = false;
      delay(1000);

    } else {
      validOption = false;  // La opción ingresada no es válida
    }

    if (!validOption) {
      Serial.println(" ");
    }
  }

}




void loop() {

  if (mfrc522.PICC_IsNewCardPresent()) {

    menu();  // Ejecutar el menú interactivo
  }

}
