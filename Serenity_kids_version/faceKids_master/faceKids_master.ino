#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library

#include "Wire.h"
#include <Servo.h>

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

int CODE;
Servo servo_3;
Servo servo_5;

void setup() {

  Wire.begin();
  Serial.begin(9600);

  mylcd.Init_LCD();
  mylcd.Set_Rotation(1);
  mylcd.Fill_Screen(BLACK);// put your setup code here, to run once:

  // pinMode(3, INPUT);
  // pinMode(LED, OUTPUT);
  
  servo_3.attach(3, 500, 2500);
  servo_5.attach(5, 500, 2500);
  
  servo_5.write(90);
  servo_3.write(10);

  eyes1();
  //mouth();
  // delay(1000);
  // mouthSad();
  // delay(1000);
  // Nomouth();
  mouthHap();
  //blinkmouth();
}

void loop() {

  //mouth();
  Wire.requestFrom(8, 1);
  
  while(Wire.available())
  {
    Serial.println("ENTRADA");
    CODE = Wire.read();
    Serial.println(CODE);

  }

if (CODE == 0 || CODE == 1 || CODE == 2 || CODE == 3 || CODE == 4 || CODE == 5 || CODE == 6 || CODE == 7 || CODE == 8 || CODE == 9 || CODE == 10){

    mouth();
    delay(10);
    mouth3();
    delay(10);
    Nomouth2();

  if(CODE == 1){

    //Nomouth();
    mouthSad();
    delay(1000);
    Nomouth();

    
  }
  else if(CODE == 2){
  
    mouthHap();
    delay(1000);
  
  }

  else if (CODE == 3){
    mouthHap();
    brazoDerU();
    delay(2700);
    brazoDerD();
  
  }

  else if (CODE == 4){
    mouthHap();
    brazoIzqU();
    delay(2000);
    brazoIzqD();
  }

  else if(CODE == 5){
    mouthHap();
    brazosBaile();
  }

  else if (CODE == 6){
    mouthHap();
    brazos2();
  }

  else if (CODE == 7){
    mouthHap();
    delay(2000);
    brazoDerU2();

  }

  else if (CODE == 8){
    mouthHap();
    delay(2000);
    brazoIzqU2();
  }

  else if (CODE == 9){
    mouthHap();
    delay(2000);
    brazoDerD();

  }
  else if (CODE == 10){
    mouthHap();
    delay(3000);
    brazoIzqD();
  }
  // //brazos2();
  // brazoDerU();
  // delay(1000);
  // brazoDerD();
  // delay(1000);
  // brazoIzqU();
  // delay(1000);
  // brazoIzqD();
  // delay(1000);
}
}

void eyes1(){

  mylcd.Set_Draw_color(MAGENTA);

  int x = 70;
  int y = 60;
  int x2 = 150;
  int y2 = 140;
  int r = 7;

  int x3 = 200;
  int y3 = 60;
  int x4 = 280;
  int y4 = 140;
  int r2 = 7;


  mylcd.Draw_Round_Rectangle(x, y, x2, y2, r);
  mylcd.Fill_Round_Rectangle(x, y, x2, y2, r);

  
  mylcd.Draw_Round_Rectangle(x3, y3, x4, y4, r2);
  mylcd.Fill_Round_Rectangle(x3, y3, x4, y4, r2);

  mylcd.Set_Draw_color(BLACK);

  int x5 = 110;
  int y5 = 120;
  int r5 = 30;
  
  int x6 = 240;
  int y6 = 120;
  int r6 = 30;
  
  mylcd.Fill_Circle(x5, y5, r5);
  mylcd.Fill_Circle(x6, y6, r6);


}

void Nomouth(){

  mylcd.Set_Draw_color(BLACK);

  int x1 = 140;
  int y1 = 180;
  int x2 = 210;
  int y2 = 182;
  
  int x3 = 140;
  int y3 = 190;
  int x4 = 142;
  int y4 = 182;

  int x5 = 208;
  int y5 = 190;
  int x6 = 210;
  int y6 = 182;

  int x7 = 140;
  int y7 = 170;
  int x8 = 142;
  int y8 = 182;

  int x9 = 208;
  int y9 = 170;
  int x10 = 210;
  int y10 = 182;

  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  mylcd.Fill_Rectangle(x3, y3, x4, y4);
  mylcd.Fill_Rectangle(x5, y5, x6, y6);
  mylcd.Fill_Rectangle(x7, y7, x8, y8);
  mylcd.Fill_Rectangle(x9, y9, x10, y10);

}

void mouth(){

  mylcd.Set_Draw_color(MAGENTA);


  int x1 = 140;
  int y1 = 180;
  int x2 = 210;
  int y2 = 182;
  
  int x3 = 140;
  int y3 = 170;
  int x4 = 142;
  int y4 = 182;

  int x5 = 208;
  int y5 = 170;
  int x6 = 210;
  int y6 = 182;

  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  // mylcd.Fill_Rectangle(x3, y3, x4, y4);
  // mylcd.Fill_Rectangle(x5, y5, x6, y6);

}


