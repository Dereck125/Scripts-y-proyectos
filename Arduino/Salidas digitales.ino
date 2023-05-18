//Se crea una lista con los leds que encenderan, un arreglo en el cual tiene los pines de los led
const int LEDS[5] = {14,27,26,25,33};

//se manda la señal para dar la instruccion de que los leds se usaran
void setup() {
//ciclo for que itera sobre la lista de los leds, para mandar señal de salida a cada uno
  for(int i= 0; i<5;i++){
    
  pinMode(LEDS[i], OUTPUT);
  
  }
}
//Accion que se repetira en los leds
void loop() {
//Primer ciclo for para encender y apagar los leds de un lado
  for(int i=0;i<5;i++){
  digitalWrite(LEDS[i], HIGH);
  //Delay para el encendido y apagado
  delay(100);      
  digitalWrite(LEDS[i], LOW);  
    }
//Segundo ciclo for para el regreso del encendido y apagado de cada led
    for(int i=4;i>=0;i--){
  digitalWrite(LEDS[i], HIGH);
  delay(100);      
  digitalWrite(LEDS[i], LOW);  
    }
                    
}
