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
  float t = 0; //Variable para el tiempo
  float dt = 0.0001; //Delta de tiempo

  while (t <= 2 * PI) { //Realiza la lectura por un ciclo completo
    //Genera la señal sinusoidal de 50 Hz
    float sin_value = sin(2 * PI * 50 * t);

    //Convierte la señal sinusoidal a valores del DAC
    int dac_value = (sin_value + 1) * 120;
    dacWrite(PinDAC, dac_value); //Asigna el valor del DAC al LED

    //Convierte la señal sinusoidal a valores PWM
    int pwm_value = (sin_value + 1) * 120;
    ledcWrite(ChanelPWM, pwm_value); //Asigna el valor del PWM al LED


    //Incrementa el tiempo
    t += dt;

    
    //Lee el valor del ADC1 y lo muestra en el monitor serial
    int adc_value1 = analogRead(PinADC1);
    Serial.print("ADC1: ");
    Serial.print(adc_value1);
    Serial.print(", ");
    //Lee el valor del ADC2 y lo muestra en el monitor serial
    int adc_value2 = analogRead(PinADC2);
    Serial.print("ADC2: ");
    Serial.println(adc_value2);
    //Espera un tiempo para el siguiente ciclo
    delay(10);
  }
}
