#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include "Wire.h"
#include <Servo.h>

// Definiciones de pantalla
#define MODEL ILI9341
#define CS   10    
#define CD   8
#define RST  9
#define MOSI  11
#define MISO  12
#define SCK   13
#define LED  A0

LCDWIKI_SPI mylcd(MODEL,CS,CD,MISO,MOSI,RST,SCK,LED); 
 

// Colores
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int CODE = 0;
int previousCODE = -1; // Para detectar cambios de estado
Servo servo_3;
Servo servo_5;
void(* resetFunc) (void) = 0; // Función de reset

void setup() {
  Wire.begin();
  Serial.begin(9600);

  mylcd.Init_LCD();
  mylcd.Set_Rotation(1);
  mylcd.Fill_Screen(BLACK);

  // Inicializar servos en posición neutral
  servo_3.attach(3, 500, 2500);
  servo_5.attach(5, 500, 2500);
  servo_5.write(90);
  servo_3.write(10); // Ajustado según código original

  eyes1(); // Ojos base
  mouthHap(); // Boca base
}

void loop() {
  Wire.requestFrom(8, 1);
  
  if (Wire.available()) {
    CODE = Wire.read();
    Serial.print("Recibido: ");
    Serial.println(CODE);
  }

  // Solo actuamos si el código HA CAMBIADO
  if (CODE != previousCODE) {
    previousCODE = CODE; // Actualizamos para no repetir inmediatamente
    
    // Limpiar boca anterior (opcional, dependiendo de si las funciones limpian)
    // Pero como todas dibujan sobre la misma area, a veces es bueno resetear
    // Nomouth(); 

    // Ejecutar acción basada en el código
    switch (CODE) {
      case 0: // Estado NEUTRO / IDLE
        mouthHap();
        break;

      case 1: // INCORRECTO / TRISTE
        mouthSad();
        delay(1000); 
        Nomouth();
        break;

      case 2: // CORRECTO / FELIZ (Simple + Brazos)
        mouthHap();
        brazosBaile(); // Animación nueva de brazos
        // delay(1000); // Ya incluido en celebracionCorta
        break;

      case 3: // Celebración 1 (Brazo Der Arriba)
        mouthHap();
        brazoDerU();
        delay(1000); // Reduje un poco el delay original de 2700 para fluidez
        brazoDerD();
        break;

      case 4: // Celebración 2 (Brazo Izq Arriba)
        mouthHap();
        brazoIzqU();
        delay(1000);
        brazoIzqD();
        break;

      case 5: // BAILE
        mouthHap();
        brazosBaile();
        break;

      case 6: // Brazos 2
        mouthHap();
        brazos2();
        break;

      case 7: // Brazo Der U 2 (Lento?)
        mouthHap();
        brazoDerU2();
        // Nota: la funcion original tiene un delay interno
        break;
      
      // Agrega más casos si es necesario para códigos 8, 9, 10...
      case 8:
         mouthHap();
         brazoIzqU2();
         break;
      
      case 9:
         mouthHap();
         brazoDerD();
         break;
         
      case 10:
         mouthHap();
         brazoIzqD();
         break;


      case 20: // MODO HABLAR
         talkLoop(); // Llamada a función externa
         break;

      case 99: // RESET REMOTO
         Serial.println("RESET COMANDADO");
         delay(100);
         resetFunc();
         break;

      default:
        // Si llega algo desconocido, quizas volver a neutro
        // mouthHap();
        break;
    }
  }
  
  // Pequeña pausa para no saturar el bus I2C
  delay(100);
}

// ==========================================
// FUNCIONES DE ANIMACION
// ==========================================

void talkLoop() {
  while (true) {
     // Frame 1: Boca Abierta
     Nomouth();
     mouth3(); // Rectangulo (Boca abierta)
     
     // Poll I2C
     Wire.requestFrom(8, 1);
     if (Wire.available()) {
        int c = Wire.read();
        if (c != 20) { 
          CODE = c; 
          // LIMPIEZA DE SALIDA: Asegurar boca cerrada antes de salir
          Nomouth();
          mouthHap();
          return; 
        } 
     }
     delay(10); // Mas rapido (antes 150)

     // Frame 2: Boca Cerrada (Feliz)
     Nomouth();
     mouthHap(); 
     
     // Poll I2C
     Wire.requestFrom(8, 1);
     if (Wire.available()) {
        int c = Wire.read();
        if (c != 20) { 
          CODE = c; 
          // LIMPIEZA DE SALIDA
          Nomouth();
          mouthHap();
          return; 
        }
     }
     delay(10); // Mas rapido
  }
}


