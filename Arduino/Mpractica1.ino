
void setup() {
  //salida digitial para activar el pin .
  pinMode(14, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(33, OUTPUT);
}

// funcion loop que fuciona como un ciclo for, para repetir el prendido y apagado del led
void loop() {
  //Encienden los focos led.
  digitalWrite(14, HIGH); 
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(33, HIGH);
  delay(15);                       // Espera 15 milisegudnos
  //Apaga el led despues de los 15 milisegundos.
  digitalWrite(14, LOW); 
  digitalWrite(25, LOW); 
  digitalWrite(26, LOW); 
  digitalWrite(27, LOW); 
  digitalWrite(33, LOW); 
  delay(15);                       // espera nuevamente 15 milisegundos para reiniciar el ciclo
}
