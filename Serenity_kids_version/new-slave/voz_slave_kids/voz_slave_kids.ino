#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Wire.h>

// ==========================================
// CONFIGURACION KEYPAD
// ==========================================
const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// PINES CORREGIDOS POR EL USUARIO:
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {13, 12, 11, 10}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// ==========================================
// CONFIGURACION AUDIO (DFPlayer)
// ==========================================
SoftwareSerial mySerial(2, 4);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// CONSTANTES AUDIO (Mapeo desde CSV)
// Carpeta 01: Info
const int F_INFO = 1;
const int A_INTRO_1 = 1; // Hola me llamo Serenity...
const int A_INTRO_2 = 2; // Yo te daré un ejercicio...
const int A_INTRO_3 = 3; // Recuerda, si tienes alguna duda...

// Carpeta 02: Ejercicios
const int F_EJERCICIOS = 2;
const int A_Q1 = 1; // 3 + 2 = 5
const int A_Q2 = 2; // 5 + 3 = 8
const int A_Q3 = 3; // 4 + 5 = 9
const int A_Q4 = 4; // 6 + 1 = 7

// Carpeta 03: Respuestas
const int F_RESPUESTAS = 3;
const int A_CORRECTA_IS_5 = 1;
const int A_CORRECTA_IS_8 = 2;
const int A_CORRECTA_IS_9 = 3;
const int A_CORRECTA_IS_7 = 4;
const int A_INCORRECTO_FX = 5; // Sonido error
const int A_EXCELENTE     = 6; // Excelente...
const int A_EXITO_FX      = 7; // Sonido exito
const int A_MUY_BIEN      = 8; // Muy buen trabajo...
const int A_SIGAMOS       = 9; // Excelente...

// Carpeta 04: Despedida
const int F_FINAL = 4;
const int A_DESPEDIDA = 1;

// VARIABLES RESPUESTAS ESPERADAS
const char RESP_Q1 = '5';
const char RESP_Q2 = '8';
const char RESP_Q3 = '9';
const char RESP_Q4 = '7';

// ==========================================
// ESTADOS DEL ROBOT
// ==========================================
enum RobotState {
  STATE_INIT,
  STATE_INTRO_1,
  STATE_INTRO_2,
  STATE_INTRO_3,
  STATE_WAIT_START,    // Esperando '#'
  
  STATE_PLAY_Q1,
  STATE_WAIT_A1,
  
  STATE_PLAY_Q2,
  STATE_WAIT_A2,
  
  STATE_PLAY_Q3,
  STATE_WAIT_A3,
  
  STATE_PLAY_Q4,
  STATE_WAIT_A4,
  
  STATE_FINISH
};

RobotState currentState = STATE_INIT;
int codigoParaMaster = 0; 
char inputAnswer = 0;
unsigned long lastStateTime = 0; // Para controlar tiempos sin delay bloqueante si fuera necesario (usaremos delay simple por ahora)

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  
  Wire.begin(8);  
  Wire.onRequest(requestEvent);
  
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Error al iniciar DFPlayer");
  }
  myDFPlayer.volume(25);
  
  currentState = STATE_INIT;
}

