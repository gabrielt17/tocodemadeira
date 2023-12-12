/*
Autor: Gabriel Víctor e Cleide Almeida
Data de início: 18/10/2023
Data de término: xx/xx/xxxx
* Programa com funções que utiliza o sensor ultrassônico para determinar
* a posição em até duas dimensões em um espaço de área retangular.
* Raio de medição: 2 cm ~ 400 cm
* Precisão: 3 mm
*/

#define trig 25 // Associa o pino 25 ao pino Trigger
#define echo 26 // Associa o pino 16 ao pino Echo


const byte pwma = 27;
const byte pwmb = 14;
const byte ina1 = 32;
const byte inb1 = 33;
const byte ina2 = 12;
const byte inb2 = 13;

const unsigned int freq = 1000;
const byte canala = 0;
const byte canalb = 1;
const byte res = 8;


void setup() {
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
  pinMode(ina1, OUTPUT);
  pinMode(ina2, OUTPUT);
  pinMode(inb1, OUTPUT);
  pinMode(inb2, OUTPUT);
  ledcSetup(canala, freq, res);
  ledcSetup(canalb, freq, res);
  ledcAttachPin(pwma, canala);
  ledcAttachPin(pwmb,canalb);
  Serial.begin(115200);
}


void loop() {
  Serial.print("Distância atual: ");
  Serial.print(distanciaCm());
  Serial.println(" cm");
  Andar(distanciaCm());
}

float distanciaCm(void) {
  digitalWrite(trig, LOW);
  delay(60); // Recomendação de 60 ms entre ciclos
  digitalWrite(trig, HIGH);
  // Intervalo de tempo mínimo para que a onda seja enviada
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);

  // Mensura os intervalos de tempo entre o último momento que o
  float tempoalto = pulseIn(echo, HIGH);
  float velsom = 0.0003403; // Velocidade do som em microssegundos
  float distancia = tempoalto*velsom*50; // Tempo em nível alto * velocidade do ar/2
  if (distancia >= 400){
    return -1;
  }
  else
    return distancia;
}

void Andar(float distancia) {

  Serial.print("Vou fazer minha escolha baseado no valor: ");
  Serial.print(distancia);
  Serial.println(" cm");
  digitalWrite(ina1, HIGH);
  digitalWrite(inb1, HIGH);
  digitalWrite(ina2, LOW);
  digitalWrite(inb2, LOW);


	if ((distancia >=10)||(distancia == -1)){
    Serial.println("Andar para FRENTE");
    ledcWrite(canala, 130);
    ledcWrite(canalb, 135);
  }
		
	else if ((distancia > 3) && (distancia < 10)) {
    Serial.println("Virar para algum LADO");
    long side_turn;
    side_turn = random(0, 2);

    if(side_turn == 0){
        digitalWrite(ina1, HIGH);
        digitalWrite(ina2, LOW);
        digitalWrite(inb1, LOW);
        digitalWrite(inb2, HIGH);
        ledcWrite(canala, 150);
        ledcWrite(canalb, 150);
        Espera(600);
        ledcWrite(canala, 0);
        ledcWrite(canalb, 0);
        Espera(200);
    }
    else{
        digitalWrite(inb1, HIGH);
        digitalWrite(inb2, LOW);
        digitalWrite(ina1, LOW);
        digitalWrite(ina2, HIGH);
        ledcWrite(canala, 150);
        ledcWrite(canalb, 150);
        Espera(450);
        ledcWrite(canala, 0);
        ledcWrite(canalb, 0);
        Espera(200);
    }
  }
		
		
	else {
    Serial.println("Andar para TRÁS");
    while ( distanciaCm() <= 10) {
      digitalWrite(ina1, LOW);
      digitalWrite(inb1, LOW);
      digitalWrite(ina2, HIGH);
      digitalWrite(inb2, HIGH);
      ledcWrite(canala, 120);
      ledcWrite(canalb, 130);
    Espera(200);

      }
  }
		
}

void Espera(unsigned long tempo) {
  unsigned long tempoatual = millis();
  while (tempoatual + tempo > millis()) {}
}
