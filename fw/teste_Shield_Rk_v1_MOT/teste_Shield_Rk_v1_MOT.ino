/* Sketch:   teste_Shield_Rk_v1_MOT
   Objetivo: movimentar RK com comandos simples: mover(), virar()
   Por:      Prof. Cláudio
   Em:       16.12.2019 - IFG - Campus Goiânia - Datum               */

#include "Rkmotor.h"       // comandos de movimentação do Rk
#include "RKbuzledultra.h"

void setup() {
  pinMode(MA1, OUTPUT); pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT); pinMode(MB2, OUTPUT);
  pinMode(ENC_A, INPUT_PULLUP); pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), motorA, RISING);    // interrupção gerada pelo Encoder do motor 'A'
  attachInterrupt(digitalPinToInterrupt(ENC_B), motorB, RISING);    // interrupção gerada pelo Encoder do motor 'B'
  //pararMotor('*');                                                  // desliga ambos motores
  furosA = furosB = 0;
  Serial.begin(9600);
  Serial.println("RK - v.1.0 Dez/2019\n");
}

void loop() {
  led(VERM, HIGH);
  Serial.print("*");
  avancar(); 
  esperar(3);
  led(VERM, LOW);
  Serial.println("*");
  /*esquerda(); esperar(1);
  direita(); 
  esperar(1);
  pararMotor('*');
  fim();*/
  pararMotor('*');
  esperar(1.);
}
