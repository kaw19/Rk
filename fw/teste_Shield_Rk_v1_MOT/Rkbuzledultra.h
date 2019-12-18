/* RKbuzledultra.h - Arquivo cabeçalho para o roboKAW (RK)
   Procedimentos básicos do robô: esperar, apitar, ...
   29.10.2018 - Prof. Cláudio A. Fleury */

#define BUZ           8               // acionamento do buzzer
#define FREQBUZ       1000            // frequência em Hz do apito do buzzer (não é piezo speaker)
#define LIGA          HIGH
#define DESLIGA       LOW
#define VERM          A5
#define VERD          A4
#define AZUL          A3
#define DIREITO       A1              // pino do sinal digital do sensor IV à esquerda do Rk
#define ESQUERDO      A0              // pino do sinal digital do sensor IV à direita do Rk               
#define TRIG          5               // pino de disparo dos 8 pulsos de 40 kHz do HC-SR04
#define ECHO          4               // pino de retorno do HC-SR04
#define LIM_IV        500             // limiar de mudança de contraste (linha) da reflexão do IV na superfície
#define CLARA         true
#define ESCURA        false

/* Protótipos: */
bool infravermelho(byte sensor);      // retorna 'true' se 'sensor' IV (DIREITO ou ESQUERDO) tiver leitura maior que LIM_IV
int ultrassom(void);                  // retorna a dist. (cm) do RK ao obstáculo frontal
void esperar(float seg);              // atraso de tempo com CPU ocupada por 'seg' segundos
void apitar(float seg);               // apito em frequência FREQBUZ por 'seg' segundos
void led(byte cor,bool estado);       // liga/desliga ('estado') LED no pino 'cor'
int  chave(void);                     // retorna 'true' se a chave pushbutton estiver pressionada
int  ivEsq,                           // valor da leitura do sensor IV à esquerda
     ivDir;                           // valor da leitura do sensor IV à direita

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
  digitalWrite(cor, estado);
}

int chave(void) {
  return true;
}

bool infravermelho(byte sensor) {     // retorna 'true' se estiver sobre superfície clara
//  int IVDir, IVEsq;
//  IVEsq = analogRead(ESQUERDO); 
//  IVDir = analogRead(DIREITO); 
//  Serial.print("Esq.: "); Serial.println(IVEsq); 
//  Serial.print("Dir.: "); Serial.println(IVDir); 
  return analogRead(sensor) < LIM_IV;  // valor alto (1020): sensor sobre sup. escura; baixo (50): sup. clara
}
