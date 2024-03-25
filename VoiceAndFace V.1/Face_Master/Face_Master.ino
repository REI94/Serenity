
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

#define button 3
#define LED 4

// byte CODE;
int CODE;
Servo servo_3;
Servo servo_5;


void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(3, INPUT);
  pinMode(LED, OUTPUT);
  
  servo_3.attach(3, 500, 2500);
  servo_5.attach(5, 500, 2500);
  
  servo_5.write(0);
  servo_3.write(0);

  mylcd.Init_LCD();
  mylcd.Set_Rotation(1);
  mylcd.Fill_Screen(BLACK);
  
  eyes2();
  mouth();
  teeth1();

}

void loop() 
{
  //String CADENA="";
  //RECOPILA informacion "CODIGO" al esclavo 1

  Wire.requestFrom(8, 1);
  
  while(Wire.available())
  {
    Serial.println("ENTRADA");
    CODE = Wire.read();
    Serial.println(CODE);

  }

  delay(500);


    if(CODE == 1 || CODE == 2 || CODE == 3)
    {
      teeth2();
      //delay(10);
      NOteeth2();

 
      if (CODE == 2){
        movBrazosARRIBA();
      }
      else if(CODE == 3){
        movBrazosABAJO();
      }

    }
    else if(CODE == 0){
      teeth1();
      teeth2();
    }
    // else if(CODE == 2){
    //   movBrazosARRIBA();
    // }
    // else if(CODE == 3){
    //   movBrazosABAJO();
    // }

    
  // if(CODE == 1)
  // {
  //   blinkTeeth();
  // }
  
  // for(;;){
  //   task_d(LED);
  // }
  
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
    // delay(100);
    teeth2();
    delay(200);
    NOteeth2();

    delay(100);
  }while(true);

}

void blink(int led) //Función Blink
{
  // for(int i = 0; i <= 3; i++)
  // {
  do{
  	digitalWrite(led, HIGH);
 	  delay(500); 
  	digitalWrite(led, LOW);
  	delay(500); 
  
  }while(true);
}

void ledHigh(int led){
  digitalWrite(led, HIGH);
}

void ledLow(int led){
  digitalWrite(led, LOW);
}

void movBrazosARRIBA(){
  for(int i = 20; i <= 120; i++)
    {
    	servo_3.write(i);
      servo_5.write(i);
      delay(50);
    }
    	//delay(1000);
}

void movBrazosABAJO(){
      for(int i = 120; i >= 20; i--)
    {
      servo_3.write(i);
    	servo_5.write(i);
      delay(58);
    } 
}