/* RKmotor.h - Arquivo cabeçalho para o roboKAW (RK)
   Procedimentos básicos do robô: mover, virar, pararMotor, ...
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
#define BAIXA         20
#define NORMAL        75
#define ALTA          180
#define FLOAT2INT(x)  ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
#define AJUSTE_MA     1.0
#define AJUSTE_MB     1.0

/* Protótipos: */
void mover(int dist, byte vel);       // move 'dist' cm em relação ao local no instante do comando
void virar(int angulo);               // vira 'angulo' graus: ang. posit. (anti-horário) vira  p/ esq.
void pararMotor(char motor);          // desliga o motor indicado pelo parâmetro 'motor'
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

void pararMotor(char motor) {
  switch (motor) {
    case 'A': digitalWrite(MA1, LOW); digitalWrite(MA2, LOW); break;
    case 'B': digitalWrite(MB1, LOW); digitalWrite(MB2, LOW); break;
    case '*': digitalWrite(MA1, LOW); digitalWrite(MA2, LOW); 
              digitalWrite(MB1, LOW); digitalWrite(MB2, LOW); 
  }
}

void finalizaMov(void) {
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
                    Dir_B = MB1; PWM_B = MB2; break;
    case PRADIR:    Dir_A = MA1; PWM_A = MA2;
                    Dir_B = MB2; PWM_B = MB1;
  }
  switch (motor) {                                // liga motor(es)
    case 'A': digitalWrite(Dir_A, LOW); analogWrite(PWM_A, vel*AJUSTE_MA); while (furosA > 0); break;
    case 'B': digitalWrite(Dir_B, LOW); analogWrite(PWM_B, vel*AJUSTE_MB); while (furosB > 0); break;
    case '*': digitalWrite(Dir_A, LOW); analogWrite(PWM_A, vel*AJUSTE_MA);  
              digitalWrite(Dir_B, LOW); analogWrite(PWM_B, vel*AJUSTE_MB);  
              while (furosA > 0 || furosB > 0);   // aguarda até que o movimento seja finalizado
  }
  //finalizaMov(); 
  pararMotor(motor); 
  Serial.print("Furos A: "); Serial.print(furosA, DEC); Serial.print(" Furos B: "); Serial.println(furosB, DEC); 
}

void virar(int angulo) {              
  /* vira 'angulo' (graus): ang. posit. vira  p/ esq. (anti-horário) */
  int dist = abs(angulo)*22/90;

  furosB = furosA = dist;
  // Serial.print("Virar::dist = "); Serial.println(dist);
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
  int furos = FLOAT2INT(abs(dist)*2.666*20/20.9)-20; 
  
  if (furos < 20) furos = 21;                   // acionamento mínimo de 1 volta = 20 furos
  furosA = furosB = furos;
  Serial.print("FurosA: "); Serial.println(furosA, DEC); 
  Serial.print("FurosB : "); Serial.println(furosB, DEC); 
  acionaMotor((dist < 0) ? PRATRAS : PRAFRENTE,'*',vel);
  delay(1000);
}

void avancar() {
  Dir_A = MA1; PWM_A = MA2;
  Dir_B = MB1; PWM_B = MB2;                       // padrão: PRAFRENTE
  digitalWrite(Dir_A, LOW); analogWrite(PWM_A, NORMAL * AJUSTE_MA);  // motor da esquerda
  digitalWrite(Dir_B, LOW); analogWrite(PWM_B, NORMAL * AJUSTE_MB);  // motor da direita  
}

void esquerda() {
  Dir_A = MA1; PWM_A = MA2;
  Dir_B = MB1; PWM_B = MB2;                       // padrão: PRAFRENTE
  digitalWrite(Dir_A, LOW); analogWrite(PWM_A, ALTA * AJUSTE_MA);   // motor da esquerda
  digitalWrite(Dir_B, LOW); analogWrite(PWM_B, BAIXA * AJUSTE_MB);  // motor da direita  
}

void direita() {
  Dir_A = MA1; PWM_A = MA2;
  Dir_B = MB1; PWM_B = MB2;                       // padrão: PRAFRENTE
  digitalWrite(Dir_B, LOW); analogWrite(PWM_B, ALTA * AJUSTE_MB);   // motor da direita  
  digitalWrite(Dir_A, LOW); analogWrite(PWM_A, BAIXA * AJUSTE_MA);  // motor da esquerda
}

//void desacelera() {
//  while ((furosA > 20) && (furosB > 20));     // aguarda até que falte 1 volta para diminuir a velocidade
//  if (furosA <= 20) {
//    Serial.print("FurosA: "); Serial.println(furosA);
//    analogWrite(pinoPWM_A, BAIXA);            // reduz velocidade do Motor 'A' p/ alcançar contagem precisa de furos
//    while(furosA > 0);  
//    pararMotor('A');
//    Serial.print("FurosB: "); Serial.println(furosB);
//    while(furosB > 20); 
//    analogWrite(pinoPWM_B, BAIXA);            // reduz velocidade do Motor 'B' p/ alcançar contagem precisa de furos
//    while(furosB > 0);  
//    pararMotor('B');
//  }
//  else {
//    Serial.print("FurosB: "); Serial.println(furosB);
//    analogWrite(pinoPWM_B, BAIXA);            // reduz velocidade do Motor 'B' p/ alcançar contagem precisa de furos
//    while(furosB > 0);
//    pararMotor('B');
//    while(furosA > 20);
//    analogWrite(pinoPWM_A, BAIXA);            // reduz velocidade do Motor 'A' p/ alcançar contagem precisa de furos
//    Serial.print("FurosA: "); Serial.println(furosA);
//    while(furosA > 0);                        
//    pararMotor('A');
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
