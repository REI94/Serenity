#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include <EMIC2.h>

#include <SoftwareSerial.h>  // Needed by the EMIC2 library
#include <SD.h>  // Needed by the EMIC2 library, though not utilized in this example
#include "EMIC2.h"

#define RX_PIN 0  // Connect SOUT pin of the Emic 2 module to the RX pin
#define TX_PIN 1  // Connect SIN pin of the Emic 2 module to the TX pin

EMIC2 emic;  // Creates an instance of the EMIC2 library

//paramters define
#define MODEL ILI9341
#define CS   10    
#define CD   8
#define RST  9
#define MOSI  11
#define MISO  12
#define SCK   13
#define LED  A0   //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V

LCDWIKI_SPI mylcd(MODEL,CS,CD,MISO,MOSI,RST,SCK,LED); //model,cs,dc,miso,mosi,reset,sck,led

//define some colour values
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define button 3
#define LED 4


void setup() 
{

  pinMode(3, INPUT);
  pinMode(LED, OUTPUT);

  emic.begin(RX_PIN, TX_PIN);
  emic.setVoice(8);  // Sets the voice (9 choices: 0 - 8)
  emic.setVolume(18);
  emic.setLanguage(2);
  emic.setRate(270);

  mylcd.Init_LCD();
  mylcd.Set_Rotation(1);
  mylcd.Fill_Screen(BLACK);
  
  eyes2();
  mouth();
  teeth1();
  
}

void loop() 
{

  task_f(LED);


}


void voz(){

  emic.speak("He notado que su nivel cardiaco esta un poco alto");
  //delay(500);
  emic.speak("Permiitaaame ayudarle con unos ejercicios de respiiiraaaciooon");

  emic.speak("ejercicio 1 ");
  emic.speak("Respiraaaciooon cuatro siete ocho");

  emic.speak("ejercicio 2 ");
  emic.speak("respiraaaciooon abdominal");

  emic.speak("que ejercicio desea practicar?");
}


void respiracion478(){
  emic.speak("Le guiare a traves de la respiraaaciooon cuatro siete ocho");
  emic.speak("Pooongaaase en una posicion comoda");
  emic.speak("Puede estar sentado o acostado");
  emic.speak("Para comenzar, coloque una mano sobre el abdomen");
  emic.speak("y su otra mano sobre el pecho");
  emic.speak("Respire profunda y lentamente desde el abdomen");
  emic.speak("y cuente hasta cuatro a medida que inhala");

  emic.speak("1");
  emic.speak("2");
  emic.speak("3");
  emic.speak("4");

  emic.speak("contenga la respiraaaciooon y cuente en silencio del uno al siete");

  emic.speak("1");
  emic.speak("2");
  emic.speak("3");
  emic.speak("4");
  emic.speak("5");
  emic.speak("6");
  emic.speak("7");

  emic.speak("Exhale por completo a medida que cuenta en silencio del uno al ocho");
  emic.speak("Intente sacar todo el aire de los pulmones para cuando llegue a ocho");

  emic.speak("1");
  emic.speak("2");
  emic.speak("3");
  emic.speak("4");
  emic.speak("5");
  emic.speak("6");
  emic.speak("7");
  emic.speak("8");

  emic.speak("repita este ejercicio entre tres y siete veces");
  emic.speak("hasta que se sienta tranquilo");

  emic.speak("se siente usted mas tranquilo?");
  emic.speak("desea repetir el ejercicio?");
}

