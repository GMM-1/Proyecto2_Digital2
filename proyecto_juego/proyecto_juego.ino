/* Nombre: Gabriel Andrade || Michelle Serrano
 *  universidad del valle de Guatemala 
 *  Proyecto: Juego de duelo de vaqueros (batman vs superman)
 *  curso: Digital 2
 *  materiales: TIVA C placa de desarrollador, pantalla ili9341 2,8'', cuatro push buttons 
 *  commit inicial: 15/04/2023
 *  commit final:
 *  ultima revision:
 */

//************************************************************************************************************************************************************************
//******************************************************************************librerias********************************************************************************* 
//************************************************************************************************************************************************************************
#include <stdint.h>              //libreria de funciones 
#include <ili9341_8_bits_fast.h> //libreria para el uso de pantalla tft
#include "bitmaps.h"             //adjuntamos el archivo con los bitmaps 

//************************************************************************************************************************************************************************
//**************************************************************************definiciones********************************************************************************** 
//************************************************************************************************************************************************************************
#define der_bat PE_0  //boton derecho de J Batman
#define mid_bat PC_5  //boton medio de J Batman
#define iz_bat PF_0   //boton izquierdo de J Batman

#define der_sup PD_7  //boton derecho de J Superman
#define mid_sup PD_6  //boton medio de J Superman 
#define iz_sup PC_4   //boton izquierdo de J Superman 

//************************************************************************************************************************************************************************
//**************************************************************************variables************************************************************************************* 
//************************************************************************************************************************************************************************
int x_bat = 180;      //variable para trackear movimiento horizontal de batman 
int x_sup = 0;        //variable para trackear movimiento horizontal de superman 
int game_state = 0;   //estado del juego por etapa (start, movimiento libre, seleccion de disparo, seleccion de esquive, pantalla ganador)

//banderas para disparos y saltos 
//estas banderas se activan cuando el usuario presiona el push correspondiente en el juego 

//banderas de disparo
int bandera_bat_shoot_up = 0; 
int bandera_bat_shoot_down = 0;
int bandera_bat_shoot_mid = 0;
int bandera_sup_shoot_up = 0; 
int bandera_sup_shoot_down = 0; 
int bandera_sup_shoot_mid = 0;
//banderas de esquivo 
int bandera_bat_move_up = 0; 
int bandera_bat_move_down = 0;
int bandera_bat_move_mid = 0;
int bandera_sup_move_up = 0; 
int bandera_sup_move_down = 0; 
int bandera_sup_move_mid = 0;
//************************************************************************************************************************************************************************
//******************************************************************************setup************************************************************************************* 
//************************************************************************************************************************************************************************
void setup()
{
  //controles 
  pinMode(iz_bat, INPUT_PULLUP); 
  pinMode(der_bat, INPUT_PULLUP);
  pinMode(mid_bat, INPUT_PULLUP);
  pinMode(iz_sup, INPUT_PULLUP); 
  pinMode(der_sup, INPUT_PULLUP);  
  pinMode(mid_sup, INPUT_PULLUP);




  //configuracion de la comunicacion serial 
  Serial.begin(9600);
  Serial3.begin(9600);

  //configuracion inicial de la pantalla 
  lcdInit(); //inicializacion de la lcd 
  lcdClear(0xECEB); //color del fondo 
  delay(100);
  //dibujamos el fondo8/j
  FillRect(0, 200, 320, 100, BLACK); //dibujar suelo 
  LCD_Print("Duelo de Vaqueros", 30, 50, 2, PINK, BLACK); 
  LCD_Print("BATMAN VS SUPERMAN", 20, 220, 2, PINK, BLACK); 
  FillRect(0, 130, 50,10, BLUE); //posicion SUPERMAN
  FillRect(230, 130, 50,10, RED); //posicion BATMAN

}

