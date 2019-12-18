/* RKbuzled.h - Arquivo cabeçalho para o roboKAW (RK)
   Procedimentos básicos do robô: esperar, apitar, ...
   29.10.2018 - Prof. Cláudio A. Fleury */

#define BUZ           5               // acionamento do buzzer
#define FREQBUZ       1000            // frequência em Hz do apito do buzzer (não é piezo speaker)
#define LIGA          HIGH
#define DESLIGA       LOW
#define VERM          A5              // LED vermelho
#define VERD          A4              // LED verde
#define AZUL          A3              // LED azul
#define TRIG          13              // pino de disparo dos 8 pulsos de 40 kHz do HC-SR04
#define ECHO          12              // pino de retorno do HC-SR04
//#define LED_CAT_COMUM
#define LED_AN_COMUM

/* Protótipos: */
int ultrassom(void);                  // retorna a dist. (cm) do RK ao obstáculo frontal
void esperar(float seg);              // atraso de tempo com CPU ocupada por 'seg' segundos
void apitar(float seg);               // apito em frequência FREQBUZ por 'seg' segundos
void led(byte cor,bool estado);       // liga/desliga ('estado') LED no pino 'cor'

int ultrassom(void) {
  long duracao;
  digitalWrite(TRIG, LOW);            // coloca em nível baixo o pino de disparo (trigger)
  delayMicroseconds(5);               // aguarda tempo mínimo
  digitalWrite(TRIG, HIGH);           // sobe o nível para iniciar emissão de 8 pulsos de 40 kHz
  delayMicroseconds(10);              // aguarda tempo mínimo
  digitalWrite(TRIG, LOW);            // retorna sinal para o nível baixo
  duracao = pulseIn(ECHO, HIGH);      // lê o sinal ECHO e retorna o tempo de viagem dos pulsos em us
  return duracao*0.017;               // calcula e retorna a distância em cm: d = v.t
}

void esperar(float seg) {
  delay(seg*1000);
}

void apitar(float seg) {
  digitalWrite(BUZ, HIGH);
  delay(seg*1000);
  digitalWrite(BUZ, LOW);
}

void led(byte cor,bool estado) {      // LED usado é do tipo Anodo Comum (5V)
#ifdef LED_AN_COMUM
  digitalWrite(cor, !estado);
#else
  digitalWrite(cor, estado);
#endif
}
