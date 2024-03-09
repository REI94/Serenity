
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library

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
  Serial.begin(9600);
  pinMode(3, INPUT);
  pinMode(LED, OUTPUT);

  mylcd.Init_LCD();
  mylcd.Set_Rotation(1);
  mylcd.Fill_Screen(BLACK);
  
  eyes2();
  mouth();
  teeth1();

}

void loop() 
{
  for(;;){
    task_d(LED);
  }
  
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
    delay(300);
    NOteeth2();
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

void task_a(){
  static int state_a;
  Serial.print("state a: ");
  Serial.println(state_a);
  if(state_a == 1 || state_a == 2 || state_a == 3){ 
    for(;;){
      switch(state_a){
        case 1: 
          teeth2();
          state_a = 2;
          Serial.print("state a: ");
          Serial.println(state_a);
          break;
        case 2: 
          NOteeth2();
          state_a = 3;
          Serial.print("state a: ");
          Serial.println(state_a);
          break;
        case 3: 
          teeth2();
          state_a = 1;
          Serial.print("state a: ");
          Serial.println(state_a);
          break;
      }
    }
  }else{
    state_a = 1;
  }
  
}

void task_b(int led){
  static int state_b;
  Serial.print("state b: ");
  Serial.println(state_b);
  if(state_b == 1 || state_b == 2 || state_b == 3){
      for(;;){
        switch(state_b){
        case 1: 
          ledHigh(led);
          state_b = 2;
          Serial.print("state b: ");
          Serial.println(state_b);
          break;
        case 2: 
          ledLow(led);
          state_b = 3;
          Serial.print("state b: ");
          Serial.println(state_b);
          break;
        case 3: 
          ledHigh(led);
          state_b = 1; 
          Serial.print("state b: ");
          Serial.println(state_b);
          break;
        }
      }
  }else{
    state_b = 1;
  }
}

void task_c(int led){
  static int state_c;
  Serial.print("state c: ");
  Serial.println(state_c);
  if(state_c == 1 || state_c == 2 || state_c == 3){ 
    for(;;){
      switch(state_c){
        case 1: 
          teeth2();
          ledHigh(led);
          state_c = 2;
          Serial.print("state c: ");
          Serial.println(state_c);
          break;
        case 2: 
          NOteeth2();
          ledLow(led);
          state_c = 3;
          Serial.print("state c: ");
          Serial.println(state_c);
          break;
        case 3: 
          teeth2();
          ledHigh(led);
          state_c = 1;
          Serial.print("state c: ");
          Serial.println(state_c);
          break;
      }
    }
  }else{
    state_c = 1;
  }
  
}

void task_d(int led){
  static int state_d;
  Serial.print("state d: ");
  Serial.println(state_d);
  if(state_d == 1 || state_d == 2 || state_d == 3){ 
    for(;;){
      switch(state_d){
        case 1: 
          teeth2();
          state_d = 2;
          Serial.print("state d: ");
          Serial.println(state_d);
          break;
        case 2:
          ledHigh(led);
          state_d = 3;
          Serial.print("state d: ");
          Serial.println(state_d);
          break;
        case 3: 
          NOteeth2();
          state_d = 4;
          Serial.print("state d: ");
          Serial.println(state_d);
          break;
        case 4:
          ledLow(led);
          state_d = 1;
          Serial.print("state d: ");
          Serial.println(state_d);
          break;
      }
    }
  }else{
    state_d = 1;
  }
  
}

void total_task(int led){
  static int total_state;

  Serial.print("total_state: " );
  Serial.println(total_state);
  if(total_state == 1 || total_state == 2 || total_state == 3){

    for(;;){
      switch(total_state){
        case 1: 
          task_a();
          total_state = 2;
          Serial.print("total_state: " );
          Serial.println(total_state);
          break;
        case 2:
          task_b(LED);
          total_state = 3;
          Serial.print("total_state: " );
          Serial.println(total_state);
          break;
        case 3:
          task_a();
          total_state = 1;
          Serial.print("total_state: " );
          Serial.println(total_state);
          break;
      }
    }

  }else{
    total_state = 1;
  }
}
