#define PinADC 34 //Defino pin
const int LEDS[5] = {14, 27, 26, 25, 33}; //Defino los pins de los leds

void setup() {
  //Los leds seran señales de salida
for (int i = 0; i < 5; i++) {
  pinMode(LEDS[i], OUTPUT);
  }
  //El sensor de luz como se señal de entrada
  pinMode(PinADC, INPUT);
  Serial.begin(115200);     //Inicializo comunicación serial

}

void loop() {
  
 int Luz = analogRead(PinADC);  //Guardo en una variable la lectura
 int prom = media(10, PinADC);     //Guardo en una variable el resultado de la función media
 Serial.print("Prom:");
  Serial.print(prom);               //Imprimo el valor de la variable "prom"
  Serial.print(",");
  Serial.print("Luz:");
  Serial.println(Luz);           //Imprimo el valor de la variable "sensor"
//Aqui la configuracion del encendido de los led en base a la cantidad de luz recibida por el sensor
  if(prom > 800){
    digitalWrite(LEDS[0],HIGH);
  }
  else{digitalWrite(LEDS[0],LOW);}
  
   if(prom > 1600){
    digitalWrite(LEDS[1],HIGH);
  }
  else{digitalWrite(LEDS[1],LOW);}
  
   if(prom > 2400){
    digitalWrite(LEDS[2],HIGH);
  }
  else{digitalWrite(LEDS[2],LOW);}
  
   if(prom > 3200){
    digitalWrite(LEDS[3],HIGH);
  }
  else{digitalWrite(LEDS[3],LOW);}
  
  if(prom > 4000){
    digitalWrite(LEDS[4],HIGH);
  }
  else{digitalWrite(LEDS[4],LOW);}


  
 }


//Función media(número de datos, pin de entrada)
int media(int data_number, int pin) {
  int value = 0;
  for (int i = 0; i < data_number; i++) {
    value = analogRead(pin) + value;
  }
  value /= data_number;
  return value;
}