void loop() {
  char key = customKeypad.getKey();
  
  if (key) {
    Serial.print("DEBUG: Tecla: ");
    Serial.println(key);
  }
  
  switch (currentState) {
    case STATE_INIT:
      Serial.println("Estado: INTRO 1");
      myDFPlayer.playFolder(F_INFO, A_INTRO_1); 
      // Duraciones aproximadas + margen. Lo ideal seria detectar busy pin, pero usaremos delays seguros.
      delay(6000); 
      currentState = STATE_INTRO_2;
      break;
      
    case STATE_INTRO_2:
       Serial.println("Estado: INTRO 2");
       myDFPlayer.playFolder(F_INFO, A_INTRO_2);
       delay(12000); // AUMENTADO de 8000 a 14000 para evitar corte
       currentState = STATE_INTRO_3;
       break;

    case STATE_INTRO_3:
       Serial.println("Estado: INTRO 3 (Esperando confirmacion)");
       myDFPlayer.playFolder(F_INFO, A_INTRO_3);
       // despues de este audio, esperamos '#'
       currentState = STATE_WAIT_START;
       break;

    case STATE_WAIT_START:
      // Solo avanzar si presiona '#'
      if (key == '#') {
        Serial.println("ACCION: Inicio Confirmado.");
        currentState = STATE_PLAY_Q1;
        delay(1000); 
      }
      break;

    // --- PREGUNTA 1 ---
    case STATE_PLAY_Q1:
      Serial.println("Pregunta 1");
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q1);
      currentState = STATE_WAIT_A1;
      inputAnswer = 0;
      break;

    case STATE_WAIT_A1:
      // LOGICA CAMBIADA: Si falla, explica y avanza (A_CORRECTA_IS_5)
      handleInput(key, RESP_Q1, STATE_PLAY_Q2, A_CORRECTA_IS_5);
      break;

    // --- PREGUNTA 2 ---
    case STATE_PLAY_Q2:
      Serial.println("Pregunta 2");
      delay(2000); 
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q2);
      currentState = STATE_WAIT_A2;
      inputAnswer = 0;
      break;

    case STATE_WAIT_A2:
      handleInput(key, RESP_Q2, STATE_PLAY_Q3, A_CORRECTA_IS_8);
      break;

     // --- PREGUNTA 3 ---
     case STATE_PLAY_Q3:
      Serial.println("Pregunta 3");
      delay(2000);
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q3);
      currentState = STATE_WAIT_A3;
      inputAnswer = 0;
      break;

    case STATE_WAIT_A3:
      handleInput(key, RESP_Q3, STATE_PLAY_Q4, A_CORRECTA_IS_9);
      break;

     // --- PREGUNTA 4 ---
     case STATE_PLAY_Q4:
      Serial.println("Pregunta 4");
      delay(2000);
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q4);
      currentState = STATE_WAIT_A4;
      inputAnswer = 0;
      break;

    case STATE_WAIT_A4:
      handleInput(key, RESP_Q4, STATE_FINISH, A_CORRECTA_IS_7);
      break;

    case STATE_FINISH:
      Serial.println("Fin");
      delay(2000);
      myDFPlayer.playFolder(F_FINAL, A_DESPEDIDA);
      codigoParaMaster = 5; // Baile
      delay(15000);
      currentState = STATE_INIT; // Reiniciar o Loop infinito
      break;
  }
}

// Función para manejar la lógica repetitiva de entrada
// NUEVO COMPORTAMIENTO: Siempre avanza a nextState. Si falla, reproduce explanationAudio.
void handleInput(char key, char correctAnswer, RobotState nextState, int explanationAudio) {
  if (key) {
    if (key >= '0' && key <= '9') {
       inputAnswer = key;
       Serial.print("Seleccionado: "); Serial.println(inputAnswer);
    } else if (key == '#') {
       if (inputAnswer == correctAnswer) {
          // CORRECTO
          Serial.println("Correcto!");
          
          Serial.println("DEBUG: Reproduciendo Exito...");
          myDFPlayer.playFolder(F_RESPUESTAS, A_EXITO_FX);
          codigoParaMaster = 2; // Feliz
          
          Serial.println("DEBUG: Esperando 3s...");
          delay(3000);
          
          Serial.println("DEBUG: Reproduciendo Muy Bien...");
          myDFPlayer.playFolder(F_RESPUESTAS, A_MUY_BIEN);
          
          Serial.println("DEBUG: Esperando 3s...");
          delay(3000);
          
       } else {
          // INCORRECTO
          Serial.println("Incorrecto!");
          
          Serial.println("DEBUG: Reproduciendo Error...");
          myDFPlayer.playFolder(F_RESPUESTAS, A_INCORRECTO_FX);
          codigoParaMaster = 1; // Triste
          
          Serial.println("DEBUG: Esperando 3s...");
          delay(3000);
          
          Serial.println("DEBUG: Reproduciendo Explicacion...");
          myDFPlayer.playFolder(F_RESPUESTAS, explanationAudio);
          
          Serial.println("DEBUG: Esperando 4s...");
          delay(4000); 
       }
       // SIEMPRE AVANZAMOS
       Serial.println("DEBUG: Avanzando estado...");
       codigoParaMaster = 0; 
       currentState = nextState;
    }
  }
}

void requestEvent() {
  Wire.write(codigoParaMaster);
}
