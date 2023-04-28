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

#include <SD.h>
#include <SPI.h>

File myFile;

//************************************************************************************************************************************************************************
//**************************************************************************definiciones********************************************************************************** 
//************************************************************************************************************************************************************************
#define der_bat PE_0  //boton derecho de J Batman
#define mid_bat PC_5  //boton medio de J Batman
#define iz_bat PF_0   //boton izquierdo de J Batman

#define der_sup PD_7  //boton derecho de J Superman
#define mid_sup PD_6  //boton medio de J Superman 
#define iz_sup PC_4   //boton izquierdo de J Superman

//colores
#define TRANSPARENT_COLOR 0x6DDB  //color del cielo 

//************************************************************************************************************************************************************************
//**************************************************************************variables************************************************************************************* 
//************************************************************************************************************************************************************************
int x_bat = 180;      //variable para trackear movimiento horizontal de batman 
int x_sup = 0;        //variable para trackear movimiento horizontal de superman 
int y_bat = 0;
int menu = 0;         //estado del menu para elegir escenario/jugadores
//int posicion_x = 0;
//int posicion_las = 0;
int game_state = 20;   //inicia el juego en la pantalla de inicio y menú

//banderas para disparos y saltos, estas banderas se activan cuando el usuario presiona el push correspondiente en el juego 

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
//banderas armas
int boomerang_mostrado_day = 0;
int boomerang_mostrado_night = 0;
int laser_mostrado_day = 0;
int laser_mostrado_night =0;
//banderas empate
int bandera_bat_gana = 0;
int bandera_sup_gana = 0;
//variables memoria flash
extern uint8_t laser_day[];
extern uint8_t batbo_day[];
extern uint8_t laser_night[];
extern uint8_t batbo_night [];
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
  pinMode(PE_1, INPUT_PULLUP);
  pinMode(PE_2, INPUT_PULLUP);

  //configuracion de la comunicacion serial SD
  Serial.begin(9600);
  Serial3.begin(9600);
   pinMode(PA_3, OUTPUT); //Se define PA_3 como salida y se convierte en slave select
   SPI.setModule(0); //Utilizar el SPI Module 0 
   
   if (!SD.begin(PA_3)){ //Verificar que el slave select este en 0, y se responda un 1
      Serial.println("initialization failed!"); //Si no falló la inicialización
      return;
    }

//configuración de la pantalla
    lcdInit(); //inicializacion de la lcd 
    lcdClear(0x000); //limpiar  
   
}