//************************************************************************************************************************************************************************
//******************************************************************************ciclo infinito**************************************************************************** 
//************************************************************************************************************************************************************************
void loop()
{
  switch (game_state)
  {
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
  case 0: //movimiento libre de los jugadores para tomar posiciones 
  Serial3.write(1);
  //asignamos la interrupcion de movimiento de los pushes 
  attachInterrupt(digitalPinToInterrupt(der_bat), mov_right_bat, LOW);
  attachInterrupt(digitalPinToInterrupt(iz_bat), mov_left_bat, LOW);
  attachInterrupt(digitalPinToInterrupt(der_sup), mov_right_sup, LOW);
  attachInterrupt(digitalPinToInterrupt(iz_sup), mov_left_sup, LOW);

  //condicion para que el juego empiece
  if(x_bat > 230 and x_bat < 280 and x_sup > 0 and x_sup < 50)
  {game_state = 1; // bandera para iniciar el juego 
  FillRect(0, 200, 320, 100, BLACK);} //dibujar suelo  
  break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  case 1: //estado de eleccion de disparo
  Serial3.write(2); 

  //inmovilizamos a los jugadores
  detachInterrupt(der_bat);
  detachInterrupt(iz_bat);
  detachInterrupt(der_sup);
  detachInterrupt(iz_sup);
  LCD_Print("Elija posicion a disparar", 20, 220, 2, PINK, BLACK); 
  //buscamos donde disparo el J Batman
  if (digitalRead(iz_bat) == LOW)
  {bandera_bat_shoot_up = 1;
  Serial3.write(2); }
   if (digitalRead(der_bat) == LOW)
  {bandera_bat_shoot_down = 1;
  Serial3.write(2); }
   if (digitalRead(mid_bat) == LOW)
  {bandera_bat_shoot_mid = 1;
  Serial3.write(2); }

  //buscamos donde disparo el J Superman
  if (digitalRead(iz_sup) == LOW)
  {bandera_sup_shoot_up = 1;
  Serial3.write(2); };
   if (digitalRead(der_sup) == LOW)
  {bandera_sup_shoot_down = 1;
  Serial3.write(2); }
   if (digitalRead(mid_sup) == LOW)
  {bandera_sup_shoot_mid = 1;
  Serial3.write(2); }
  Serial3.write(2); 

  //condicion para cambiar de estado cuando ambos elijieron a donde disparar 
  if (((bandera_bat_shoot_up == 1)or(bandera_bat_shoot_mid == 1)or(bandera_bat_shoot_down == 1)) and ((bandera_sup_shoot_up == 1)or(bandera_sup_shoot_mid == 1)or(bandera_sup_shoot_down == 1)))
  {game_state = 2;
  FillRect(0, 200, 320, 100, BLACK);} //dibujar suelo 
  break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  case 2: //estado de eleccion de defensa
  
  LCD_Print("Elija posicion a esquivar", 20, 220, 1, PINK, BLACK); 
  //buscamos donde se mueve el J Batman
  if (digitalRead(iz_bat) == LOW)
  {bandera_bat_move_up = 1;}
   if (digitalRead(der_bat) == LOW)
  {bandera_bat_move_down = 1;}
   if (digitalRead(mid_bat) == LOW)
  {bandera_bat_move_mid = 1;}

  //buscamos donde se mueve el J Superman
  if (digitalRead(iz_sup) == LOW)
  {bandera_sup_move_up = 1;}
   if (digitalRead(der_sup) == LOW)
  {bandera_sup_move_down = 1;}
   if (digitalRead(mid_sup) == LOW)
  {bandera_sup_move_mid = 1;}

  //condicion para cambiar de estado cuando ambos elijieron a donde mover
  if (((bandera_bat_move_up == 1)or(bandera_bat_move_mid == 1)or(bandera_bat_move_down == 1)) and ((bandera_sup_move_up == 1)or(bandera_sup_move_mid == 1)or(bandera_sup_move_down == 1)))
  {game_state = 3;
  {FillRect(0, 200, 320, 100, BLACK);} //dibujar suelo
  break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 case 3: //pantalla del ganador 
 Serial3.print(1);
if (((bandera_bat_move_up == 1) and (bandera_sup_shoot_up)) or ((bandera_bat_move_down == 1) and (bandera_sup_shoot_down)) or ((bandera_bat_move_mid == 1) and (bandera_sup_shoot_mid)))
  {LCD_Print("Ganador Superman", 20, 220, 1, PINK, BLACK);
  Serial3.print(1);}
//condicional para encontrar si gana Batman
if (((bandera_bat_shoot_up == 1) and (bandera_sup_move_up)) or ((bandera_bat_shoot_down == 1) and (bandera_sup_move_down)) or ((bandera_bat_shoot_mid == 1) and (bandera_sup_move_mid)))
  LCD_Print("Ganador Batman", 20, 220, 1, PINK, BLACK);
  Serial3.print(1);}


  break;
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  }
}
//************************************************************************************************************************************************************************
//******************************************************************************funciones********************************************************************************* 
//************************************************************************************************************************************************************************

void mov_right_sup(){
  //superman hacia la derecha
  while ((digitalRead(der_sup) == LOW) && (x_sup < 320 - 34))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int sup_index = (x_sup/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_sup++, 151, 17, 24, sup_run, 3, sup_index, 1, 0,0); //Mostrar imagen
  V_line( x_sup - 1, 151, 51, 0xECEB); //Limpiar rastro
  }
}

void mov_left_sup(){
  //superaman hacia la izquierda 
  while ((digitalRead(iz_sup) == LOW) && (x_sup > 0))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int sup_index = (x_sup/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_sup--, 151, 17, 24, sup_run, 3, sup_index, 0, 0,0); //Mostrar imagen
  V_line( x_sup + 18, 151, 51, 0xECEB); //Limpiar rastro
  }
}

void mov_right_bat()
{
    //batman hacia la derecha 
  while ((digitalRead(der_bat) == LOW) && (x_bat < 320 - 46))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int bat_index = (x_bat/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_bat++, 148, 23, 26, bat_run, 3, bat_index, 1, 0, 0); //Mostrar imagen
  V_line( x_bat - 1, 148, 51, 0xECEB); //Limpiar rastro
  }
}

void mov_left_bat(){
  //batman hacia la izquierda 
  while ((digitalRead(iz_bat) == LOW) && (x_bat > 0))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int bat_index = (x_bat/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_bat--, 148, 23, 26, bat_run, 3, bat_index, 0, 0, 0); //Mostrar imagen
  V_line( x_bat + 23, 148, 51, 0xECEB); //Limpiar rastro
  }
}
  
  
