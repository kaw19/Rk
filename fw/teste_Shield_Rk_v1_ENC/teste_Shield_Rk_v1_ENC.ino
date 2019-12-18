/* Sketch:   teste_MotoresDC_3.5
   Objetivo: movimentar RK com comandos simples: mover(), virar()
             e acionando atuadores: Buzzer e LED RGB (branco opaco, catodo [-] comum)
   Por:      Prof. Cláudio
   Em:       29.11.2018 - IFG - Campus Goiânia                    */

#include "Rkmotor.h"       // comandos de movimentação do Rk
#include "Rkbuzledultra.h" // comandos dos atuadores: Buzzer e LED RGB

unsigned long tick;

void setup() {
  pinMode(BUZ, OUTPUT); pinMode(VERM,OUTPUT); 
  pinMode(AZUL,OUTPUT); pinMode(VERD,OUTPUT); 
  pinMode(MA1, OUTPUT); pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT); pinMode(MB2, OUTPUT);
  pinMode(IP1, INPUT_PULLUP); pinMode(IP2, INPUT_PULLUP);
  pararMotor('*');          // desliga motores
  Serial.begin(9600);
  Serial.println("RK - v.1.0 Dez/2019\n");
  iniciaEncoders();         // configura Encoders
  led(AZUL,LIGA);           // Vivo!
  apitar(0.1);              // Oi!
  tick = millis();
}

void loop() {
  acionaMotor(PRAFRENTE, '*', NORMAL);
  while (millis()-tick < 2000) {
    Serial.print("Furos Encoder A: "); Serial.println(furosA);
    Serial.print("Furos Encoder B: "); Serial.println(furosB);
    Serial.println();
    delay(200);
  }
  pararMotor('*');
  esperar(0.5);
  tick = millis();
}