//************************************************************************************************************************************************************************
//******************************************************************************ciclo infinito**************************************************************************** 
//************************************************************************************************************************************************************************
void loop()
{
    
  switch (game_state)
  {
 ///////////////////////////////////////////////////////////////////////////////INICIO///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 20:
    myFile = SD.open("inicio.txt");
    mapeoSD();
    myFile.close(); 
    LCD_Print("BATMAN", 30, 160, 2, WHITE, BLACK);
    delay (500);
    LCD_Print("V", 150, 160, 2, WHITE, BLACK);
    delay (500);
    LCD_Print("SUPERMAN", 180, 160, 2, WHITE, BLACK);
    delay (1000);
    FillRect(120, 200, 320, 15, BLACK);//dibujar suelo
    delay (500);
    LCD_Print("START", 120, 200, 2, RED, BLACK);
    
    game_state = 10;  //escoger modo diurno o nocturno
   break;
   
 ///////////////////////////////////////////////////////////////////////////////MENU///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   case 10: 

  if (digitalRead(PE_1)==LOW){ //si se presiona el PE_1 muestra los escenarios
      lcdClear (0x000);
      myFile = SD.open("matriz.txt"); 
      mapeoSD();
      myFile.close(); 
      LCD_Print("ELIJA SU ESCENARIO", 95, 20, 1, WHITE, BLACK);
      LCD_Print("DIURNO", 190, 45, 2, WHITE, BLACK);
//      LCD_Print("CLARK V BRUCE", 180, 70, 1, WHITE, BLACK);
      LCD_Print("NOCTURNO", 30, 45, 2, WHITE, BLACK);
//      LCD_Print("BAT V SUPER", 40, 70, 1, WHITE, BLACK);
//      FillRect(0, 240, 320, 50, BLACK); //dibujar suelo 

      ;
          game_state = 11;  //escoger modo diurno o nocturno
    }  
    break;
    
////////////DIA////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  case 11: 
    if (digitalRead(PE_1)==LOW){ //si se presiona el PE_1 se escoge el modo dia 
    lcdClear (0x000);
    myFile = SD.open("diur.txt"); 
    mapeoSD();
    myFile.close();
    LCD_Print("BATMAN", 20, 25, 2, BLACK, TRANSPARENT_COLOR); 
    delay(1000);
    LCD_Print("VS", 130, 25, 2, BLACK, TRANSPARENT_COLOR); 
    delay(1000);
    LCD_Print("SUPERMAN", 170, 25, 2, BLACK, TRANSPARENT_COLOR); 
    delay(1000);
    LCD_Print("Duelo de Vaqueros", 25, 45, 2, BLACK, TRANSPARENT_COLOR);
         game_state = 0; //empieza el juego modo diurno
    }
    
////////////escoger escenario nocturno
  else if (digitalRead(PE_2)==LOW){ //si se presiona el PE_2 se escoge el modo noche 
     lcdClear(0x000); //color del fondo 
     myFile = SD.open("noct.txt"); 
     mapeoSD();
     myFile.close();
     delay(500);
    LCD_Print("BATMAN", 20, 25, 2, WHITE, BLACK); 
    delay(1000);
    LCD_Print("VS", 130, 25, 2, WHITE, BLACK); 
    delay(1000);
    LCD_Print("SUPERMAN", 170, 25, 2, WHITE, BLACK); 
    delay(1000);
    LCD_Print("Duelo de Vaqueros", 25, 45, 2, WHITE, BLACK);

        game_state = 12; //empieza el juego modo nocturno 
     }
    break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  case 12: //movimiento libre de los jugadores para tomar posiciones en modo noche
  Serial3.write(1);
  LCD_Print("POSICIONES DE JUEGO", 10, 220, 1, PURPLE, BLACK); 
  LCD_Print("BATMAN", 255, 140,1,WHITE,BLACK);
  LCD_Print("SUPERMAN", 25, 140,1,WHITE,BLACK);
  //asignamos la interrupcion de movimiento de los pushes 
  attachInterrupt(digitalPinToInterrupt(der_bat), mov_right_bat_night, LOW);
  attachInterrupt(digitalPinToInterrupt(iz_bat), mov_left_bat_night, LOW);
  attachInterrupt(digitalPinToInterrupt(der_sup), mov_right_sup_night, LOW);
  attachInterrupt(digitalPinToInterrupt(iz_sup), mov_left_sup_night, LOW);

  //condicion para que el juego empiece
  if(x_bat > 230 and x_bat < 280 and x_sup > 0 and x_sup < 50)
  {game_state = 13; // bandera para iniciar el juego 
  }
  break;
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  case 13: //estado de eleccion de disparo en modo noche

  //inmovilizamos a los jugadores
  detachInterrupt(der_bat);
  detachInterrupt(iz_bat);
  detachInterrupt(der_sup);
  detachInterrupt(iz_sup);
  LCD_Print("POSICION DISPARO   ", 10, 220, 1, PURPLE, BLACK); 
  //buscamos donde disparo el J Batman
  if (digitalRead(iz_bat) == LOW)
  {bandera_bat_shoot_up = 1;
  delay(100);}
   if (digitalRead(der_bat) == LOW)
  {bandera_bat_shoot_down = 1;
  delay(100);}
   if (digitalRead(mid_bat) == LOW)
  {bandera_bat_shoot_mid = 1;
  delay(100);}

  //buscamos donde disparo el J Superman
  if (digitalRead(iz_sup) == LOW)
  {bandera_sup_shoot_up = 1;
  delay(100);};
   if (digitalRead(der_sup) == LOW)
  {bandera_sup_shoot_down = 1;
  delay(100);}
   if (digitalRead(mid_sup) == LOW)
  {bandera_sup_shoot_mid = 1;
  delay(100);}

  //condicion para cambiar de estado cuando ambos elijieron a donde disparar 
  if (((bandera_bat_shoot_up == 1)or(bandera_bat_shoot_mid == 1)or(bandera_bat_shoot_down == 1)) and ((bandera_sup_shoot_up == 1)or(bandera_sup_shoot_mid == 1)or(bandera_sup_shoot_down == 1)))
  { delay(100);
    game_state = 14;
    } 
  break;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  case 14: //estado de eleccion de defensa en modo noche
  LCD_Print("POSICION DEFENSA   ",10, 220, 1, PURPLE, BLACK); 
  //buscamos donde se mueve el J Batman
 
   if (digitalRead(der_bat) == LOW)
  {bandera_bat_move_down = 1;
  delay(100);}
   if (digitalRead(mid_bat) == LOW)
  {bandera_bat_move_mid = 1;
  delay (100);}
  if (digitalRead(iz_bat) == LOW)
  {bandera_bat_move_up = 1;
  delay (100);}
  
  //buscamos donde se mueve el J Superman
   if (digitalRead(der_sup) == LOW)
  {bandera_sup_move_down = 1;
  delay(100);}
   if (digitalRead(mid_sup) == LOW)
  {bandera_sup_move_mid = 1;
  delay(100);}
   if (digitalRead(iz_sup) == LOW)
  {bandera_sup_move_up = 1;
  delay(100);}
  
  //condicion para cambiar de estado cuando ambos elijieron a donde mover
  if (((bandera_bat_move_up == 1)|(bandera_bat_move_mid == 1)|(bandera_bat_move_down == 1)) and ((bandera_sup_move_up == 1)|(bandera_sup_move_mid == 1)|(bandera_sup_move_down == 1)))
  { delay(100);
    game_state = 15;
  }
//  FillRect(0, 200, 320, 100, BLACK);} //dibujar suelo
  break;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 case 15: //pantalla del ganador en modo noche
 Serial3.write(2);
 
if (((bandera_bat_move_up == 1) and (bandera_sup_shoot_up)) or ((bandera_bat_move_down == 1) and (bandera_sup_shoot_down)) or ((bandera_bat_move_mid == 1) and (bandera_sup_shoot_mid)))
  { 
    if (laser_mostrado_night == 0) {
    FillRect(65, 154, 220, 2, RED); //dibujar laser    
    myFile = SD.open("wsup.txt"); //abrimos la imagen de ganador 
    mapeoSD();
    myFile.close(); 
    LCD_Print("GANA SUPERMAN", 90, 20, 2, BLACK, WHITE); 
    bandera_sup_gana = 1;
    laser_mostrado_night = 1;
    }
  }
    
//condicional para encontrar si gana Batman
if (((bandera_bat_shoot_up == 1) and (bandera_sup_move_up)) or ((bandera_bat_shoot_down == 1) and (bandera_sup_move_down)) or ((bandera_bat_shoot_mid == 1) and (bandera_sup_move_mid)))
  {
    if (boomerang_mostrado_night == 0) {
      
    for (int i = 220; i >= 0; i--) {
      int bat_index2 = (x_sup/5)%10; //indice para controlar la velocidad
      LCD_Sprite_Zoom(i, 148, 16, 20, batbo_night, 4, bat_index2, 0, 0, 0); //Mostrar boomerang
  //    V_line( posicion_x, 148, 51, 0xECEB); //Limpiar rastro
      }  
        myFile = SD.open("wbat.txt"); //abrimos la imagen de ganador 
        mapeoSD();
        myFile.close(); 
        LCD_Print("GANA BATMAN", 90, 20, 2, WHITE, BLACK);
        bandera_bat_gana = 1;
        boomerang_mostrado_night = 1;
    }  
   }

else if (((bandera_sup_gana == 1) and (bandera_bat_gana == 1)) or ((bandera_sup_gana == 0) and (bandera_bat_gana == 0))){
  lcdClear(0x000);
  LCD_Print("EMPATE", 160, 120, 2, WHITE, BLACK);
}
   
  break;    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
  case 0: //movimiento libre de los jugadores para tomar posiciones en modo dia
  Serial3.write(2);
  LCD_Print("POSICIONES DE JUEGO", 10, 220, 1, BLACK, 0xECEB); 
  LCD_Print("BATMAN", 255, 140,1,WHITE,0xECEB);
  LCD_Print("SUPERMAN", 25, 140,1,WHITE,0xECEB);
  //asignamos la interrupcion de movimiento de los pushes 
  attachInterrupt(digitalPinToInterrupt(der_bat), mov_right_bat_day, LOW);
  attachInterrupt(digitalPinToInterrupt(iz_bat), mov_left_bat_day, LOW);
  attachInterrupt(digitalPinToInterrupt(der_sup), mov_right_sup_day, LOW);
  attachInterrupt(digitalPinToInterrupt(iz_sup), mov_left_sup_day, LOW);

  //condicion para que el juego empiece
  if(x_bat > 230 and x_bat < 280 and x_sup > 0 and x_sup < 50)
  {game_state = 1; // bandera para iniciar el juego 
  } //dibujar suelo  
  break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  case 1: //estado de eleccion de disparo en modo dia

  //inmovilizamos a los jugadores
  detachInterrupt(der_bat);
  detachInterrupt(iz_bat);
  detachInterrupt(der_sup);
  detachInterrupt(iz_sup);
  LCD_Print("POSICION DISPARO   ", 10, 220, 1, BLACK, 0xECEB); 
  //buscamos donde disparo el J Batman
  if (digitalRead(iz_bat) == LOW)
  {bandera_bat_shoot_up = 1;
  delay(100);}
   if (digitalRead(der_bat) == LOW)
  {bandera_bat_shoot_down = 1;
  delay(100);}
   if (digitalRead(mid_bat) == LOW)
  {bandera_bat_shoot_mid = 1;
  delay(100);}

  //buscamos donde disparo el J Superman
  if (digitalRead(iz_sup) == LOW)
  {bandera_sup_shoot_up = 1;
  delay(100);};
   if (digitalRead(der_sup) == LOW)
  {bandera_sup_shoot_down = 1;
  delay(100);}
   if (digitalRead(mid_sup) == LOW)
  {bandera_sup_shoot_mid = 1;
  delay(100);}

  //condicion para cambiar de estado cuando ambos elijieron a donde disparar 
  if (((bandera_bat_shoot_up == 1)or(bandera_bat_shoot_mid == 1)or(bandera_bat_shoot_down == 1)) and ((bandera_sup_shoot_up == 1)or(bandera_sup_shoot_mid == 1)or(bandera_sup_shoot_down == 1)))
  { delay(100);
    game_state = 2;
    } //dibujar suelo 
  break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  case 2: //estado de eleccion de defensa en modo dia
  
  LCD_Print("POSICION DEFENSA   ", 10, 220, 1, BLACK, 0xECEB); 
  //buscamos donde se mueve el J Batman
 
   if (digitalRead(der_bat) == LOW)
  {bandera_bat_move_down = 1;
  delay(100);}
   if (digitalRead(mid_bat) == LOW)
  {bandera_bat_move_mid = 1;
  delay(100);}
  if (digitalRead(iz_bat) == LOW)
  {bandera_bat_move_up = 1;
  delay(100);}
  
  //buscamos donde se mueve el J Superman
   if (digitalRead(der_sup) == LOW)
  {bandera_sup_move_down = 1;
  delay(100);}
   if (digitalRead(mid_sup) == LOW)
  {bandera_sup_move_mid = 1;
  delay(100);}
   if (digitalRead(iz_sup) == LOW)
  {bandera_sup_move_up = 1;
  delay(100);}
  
  //condicion para cambiar de estado cuando ambos elijieron a donde mover
  if (((bandera_bat_move_up == 1)|(bandera_bat_move_mid == 1)|(bandera_bat_move_down == 1)) and ((bandera_sup_move_up == 1)|(bandera_sup_move_mid == 1)|(bandera_sup_move_down == 1)))
  { delay(100);
    game_state = 3;}
//  FillRect(0, 200, 320, 100, BLACK);} //dibujar suelo
  break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 case 3: //pantalla del ganador en modo dia
 Serial3.write(2);
if (((bandera_bat_move_up == 1) and (bandera_sup_shoot_up)) or ((bandera_bat_move_down == 1) and (bandera_sup_shoot_down)) or ((bandera_bat_move_mid == 1) and (bandera_sup_shoot_mid)))
  { 
    if (laser_mostrado_day == 0){
    FillRect(65, 154, 220, 2, RED); //dibujar suelo 
    myFile = SD.open("wsup.txt"); //abrimos la imagen de ganador 
    mapeoSD();
    myFile.close(); 
    LCD_Print("GANA SUPERMAN", 90, 20, 2, BLACK, WHITE); 
    bandera_sup_gana = 1;
    laser_mostrado_day = 1;
  }
  }
  
//condicional para encontrar si gana Batman
if (((bandera_bat_shoot_up == 1) and (bandera_sup_move_up)) or ((bandera_bat_shoot_down == 1) and (bandera_sup_move_down)) or ((bandera_bat_shoot_mid == 1) and (bandera_sup_move_mid)))
  {
    if (boomerang_mostrado_day == 0) {
      
    for (int i = 220; i >= 0; i--) {
  //      posicion_x = i;
          int bat_index2 = (x_sup/5)%5; //indice para controlar la velocidad
      LCD_Sprite_Zoom(i, 148, 16, 20, batbo_day, 4, bat_index2, 0, 0, 0); //Mostrar boomerang
  //    V_line( posicion_x, 148, 51, 0xECEB); //Limpiar rastro
      }  
        lcdClear(0x000);
        myFile = SD.open("wbat.txt"); //abrimos la imagen de ganador 
        mapeoSD();
        myFile.close(); 
        LCD_Print("GANA BATMAN", 90, 20, 2, WHITE, BLACK);
        bandera_bat_gana = 1;
        boomerang_mostrado_day = 1;
    }  
   }
   
else if (((bandera_sup_gana == 1) and (bandera_bat_gana == 1)) or ((bandera_sup_gana == 0) and (bandera_bat_gana == 0))){
  lcdClear(0x000);
  LCD_Print("EMPATE", 160, 120, 2, WHITE, BLACK);
  }
  break;    
}  
}
//************************************************************************************************************************************************************************
//******************************************************************************funciones********************************************************************************* 
//************************************************************************************************************************************************************************