void mouth3(){

  mylcd.Set_Draw_color(MAGENTA);


  int x1 = 140;
  int y1 = 160;
  int x2 = 210;
  int y2 = 182;
  
  int x3 = 140;
  int y3 = 170;
  int x4 = 142;
  int y4 = 182;

  int x5 = 208;
  int y5 = 170;
  int x6 = 210;
  int y6 = 182;

  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  // mylcd.Fill_Rectangle(x3, y3, x4, y4);
  // mylcd.Fill_Rectangle(x5, y5, x6, y6);

}

void Nomouth2(){

  mylcd.Set_Draw_color(BLACK);


  int x1 = 140;
  int y1 = 160;
  int x2 = 210;
  int y2 = 182;
  
  int x3 = 140;
  int y3 = 170;
  int x4 = 142;
  int y4 = 182;

  int x5 = 208;
  int y5 = 170;
  int x6 = 210;
  int y6 = 182;

  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  // mylcd.Fill_Rectangle(x3, y3, x4, y4);
  // mylcd.Fill_Rectangle(x5, y5, x6, y6);

}



void blinkmouth(){
  do{
    
    mouthHap();
    delay(300);
    mouth3();
    delay(100);
    Nomouth2();
  }while(true);
}

void mouth2(){

  mylcd.Set_Draw_color(CYAN);


  int x1 = 140;
  int y1 = 170;
  int x2 = 150;
  int y2 = 160;
  
  int x3 = 160;
  int y3 = 170;
  int x4 = 170;
  int y4 = 160;

  int x5 = 180;
  int y5 = 170;
  int x6 = 190;
  int y6 = 160;

  int x7 = 200;
  int y7 = 170;
  int x8 = 210;
  int y8 = 160;

  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  mylcd.Fill_Rectangle(x3, y3, x4, y4);
  mylcd.Fill_Rectangle(x5, y5, x6, y6);
  mylcd.Fill_Rectangle(x7, y7, x8, y8);

}

void mouthHap(){

  mylcd.Set_Draw_color(MAGENTA);


  int x1 = 140;
  int y1 = 180;
  int x2 = 210;
  int y2 = 182;
  
  int x3 = 140;
  int y3 = 170;
  int x4 = 142;
  int y4 = 182;

  int x5 = 208;
  int y5 = 170;
  int x6 = 210;
  int y6 = 182;

  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  mylcd.Fill_Rectangle(x3, y3, x4, y4);
  mylcd.Fill_Rectangle(x5, y5, x6, y6);

}

void mouthSad(){

  mylcd.Set_Draw_color(MAGENTA);


  int x1 = 140;
  int y1 = 180;
  int x2 = 210;
  int y2 = 182;
  
  int x3 = 140;
  int y3 = 190;
  int x4 = 142;
  int y4 = 182;

  int x5 = 208;
  int y5 = 190;
  int x6 = 210;
  int y6 = 182;

  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  mylcd.Fill_Rectangle(x3, y3, x4, y4);
  mylcd.Fill_Rectangle(x5, y5, x6, y6);

}

void brazos2(){

  for (int angle = 0; angle <= 120; angle++) {
    servo_3.write(angle);
    servo_5.write(120 - angle);
    delay(30);  // Retardo para dar tiempo a los servos a moverse
  }

  delay(8000);
    // Volver a 0 grados
  for (int angle = 120; angle >= 0; angle--) {
    servo_3.write(angle);
    servo_5.write(120 - angle);
    delay(30);  // Retardo para dar tiempo a los servos a moverse
  }
}

void brazosBaile(){

  for (int angle = 0; angle <= 80; angle++) {
    servo_3.write(angle);
    servo_5.write(80 - angle);
    delay(20);  // Retardo para dar tiempo a los servos a moverse
  }
    // Volver a 0 grados
  for (int angle = 80; angle >= 0; angle--) {
    servo_3.write(angle);
    servo_5.write(80 - angle);
    delay(20);  // Retardo para dar tiempo a los servos a moverse
  }
}


void brazoDerU(){
  
  for (int i = 0; i <= 120; i++){
    servo_3.write(i);
    delay(30);
  }
  

}


void brazoDerU2(){
  
  for (int i = 0; i <= 120; i++){
    servo_3.write(i);
    delay(30);
  }
  
  delay(4000);
}

void brazoDerD(){
  
  for (int i = 120; i >= 0; i--){
    servo_3.write(i);
    delay(30);
  }

}

void brazoIzqD(){
  
  for (int i = 0; i <= 120; i++){
    servo_5.write(i);
    delay(30);
  }

}
void brazoIzqU(){
  
  for (int i = 120; i >= 0; i--){
    servo_5.write(i);
    delay(30);
  }

}

void brazoIzqU2(){
  
  for (int i = 120; i >= 0; i--){
    servo_5.write(i);
    delay(30);
  }

  delay(2000);

}