// ==========================================
// FUNCIONES DE DIBUJO Y SERVOS (Copiadas y Limpiadas)
// ==========================================

void eyes1(){
  mylcd.Set_Draw_color(MAGENTA);
  // Ojos rectangulares
  mylcd.Fill_Round_Rectangle(70, 60, 150, 140, 7); // Izq
  mylcd.Fill_Round_Rectangle(200, 60, 280, 140, 7); // Der

  mylcd.Set_Draw_color(BLACK);
  // Pupilas
  mylcd.Fill_Circle(110, 120, 30);
  mylcd.Fill_Circle(240, 120, 30);
}

void Nomouth(){
  mylcd.Set_Draw_color(BLACK);
  // Borra zona de la boca (ajustar coordenadas si es necesario para cubrir todo)
  mylcd.Fill_Rectangle(140, 160, 210, 200); 
}

void Nomouth2(){
   // Version especifica del codigo original
   mylcd.Set_Draw_color(BLACK);
   mylcd.Fill_Rectangle(140, 160, 210, 182);
}

void mouth(){
  // Boca normal
  mylcd.Set_Draw_color(MAGENTA);
  mylcd.Fill_Rectangle(140, 180, 210, 182);
}

void mouth3(){
  // Boca var 3
  mylcd.Set_Draw_color(MAGENTA);
  mylcd.Fill_Rectangle(140, 160, 210, 182);
}

void mouthHap(){
  // Boca Feliz
  mylcd.Set_Draw_color(MAGENTA);
  int x1 = 140; int y1 = 180; int x2 = 210; int y2 = 182;
  int x3 = 140; int y3 = 170; int x4 = 142; int y4 = 182;
  int x5 = 208; int y5 = 170; int x6 = 210; int y6 = 182;
  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  mylcd.Fill_Rectangle(x3, y3, x4, y4);
  mylcd.Fill_Rectangle(x5, y5, x6, y6);
}

void mouthSad(){
  // Boca Triste (invertida)
  mylcd.Set_Draw_color(MAGENTA);
  int x1 = 140; int y1 = 180; int x2 = 210; int y2 = 182;
  int x3 = 140; int y3 = 190; int x4 = 142; int y4 = 182;
  int x5 = 208; int y5 = 190; int x6 = 210; int y6 = 182;
  mylcd.Fill_Rectangle(x1, y1, x2, y2);
  mylcd.Fill_Rectangle(x3, y3, x4, y4);
  mylcd.Fill_Rectangle(x5, y5, x6, y6);
}

// --- SERVOS ---
void brazos2(){
  for (int angle = 0; angle <= 120; angle+=2) { // Incremento modificado para velocidad
    servo_3.write(angle);
    servo_5.write(120 - angle);
    delay(15);
  }
  delay(1000); // Pausa corta
  for (int angle = 120; angle >= 0; angle-=2) {
    servo_3.write(angle);
    servo_5.write(120 - angle);
    delay(15);
  }
}

void brazosBaile(){
  for(int k=0; k<2; k++){ // Repetir baile 2 veces
    for (int angle = 0; angle <= 80; angle+=4) {
      servo_3.write(angle);
      servo_5.write(80 - angle);
      delay(10);
    }
    for (int angle = 80; angle >= 0; angle-=4) {
      servo_3.write(angle);
      servo_5.write(80 - angle);
      delay(10);
    }
  }
}

void brazoDerU(){
  for (int i = 0; i <= 120; i+=2){
    servo_3.write(i);
    delay(15);
  }
}

void brazoDerU2(){
  for (int i = 0; i <= 120; i+=2){
    servo_3.write(i);
    delay(15);
  }
  delay(1000);
}

void brazoDerD(){
  for (int i = 120; i >= 0; i-=2){
    servo_3.write(i);
    delay(15);
  }
}

void brazoIzqD(){
  for (int i = 0; i <= 120; i+=2){
    servo_5.write(i);
    delay(15);
  }
}

void brazoIzqU(){
  for (int i = 120; i >= 0; i-=2){
    servo_5.write(i);
    delay(15);
  }
}

void brazoIzqU2(){
  for (int i = 120; i >= 0; i-=2){
    servo_5.write(i);
    delay(15);
  }
  delay(1000);
}