void mov_right_sup_day(){
  //superman hacia la derecha
  while ((digitalRead(der_sup) == LOW) && (x_sup < 320 - 34))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int sup_index = (x_sup/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_sup++, 151, 17, 24, sup_run_day, 3, sup_index, 1, 0,0); //Mostrar imagen
  V_line( x_sup - 1, 151, 51, 0xECEB); //Limpiar rastro
  }
}

void mov_left_sup_day(){
  //superaman hacia la izquierda 
  while ((digitalRead(iz_sup) == LOW) && (x_sup > 0))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int sup_index = (x_sup/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_sup--, 151, 17, 24, sup_run_day, 3, sup_index, 0, 0,0); //Mostrar imagen
  V_line( x_sup + 18, 151, 51, 0xECEB); //Limpiar rastro
  }
}

void mov_right_bat_day()
{
    //batman hacia la derecha 
  while ((digitalRead(der_bat) == LOW) && (x_bat < 320 - 46))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int bat_index = (x_bat/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_bat++, 148, 23, 26, bat_run, 3, bat_index, 1, 0, 0); //Mostrar imagen
  V_line( x_bat - 1, 148, 51, 0xECEB); //Limpiar rastro
  }
}

void mov_left_bat_day(){
  //batman hacia la izquierda 
  while ((digitalRead(iz_bat) == LOW) && (x_bat > 0))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int bat_index = (x_bat/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_bat--, 148, 23, 26, bat_run, 3, bat_index, 0, 0, 0); //Mostrar imagen
  V_line( x_bat + 23, 148, 51, 0xECEB); //Limpiar rastro
  }
}

