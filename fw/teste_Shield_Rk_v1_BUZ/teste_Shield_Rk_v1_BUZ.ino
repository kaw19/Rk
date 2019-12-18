/* Sketch:   teste_Shield_RK_v1_LED_RGB
   Objetivo: acionar o atuador óptico (LED RGB)
   Por:      Prof. Cláudio
   Em:       15.12.2019 - IFG - Campus Goiânia                    */

#include "Rkbuzledultra.h" // comandos dos atuadores: Buzzer e LED RGB

void aciona_led(byte cor, float tlig, float tdesl) {
  led(cor,LIGA); esperar(tlig);
  led(cor,DESLIGA); esperar(tdesl);
}

void setup() {
  pinMode(BUZ,OUTPUT); 
  digitalWrite(BUZ,LOW); 
  Serial.begin(9600);
  Serial.println("RK - v.2.0 Dez/2019\n");
}

void loop() {
  apitar(0.5);           
  esperar(1.0);
}
