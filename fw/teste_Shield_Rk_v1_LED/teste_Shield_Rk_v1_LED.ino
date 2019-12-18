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
  pinMode(VERM,OUTPUT); 
  pinMode(VERD,OUTPUT); 
  pinMode(AZUL,OUTPUT); 
  digitalWrite(VERM,HIGH); 
  digitalWrite(VERD,HIGH); 
  digitalWrite(AZUL,HIGH); 
  Serial.begin(9600);
  Serial.println("RK - v.2.0 Dez/2019\n");
}

void loop() {
  aciona_led(VERM,0.5,0.2);           
  aciona_led(VERD,0.3, 0.2); 
  aciona_led(AZUL,0.2, 0.2);           
  esperar(1.0);
}