/////////////////////////////////////////////////////////modo noche//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void mov_right_sup_night(){
  //superman hacia la derecha
  while ((digitalRead(der_sup) == LOW) && (x_sup < 320 - 34))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int sup_index = (x_sup/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_sup++, 151, 17, 24, sup_run_night, 3, sup_index, 1, 0,0); //Mostrar imagen
  V_line( x_sup - 1, 151, 51, 0x000); //Limpiar rastro
  }
}

void mov_left_sup_night(){
  //superaman hacia la izquierda 
  while ((digitalRead(iz_sup) == LOW) && (x_sup > 0))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int sup_index = (x_sup/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_sup--, 151, 17, 24, sup_run_night, 3, sup_index, 0, 0,0); //Mostrar imagen
  V_line( x_sup + 18, 151, 51, 0x000); //Limpiar rastro
  }
}

void mov_right_bat_night()
{
    //batman hacia la derecha 
  while ((digitalRead(der_bat) == LOW) && (x_bat < 320 - 46))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int bat_index = (x_bat/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_bat++, 148, 23, 26, bat_run_night, 3, bat_index, 1, 0, 0); //Mostrar imagen
  V_line( x_bat - 1, 148, 51, 0x000); //Limpiar rastro
  }
}

void mov_left_bat_night(){
  //batman hacia la izquierda 
  while ((digitalRead(iz_bat) == LOW) && (x_bat > 0))
  { //chequeamos las condiciones si se presiona el push adecuado y si no ha llegado al limite en x 
  int bat_index = (x_bat/5)%7; //indice para controlar la velocidad
  LCD_Sprite_Zoom(x_bat--, 148, 23, 26, bat_run_night, 3, bat_index, 0, 0, 0); //Mostrar imagen
  V_line( x_bat + 23, 148, 51, 0x000); //Limpiar rastro
  }
}



