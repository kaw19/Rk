/* Sketch:   teste_Shield_RK_v1_LED_RGB
   Objetivo: acionar o atuador óptico (LED RGB)
   Por:      Prof. Cláudio
   Em:       15.12.2019 - IFG - Campus Goiânia                    */

#include "Rkbuzledultra.h"     // comandos dos atuadores: Buzzer e LED RGB

void setup() {
  pinMode(TRIG,OUTPUT); 
  pinMode(ECHO,INPUT);
  Serial.begin(9600);
  Serial.println("RK - v.2.0 Dez/2019\n\n");
}

void loop() {
  Serial.print("Dist. (cm) = ");
  Serial.println(ultrassom());
  esperar(0.5);
}
