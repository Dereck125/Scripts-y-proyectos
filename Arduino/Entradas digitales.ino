//Defino la variable del boton y la lista de leds que usare
#define PinBotton_PullUp 4
#define PinBotton_PullDown 15

const int LEDS[5] = {14, 27, 26, 25, 33};

//Variables auxiliares para el contador y el el timepo de reaccion del boton
int count = 0;
long timeCounter = 0;
int timeThreshold = 250;

//Las salidas y entradas digitales de los botones y led que se usaran
void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  Serial.begin(115200);
  pinMode(PinBotton_PullUp, INPUT);
  pinMode(PinBotton_PullDown, INPUT);
}

void loop() {
//Variables para los botones, saber si estan presionados o no
  int in_up = digitalRead(PinBotton_PullUp);
  int in_down = digitalRead(PinBotton_PullDown);
//if para cuando uno de los botones se presiona, encnda uno a uno los leds
  if (in_up == LOW) {
    //Ajuste en el tiempo de respuesta del boton
    if (millis() > timeCounter + timeThreshold) {
      //contador de las pulsaciones
      count++;
      //Aqui se encienden los leds de acuerdo al contador
      digitalWrite(LEDS[count - 1], HIGH);
      Serial.println(count);
      timeCounter = millis();
    }
    //Cundo se llega a encender los 5 leds, reinicia el ciclo
    if (count == 6) {
      for (int i = 4; i >= 0; i--) {
        digitalWrite(LEDS[i], LOW);
      }
      count = 0;
    }

  }
//si presiona el otro boton, se reinicia el ciclo, sin importar el led donde se encuentre prendido
  if (in_down == HIGH) {
    Serial.print(in_down);
    for (int i = 4; i >= 0; i--) {
      digitalWrite(LEDS[i], LOW);
    }
    count = 0;
  }

}