//***************************************************************************************************************************************
// Funciones SD-LCD
//***************************************************************************************************************************************
int asciitohex(int a){
  switch (a){
    case 48:
      return 0x00;
    case 49:
      return 0x01;
    case 50:
      return 0x02;
    case 51:
      return 0x03;
    case 52:
      return 0x04;
    case 53:
      return 0x05;
    case 54:
      return 0x06;
    case 55:
      return 0x07;
    case 56:
      return 0x08;
    case 57:
      return 0x09;
    case 97:
      return 0x0A;
    case 98:
      return 0x0B;
    case 99:
      return 0x0C;
    case 100:
      return 0x0D;
    case 101:
      return 0x0E;
    case 102:
      return 0x0F;
  }
}

void mapeoSD(){
  int hex1 = 0;
  int val1 = 0;
  int val2 = 0;
  int mapear = 0;
  int vertical = 0;
  unsigned char maps[640];

  if (myFile){
    while ((myFile.available()) && (vertical<238)){
      mapear = 0;
      while (mapear < 640){
        hex1 = myFile.read();
        if (hex1 == 120){
          val1 = myFile.read();
          val2 = myFile.read();
          val1 = asciitohex(val1);
          val2 = asciitohex(val2);
          maps[mapear] = val1*16 + val2;
          mapear++;
        }
      }
      
      LCD_Bitmap(0, vertical, 320, 1, maps);
      vertical++;
    }
    Serial.println("finish");
    myFile.close();
  }
  else {
    myFile.close();
    Serial.println("NO SE ABRIO");
  }
}
