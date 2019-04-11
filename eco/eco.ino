/* Sketch:   teste_MotoresDC_3.5
   Objetivo: movimentar RK com comandos simples: mover(), virar()
             e acionando atuadores: Buzzer e LED RGB (branco opaco, catodo [-] comum)
   Por:      Prof. Cláudio
   Em:       29.11.2018 - IFG - Campus Goiânia                    */

#include "Rkmotor.h"       // comandos de movimentação do Rk
#include "Rkbuzledultra.h" // comandos dos atuadores: Buzzer e LED RGB

void setup() {
  pinMode(TRIG,OUTPUT); pinMode(ECHO,INPUT);
  pinMode(BUZ, OUTPUT); pinMode(VERM,OUTPUT); 
  pinMode(AZUL,OUTPUT); pinMode(VERD,OUTPUT); 
  digitalWrite(VERM,LOW); digitalWrite(VERD,LOW); 
  digitalWrite(AZUL,LOW); digitalWrite(BUZ, LOW);
  pinMode(MA1, OUTPUT); pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT); pinMode(MB2, OUTPUT);
  pinMode(IP1, INPUT_PULLUP); pinMode(IP2, INPUT_PULLUP);
  pararMotor('*');                                                // desliga ambos motores
  Serial.begin(9600);
  Serial.println("RK - v.1.0 Nov/2018\n");
  furosA = furosB = 0;
  attachInterrupt(digitalPinToInterrupt(IP1), motorA, RISING);    // interrupção gerada pelo Encoder do motor 'A'
  attachInterrupt(digitalPinToInterrupt(IP2), motorB, RISING);    // interrupção gerada pelo Encoder do motor 'B'
  led(AZUL,LIGA);           // Passo B
  apitar(0.1);              // Passo A
}

void loop() {
  if (ultrassom() > 30)
    mover(20,NORMAL);
  else { 
    pararMotor('*');
    fim();
  }
}
