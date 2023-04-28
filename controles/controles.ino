#include <stdint.h>              //libreria de funciones 

#define sup_up_in PE_0
#define sup_dn_in PF_2
#define sup_mid_in PF_3
#define bat_up_in PB_0
#define bat_dn_in PB_5
#define bat_mid_in PD_0


void setup() {

pinMode(sup_up_in, INPUT_PULLUP);
pinMode(sup_dn_in, INPUT_PULLUP);
pinMode(sup_mid_in, INPUT_PULLUP);
pinMode(bat_up_in, INPUT_PULLUP);
pinMode(bat_dn_in, INPUT_PULLUP);
pinMode(bat_mid_in, INPUT_PULLUP);



Serial.begin(9600);
}

void loop() { 
}