void respiracionAbdominal(){
  
  emic.speak("Le guiare a traves de la respiraaaciooon abdominal");
  emic.speak("Pooongaaase en una posicion comoda");
  emic.speak("Puede estar sentado o acostado");

  emic.speak("Coloque una mano sobre el abdomen justo debajo de las costillaaas y la otra mano sobre el peeechooo");
  emic.speak("Respire hondo por la nariz y" );
  emic.speak("deeeje que el abdomen le empuje la mano. El peechooo no deberaaa moverse.");
  emic.speak("Exhale a traves de los labiooos fruuunciiidos como si estuviese silbando");
  emic.speak("Sienta como se huuunde la mano sobre su abdomen y utiliiicelaaa para expulsar todo el aire hacia afuera.");
  emic.speak("Realice este ejercicio de respiraciiiooon entre 3 y 10 veces. Tooomeeeseee su tiempo con cada respiraciiiooon.");
  emic.speak("Note como se siente al final del ejercicio");

  emic.speak("se siente usted mas tranquilo?");
  emic.speak("desea repetir el ejercicio?");
}


void eyes2(){
  
  mylcd.Set_Draw_color(CYAN);

  int x3 = 80;
  int y3 = 70;
  int r = 35;
  
  int x4 = 230;
  int y4 = 70;
  int r2 = 35;
  
  int x5 = 80;
  int y5 = 70;
  int r3 = 20;

  int x6 = 230;
  int y6 = 70;
  int r4 = 20;

  mylcd.Fill_Circle(x3, y3, r);
  mylcd.Fill_Circle(x4, y4, r2);

  mylcd.Set_Draw_color(BLUE);
  mylcd.Fill_Circle(x5, y5, r3);
  mylcd.Fill_Circle(x6, y6, r4);
}


void mouth(){
  //background
  int x = 75;
  int y = 150;
  int w = 145;
  int h = 60;

  mylcd.Fill_Rect(x, y, w, h,  CYAN);
}

void teeth1(){
  int x2 = 80;
  int y2 = 155;
  int w2 = 15;
  int h2 = 50;

  int x4 = 120;
  int y4 = 155;
  int w4 = 15;
  int h4 = 50;

  int x6 = 160;
  int y6 = 155;
  int w6 = 15;
  int h6 = 50;
  
  int x8 = 200;
  int y8 = 155;
  int w8 = 15;
  int h8 = 50;
  

  mylcd.Fill_Rect(x2, y2, w2, h2,  BLUE);
  mylcd.Fill_Rect(x4, y4, w4, h4,  BLUE);
  mylcd.Fill_Rect(x6, y6, w6, h6,  BLUE);
  mylcd.Fill_Rect(x8, y8, w8, h8,  BLUE);
}

void teeth2(){
  
  int x3 = 100;
  int y3 = 155;
  int w3 = 15;
  int h3 = 50;

  int x5 = 140;
  int y5 = 155;
  int w5 = 15;
  int h5 = 50;

  int x7 = 180;
  int y7 = 155;
  int w7 = 15;
  int h7 = 50;
  
  mylcd.Fill_Rect(x3, y3, w3, h3,  BLUE);
  mylcd.Fill_Rect(x5, y5, w5, h5,  BLUE);
  mylcd.Fill_Rect(x7, y7, w7, h7,  BLUE);
}

void NOteeth2(){
  
  int x3 = 100;
  int y3 = 155;
  int w3 = 15;
  int h3 = 50;

  int x5 = 140;
  int y5 = 155;
  int w5 = 15;
  int h5 = 50;

  int x7 = 180;
  int y7 = 155;
  int w7 = 15;
  int h7 = 50;
  
  mylcd.Fill_Rect(x3, y3, w3, h3,  CYAN);
  mylcd.Fill_Rect(x5, y5, w5, h5,  CYAN);
  mylcd.Fill_Rect(x7, y7, w7, h7,  CYAN);
}

void blinkTeeth(){
  
  do{
    delay(100);
    teeth2();
    delay(500);
    NOteeth2();
  }while(true);

}


void ledHigh(int led){
  digitalWrite(led, HIGH);
}

void ledLow(int led){
  digitalWrite(led, LOW);
}

