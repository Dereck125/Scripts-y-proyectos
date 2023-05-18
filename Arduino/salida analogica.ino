//Definir pines
#define PinPWM 14
#define PinADC1 12

#define PinDAC 26
#define PinADC2 13

//Variables para definir el PWM
const int freq = 1000; //Frecuencia en Hz
const int ChanelPWM = 0; //Canal 0 - 15
const int resolution = 8; //Bits de resolución, hasta 8


void setup() {
  Serial.begin(115200); //Inicializar comunicación serial
  pinMode(PinADC1, INPUT); //Declarar pin como entrada
  pinMode(PinPWM, OUTPUT); //Declarar pin como salida

  pinMode(PinADC2, INPUT); //Declaro pin como entrada
  pinMode(PinDAC, OUTPUT); //Declaro pin como salida


  
  ledcSetup(ChanelPWM, freq, resolution); //Configuración del PWM
  ledcAttachPin(PinPWM, ChanelPWM); //Enlazar pin a canal
}

void loop() {
 
  for(int i = 0; i < 256; i++){
    ledcWrite(ChanelPWM, i);
    float v1 = analogRead(PinADC1)*(3.3/4096.0);  
    dacWrite(PinDAC, i);
    float v2 = analogRead(PinADC2)*(3.3/4096.0);  

    
    
    Serial.print("PWM:");
    Serial.print(v1);
    Serial.print(",");  
    Serial.print("DAC:");
    Serial.println(v2);  

    delayMicroseconds(1000);
    
    
  }
    
  //Escribo un contínuo incremental de valore y los leo con el ADC
  for(int i = 256; i > -1; i--){
    dacWrite(PinDAC, i);
    float v1 = analogRead(PinADC1)*(3.3/4096.0);
    float v2 = analogRead(PinADC2)*(3.3/4096.0);
    
    Serial.print("PWM:");
    Serial.print(v1);
    Serial.print(",");
    Serial.print("DAC:");
    Serial.println(v2);

    
    delay(10);


     
  
 
  }
  
}
