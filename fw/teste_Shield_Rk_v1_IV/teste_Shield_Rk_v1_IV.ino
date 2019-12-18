/* Sketch:   teste_MotoresDC_3.5
   Objetivo: movimentar RK com comandos simples: mover(), virar()
             e acionando atuadores: Buzzer e LED RGB (branco opaco, catodo [-] comum)
   Por:      Prof. Cláudio
   Em:       29.11.2018 - IFG - Campus Goiânia                    */

#include "Rkmotor.h"       // comandos de movimentação do Rk
#include "Rkbuzledultra.h" // comandos dos atuadores: Buzzer e LED RGB

void setup() {
  pinMode(MA1, OUTPUT); pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT); pinMode(MB2, OUTPUT);
  pinMode(ENC_A, INPUT_PULLUP); pinMode(ENC_B, INPUT_PULLUP);
  iniciaEncoders();
  pararMotor('*');                                                  // desliga ambos motores
  Serial.begin(9600);
  Serial.println("RK - v.1.0 Dez/2019\n");
  led(AZUL,LIGA);           // Vivo!
  apitar(0.1);              // Oi!
}

void loop() {
  if (infravermelho(DIREITO) == CLARA && infravermelho(ESQUERDO) == CLARA)          // Ambos os sensores IV estão sobre a superfície clara (reflexão)
    avancar();
  if (infravermelho(DIREITO) == ESCURA && infravermelho(ESQUERDO) == CLARA)         // Sensor IV dir. na superf. escura, sensor IV esq. na superf. clara
    esquerda();
  if (infravermelho(DIREITO) == CLARA && !infravermelho(ESQUERDO) == ESCURA)        // Sensor IV dir. na superf. clara, sensor IV esq. na superf. escura
    direita();
  if (infravermelho(DIREITO) == ESCURA && infravermelho(ESQUERDO) == ESCURA) {      // Ambos os sensores IV estão sobre a superf. clara (linha transversal)
    led(AZUL,LIGA);
    avancar();
    esperar(1);
    pararMotor('*');
    fim();
  }
}