void task_f(int led){
  static int state_f;
  if(state_f == 1 || state_f == 2 || state_f == 3){ 
    for(;;){
      switch(state_f){
        case 1: 
          teeth2();
          emic.speak("Leguiare"); 
          NOteeth2();
          state_f = 2;
          break;
        case 2:
          teeth2();
          emic.speak("atraves"); 
          NOteeth2();
          state_f = 3;
          break;
        case 3: 
          teeth2();
          emic.speak("dela");
          NOteeth2();
          state_f = 4;
          break;
        case 4:
          teeth2();
          emic.speak("respiraaacioooncuatro");
          NOteeth2();
          state_f = 5;
          break;
        case 5: 
          teeth2();
          emic.speak("sieteocho");
          NOteeth2();
          state_f = 6;
          break;
        case 6:
          teeth2();
          emic.speak("Pooongaaaseen");
          NOteeth2();
          state_f = 7;
          break;
        case 7:
          teeth2();
          emic.speak("unaposicioncomoda");
          NOteeth2();
          state_f = 8;
          break;
        case 8:
          teeth2();
          emic.speak("Puedeestar"); 
          NOteeth2();
          state_f = 9;
          break;
        case 9: 
          teeth2();
	        emic.speak("sentadooacostado");
	        NOteeth2();
          state_f = 10;
          break;
        case 10:
	        teeth2();
          emic.speak("Paracomenzar"); 
          NOteeth2();
	        state_f = 11;
          break;
        case 11: 
	        teeth2();
	        emic.speak("coloqueuna");
          NOteeth2();
          state_f = 12;
          break;
        case 12:
	        teeth2();
          emic.speak("manosobre");
	        NOteeth2();
          state_f = 13;
          break;
        case 13: 
          teeth2();
  	      emic.speak("elabdomen");
	        NOteeth2();
          state_f = 14;
          break;
        case 14:
	        teeth2();
          emic.speak("ysu");
          NOteeth2();
	        state_f = 15;
          break;
        case 15:
	        teeth2(); 
	        emic.speak("otramano");
          NOteeth2();
          state_f = 16;
          break;
        case 16:
	        teeth2();
          emic.speak("sobreelpecho");
          NOteeth2();
	        state_f = 17;
          break;
	      case 17: 
          teeth2();
          emic.speak("Respireprofunda"); 
          NOteeth2();
          state_f = 18;
          break;
        case 18:
          teeth2();
          emic.speak("ylentamente");
          NOteeth2();
          state_f = 19;
          break;
        case 19: 
          teeth2();
          emic.speak("desdeelabdomen");
          NOteeth2();
          state_f = 20;
          break;
        case 20:
          teeth2();
          emic.speak("ycuente");
          NOteeth2();
          state_f = 21;
          break;
        case 21: 
          teeth2();
          emic.speak("hastacuatro");
          NOteeth2();
          state_f = 22;
          break;
        case 22:
          teeth2();
          emic.speak("amedida"); 
          NOteeth2();
          state_f = 23;
          break;
        case 23:
          teeth2();
          emic.speak("queinhala");
          NOteeth2();
          state_f = 24;
          break;
        case 24:
          teeth2();
          emic.speak("1"); 
          NOteeth2();
          state_f = 25;
          break;
        case 25: 
          teeth2();
	        emic.speak("2");
	        NOteeth2();
          state_f = 26;
          break;
        case 26:
	        teeth2();
          emic.speak("3"); 
          NOteeth2();
	        state_f = 27;
          break;
        case 27: 
	        teeth2();
	        emic.speak("4");
          NOteeth2();
          state_f = 28;
          break;
        case 28:
	        teeth2();
          emic.speak("contengala");
	        NOteeth2();
          state_f = 29;
          break;
        case 29: 
          teeth2();
  	      emic.speak("respiraaaciooonycuente");
	        NOteeth2();
          state_f = 30;
          break;
        case 30:
	        teeth2();
          emic.speak("ensilencio");
          NOteeth2();
	        state_f = 31;
          break;
        case 31:
	        teeth2(); 
	        emic.speak("deluno"); 
          NOteeth2();
          state_f = 32;
          break;
        case 32:
	        teeth2();
          emic.speak("alsiete");
          NOteeth2();
	        state_f = 33;
          break;
	      case 33: 
          teeth2();
          emic.speak("1");
          NOteeth2();
          state_f = 34;
          break;
        case 34:
          teeth2();
          emic.speak("2");
          NOteeth2();
          state_f = 35;
          break;
        case 35: 
          teeth2();
          emic.speak("3");
          NOteeth2();
          state_f = 36;
          break;
        case 36:
          teeth2();
          emic.speak("4");
          NOteeth2();
          state_f = 37;
          break;
        case 37: 
          teeth2();
          emic.speak("5");
          NOteeth2();
          state_f = 38;
          break;
        case 38:
          teeth2();
          emic.speak("6");
          NOteeth2();
          state_f = 39;
          break;
        case 39:
          teeth2();
          emic.speak("7");
          NOteeth2();
          state_f = 40;
          break;
        case 40:
          teeth2();
          emic.speak("Exhalepor");
          NOteeth2();
          state_f = 41;
          break;
        case 41: 
          teeth2();
	        emic.speak("completoa");
	        NOteeth2();
          state_f = 42;
          break;
        case 42:
	        teeth2();
          emic.speak("medidaque");
          NOteeth2();
	        state_f = 43;
          break;
        case 43: 
	        teeth2();
	        emic.speak("cuentaen");
          NOteeth2();
          state_f = 44;
          break;
        case 44:
	        teeth2();
          emic.speak("silenciodel");
	        NOteeth2();
          state_f = 45;
          break;
        case 45: 
          teeth2();
  	      emic.speak("unoalocho");
	        NOteeth2();
          state_f = 46;
          break;
        case 46:
	        teeth2();
          emic.speak("Intentesacar");
          NOteeth2();
	        state_f = 47;
          break;
        case 47:
	        teeth2(); 
	        emic.speak("todoel");
          NOteeth2();
          state_f = 48;
          break;
        case 48:
	        teeth2();
          emic.speak("airede");
          NOteeth2();
	        state_f = 49;
          break;
	      case 49: 
          teeth2();
          emic.speak("lospulmones"); 
          NOteeth2();
          state_f = 50;
          break;
        case 50:
          teeth2();
          emic.speak("paracuando");
          NOteeth2();
          state_f = 51;
          break;
        case 51: 
          teeth2();
          emic.speak("llegueaocho");
          NOteeth2();
          state_f = 52;
          break;
        case 52:
          teeth2();
          emic.speak("1");
          NOteeth2();
          state_f = 53;
          break;
        case 53: 
          teeth2();
          emic.speak("2");
          NOteeth2();
          state_f = 54;
          break;
        case 54:
          teeth2();
          emic.speak("3"); 
          NOteeth2();
          state_f = 55;
          break;
        case 55:
          teeth2();
          emic.speak("4");
          NOteeth2();
          state_f = 56;
          break;
        case 56:
          teeth2();
         emic.speak("5"); 
          NOteeth2();
          state_f = 57;
          break;
        case 57: 
          teeth2();
          emic.speak("6");
          NOteeth2();
          state_f = 58;
          break;
        case 58:
	        teeth2();
          emic.speak("7");
          NOteeth2();
	        state_f = 59;
          break;
        case 59: 
          teeth2();
          emic.speak("8");
          NOteeth2();
          state_f = 60;
          break;
        case 60:
	        teeth2();
          emic.speak("sesiente");
	        NOteeth2();
          state_f = 61;
          break;
        case 61: 
          teeth2();
          emic.speak("ustedmas");
          NOteeth2();
          state_f = 62;
          break;
        case 62:
	        teeth2();
          emic.speak("tranquilo?");
          NOteeth2();
	        state_f = 63;
          break;
        case 63:
          teeth2(); 
          emic.speak("desearepetir"); 
          NOteeth2();
          state_f = 64;
          break;
        case 64:
	        teeth2();
          emic.speak("elejercicio?");
          NOteeth2();
	        state_f = 1;
          break;

      }
    }
  }else{
    state_f = 1;
  }
  
}
