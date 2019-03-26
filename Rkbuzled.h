/* RKbuzled.h - Arquivo cabeçalho para o roboKAW (RK)
   Procedimentos básicos do robô: esperar, apitar, ...
   29.10.2018 - Prof. Cláudio A. Fleury */

#define BUZ           4               // acionamento do buzzer
#define FREQBUZ       1000            // frequência em Hz do apito do buzzer (não é piezo speaker)
#define LIGA          HIGH
#define DESLIGA       LOW
#define VERM          8
#define VERD          5
#define AZUL          7

/* Protótipos: */
void esperar(float seg);              // atraso de tempo com CPU ocupada por 'seg' segundos
void apitar(float seg);               // apito em frequência FREQBUZ por 'seg' segundos
void led(byte cor,bool estado);       // liga/desliga ('estado') LED no pino 'cor'

void esperar(float seg) {
  delay(seg*1000);
}

void apitar(float seg) {
  digitalWrite(BUZ, HIGH);
  delay(seg*1000);
  digitalWrite(BUZ, LOW);
}

void led(byte cor,bool estado) {      // LED usado é do tipo Anodo Comum (5V)
  digitalWrite(cor, !estado);
}
