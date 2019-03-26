#include <Arduino.h>
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

/* RKmotor.h - Arquivo cabeçalho para o roboKAW (RK)
   Procedimentos básicos do robô: mover, virar, paraMotor, ...
   22.10.2018 - Prof. Cláudio A. Fleury */

#define MA1           10              // polo 1 do Motor 'A'
#define MA2           11              // polo 2 do Motor 'A'
#define MB1           6               // polo 1 do Motor 'B'
#define MB2           9               // polo 2 do Motor 'B'
#define IP1           2               // pino da interrupção do Encoder 'A'
#define IP2           3               // pino da interrupção do Encoder 'B'
#define FPV           20              // furos do Encoder por volta
#define RAIO          2.825           // diâmetro da roda traseira: 5.65 cm
#define PRAFRENTE     1
#define PRATRAS       2
#define PRAESQ        3
#define PRADIR        4
#define BAIXA         120
#define NORMAL        160
#define ALTA          200
#define FLOAT2INT(x)  ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

/* Protótipos: */
void mover(int dist, byte vel);       // move 'dist' cm em relação ao local no instante do comando
void virar(int angulo);               // vira 'angulo' graus: ang. posit. (anti-horário) vira  p/ esq.
void paraMotor(char motor);           // desliga o motor indicado pelo parâmetro 'motor'
void fim(void);                       // laço eterno para suspender atividades do RK
void acionaMotor(int direcao, char motor, byte vel);

/* Variáveis globais */
volatile int furosA, furosB;          // furos do disco do Encoder dos motores 'A' e 'B', respectivamente
byte Dir_A, PWM_A, Dir_B, PWM_B;

/* Implementações */
void motorA() {                       // ISR para interrupção 0 (pino 2 no SEEEDUINO)
  --furosA;                           // decrementa contador de furos
}

void motorB() {                       // ISR para interrupção 1 (pino 3 no SEEEDUINO)
  --furosB;                           // decrementa contador de furos
}

void fim(void) {
  while (true);
}

void paraMotor(char motor) {
  switch (motor) {
    case 'A': digitalWrite(MA1, LOW); digitalWrite(MA2, LOW); break;
    case 'B': digitalWrite(MB1, LOW); digitalWrite(MB2, LOW); break;
    case '*': digitalWrite(MA1, LOW); digitalWrite(MA2, LOW); 
              digitalWrite(MB1, LOW); digitalWrite(MB2, LOW); 
  }
}

void finalizaMov(void) {
  Serial.println("finalizaMov");
  if (furosA < -3) {
    furosA = -furosA; digitalWrite(PWM_A, LOW); analogWrite(Dir_A, NORMAL); while (furosA > 0);
    Serial.print("Furos A: ");  Serial.println(furosA, DEC); 
  }
  if (furosB < -3) {
    furosB = -furosB; digitalWrite(PWM_B, LOW); analogWrite(Dir_B, NORMAL); while (furosB > 0);
    Serial.print(" Furos B: "); Serial.println(furosB, DEC); 
  }
}

void acionaMotor(int dir, char motor, byte vel) {

  Dir_A = MA1; PWM_A = MA2;
  Dir_B = MB1; PWM_B = MB2;                       // padrão: PRAFRENTE
  switch (dir) {                                  // ajusta polaridade dos motores para o 'sentido' desejado
    case PRATRAS:   Dir_A = MA2; PWM_A = MA1;
                    Dir_B = MB2; PWM_B = MB1; break;
    case PRAESQ:    Dir_A = MA2; PWM_A = MA1;
                    Dir_B = MB1; PWM_B = MB2; Serial.println("PRAESQ"); break;
    case PRADIR:    Dir_A = MA1; PWM_A = MA2;
                    Dir_B = MB2; PWM_B = MB1; Serial.println("PRADIR"); break;
  }
  switch (motor) {                                // liga motor(es)
    case 'A': digitalWrite(Dir_A, LOW); analogWrite(PWM_A, vel); while (furosA > 0); break;
    case 'B': digitalWrite(Dir_B, LOW); analogWrite(PWM_B, vel); while (furosB > 0); break;
    case '*': digitalWrite(Dir_A, LOW); analogWrite(PWM_A, vel);  
              digitalWrite(Dir_B, LOW); analogWrite(PWM_B, vel);  
              while (furosA > 0 || furosB > 0);   // aguarda até que o movimento seja finalizado
  }
  //finalizaMov(); 
  paraMotor(motor); 
  Serial.print("Furos A: "); Serial.print(furosA, DEC); Serial.print(" Furos B: "); Serial.println(furosB, DEC); 
}

void virar(int angulo) {              
  /* vira 'angulo' (graus): ang. posit. vira  p/ esq. (anti-horário) */
  int dist = abs(angulo)*22/90;

  furosB = furosA = dist;
  Serial.print("Virar::dist = "); Serial.println(dist);
  if (angulo > 0)                               // vira para esquerda (anti-horário)
    acionaMotor(PRAESQ, '*', NORMAL);
  else                                          // vira para direita (horário)
    acionaMotor(PRADIR, '*', NORMAL);
  delay(1000);
}

void mover(int dist, int vel) {
  /*  Move o RK por uma distância de 'dist' (cm) na velocidade 'vel' (BAIXA, NORMAL, RAPIDO)
     fica em espera ocupada até terminar o movimento comandado (aguarda contagem de furos do encoder)
     Diâmetro da roda: 6,65 cm
     const float circunf = 3.1415 * 6.65 = 20.9; */
  int furos = FLOAT2INT(abs(dist)*2.66*20/20.9)-20; 
  
  if (furos < 20) furos = 21;                   // acionamento mínimo de 1 volta = 20 furos
  furosA = furosB = furos;
  // Serial.print("FurosA: "); Serial.println(furosA, DEC); 
  acionaMotor((dist < 0) ? PRATRAS : PRAFRENTE,'*',vel);
  delay(1000);
}

//void desacelera() {
//  while ((furosA > 20) && (furosB > 20));     // aguarda até que falte 1 volta para diminuir a velocidade
//  if (furosA <= 20) {
//    Serial.print("FurosA: "); Serial.println(furosA);
//    analogWrite(pinoPWM_A, BAIXA);            // reduz velocidade do Motor 'A' p/ alcançar contagem precisa de furos
//    while(furosA > 0);  
//    paraMotor('A');
//    Serial.print("FurosB: "); Serial.println(furosB);
//    while(furosB > 20); 
//    analogWrite(pinoPWM_B, BAIXA);            // reduz velocidade do Motor 'B' p/ alcançar contagem precisa de furos
//    while(furosB > 0);  
//    paraMotor('B');
//  }
//  else {
//    Serial.print("FurosB: "); Serial.println(furosB);
//    analogWrite(pinoPWM_B, BAIXA);            // reduz velocidade do Motor 'B' p/ alcançar contagem precisa de furos
//    while(furosB > 0);
//    paraMotor('B');
//    while(furosA > 20);
//    analogWrite(pinoPWM_A, BAIXA);            // reduz velocidade do Motor 'A' p/ alcançar contagem precisa de furos
//    Serial.print("FurosA: "); Serial.println(furosA);
//    while(furosA > 0);                        
//    paraMotor('A');
//  }
//}

/* Dados Técnicos de tração:
Tensão de alimentação dos motores:  3 ~ 6 VDC
Corrente máxima:                    120 mA
Relação da redução:                 48:1
Rotação do motor em 6 VDC:          260 rpm
Velocidade do carro a 6VDC:         90,6 cm/s (aprox. 1 m/s)
Dimensões da roda:                  6,65 x 2,6 cm
Perímetro da roda:                  20,9 cm                    */

void setup() {
  pinMode(BUZ, OUTPUT); pinMode(VERM,OUTPUT); digitalWrite(VERM,HIGH);
  pinMode(AZUL,OUTPUT); pinMode(VERD,OUTPUT); digitalWrite(VERD,HIGH); digitalWrite(AZUL,HIGH);
  pinMode(MA1, OUTPUT); pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT); pinMode(MB2, OUTPUT);
  pinMode(IP1, INPUT_PULLUP); pinMode(IP2, INPUT_PULLUP);
  paraMotor('*');                                                 // desliga ambos motores
  furosA = furosB = 0;
  Serial.begin(9600);
  Serial.println("RK - v.1.0 Out/2018\n");
  attachInterrupt(0, motorA, RISING);    // interrupção gerada pelo Encoder do motor 'A'
  attachInterrupt(1, motorB, RISING);    // interrupção gerada pelo Encoder do motor 'B'
}

