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
const int PIN_BUSY = 3; // PIN DE CONTROL DE AUDIO

// CONSTANTES AUDIO (Mapeo desde CSV)
const int F_INFO = 1;
const int A_INTRO_1 = 1; 
const int A_INTRO_2 = 2; 
const int A_INTRO_3 = 3; 

const int F_EJERCICIOS = 2;
const int A_Q1 = 1; 
const int A_Q2 = 2; 
const int A_Q3 = 3; 
const int A_Q4 = 4; 

const int F_RESPUESTAS = 3;
const int A_CORRECTA_IS_5 = 1;
const int A_CORRECTA_IS_8 = 2;
const int A_CORRECTA_IS_9 = 3;
const int A_CORRECTA_IS_7 = 4;
const int A_INCORRECTO_FX = 5; 
const int A_EXCELENTE     = 6; 
const int A_EXITO_FX      = 7; 
const int A_MUY_BIEN      = 8; 
const int A_SIGAMOS       = 9; 

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
  STATE_WAIT_START,
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

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  
  pinMode(PIN_BUSY, INPUT); // Configurar pin BUSY
  
  Wire.begin(8);  
  Wire.onRequest(requestEvent);
  
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Error al iniciar DFPlayer");
  }
  myDFPlayer.volume(25);
  
  currentState = STATE_INIT;
}

// FUNCION MAESTRA PARA ESPERAR AUDIO
void waitAudio() {
  // Pequeña espera para dar tiempo al DFPlayer de poner el pin en LOW
  delay(500); 
  
  // Mientras el pin BUSY sea LOW (0V), el audio está sonando
  while (digitalRead(PIN_BUSY) == LOW) {
    // Esperamos...
    // AquÍ podríamos leer sensores o mantener alive el I2C, 
    // pero para este flujo lineal está bien bloquear.
    delay(10);
  }
  Serial.println("DEBUG: Audio Terminado.");
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
      waitAudio(); // Espera inteligente
      currentState = STATE_INTRO_2;
      break;
      
    case STATE_INTRO_2:
       Serial.println("Estado: INTRO 2");
       myDFPlayer.playFolder(F_INFO, A_INTRO_2);
       waitAudio();
       currentState = STATE_INTRO_3;
       break;

    case STATE_INTRO_3:
       Serial.println("Estado: INTRO 3 (Esperando confirmacion)");
       myDFPlayer.playFolder(F_INFO, A_INTRO_3);
       // Aqui NO esperamos audio porque queremos que el usuario pueda presionar # mientras habla?
       // O preferimos que termine de hablar?
       // El usuario dijo "reproduce... SI debe pedir introducir". 
       // Mejor esperemos que termine la instruccion antes de aceptar el #.
       waitAudio();
       Serial.println("Listo para recibir '#'");
       currentState = STATE_WAIT_START;
       break;

    case STATE_WAIT_START:
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
      waitAudio(); // Esperamos que termine la pregunta
      currentState = STATE_WAIT_A1;
      inputAnswer = 0;
      break;

    case STATE_WAIT_A1:
      handleInput(key, RESP_Q1, STATE_PLAY_Q2, A_CORRECTA_IS_5);
      break;

    // --- PREGUNTA 2 ---
    case STATE_PLAY_Q2:
      Serial.println("Pregunta 2");
      delay(1000); 
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q2);
      waitAudio();
      currentState = STATE_WAIT_A2;
      inputAnswer = 0;
      break;

    case STATE_WAIT_A2:
      handleInput(key, RESP_Q2, STATE_PLAY_Q3, A_CORRECTA_IS_8);
      break;

     // --- PREGUNTA 3 ---
     case STATE_PLAY_Q3:
      Serial.println("Pregunta 3");
      delay(1000);
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q3);
      waitAudio();
      currentState = STATE_WAIT_A3;
      inputAnswer = 0;
      break;

    case STATE_WAIT_A3:
      handleInput(key, RESP_Q3, STATE_PLAY_Q4, A_CORRECTA_IS_9);
      break;

     // --- PREGUNTA 4 ---
     case STATE_PLAY_Q4:
      Serial.println("Pregunta 4");
      delay(1000);
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q4);
      waitAudio();
      currentState = STATE_WAIT_A4;
      inputAnswer = 0;
      break;

    case STATE_WAIT_A4:
      handleInput(key, RESP_Q4, STATE_FINISH, A_CORRECTA_IS_7);
      break;

    case STATE_FINISH:
      Serial.println("Fin");
      delay(1000);
      myDFPlayer.playFolder(F_FINAL, A_DESPEDIDA);
      codigoParaMaster = 5; // Baile
      waitAudio();
      // delay extra para baile si dura mas que el audio
      delay(5000); 
      currentState = STATE_INIT; 
      break;
  }
}

// Función con WaitAudio integrado
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
          waitAudio();
          
          Serial.println("DEBUG: Reproduciendo Muy Bien...");
          myDFPlayer.playFolder(F_RESPUESTAS, A_MUY_BIEN);
          waitAudio();
          
       } else {
          // INCORRECTO
          Serial.println("Incorrecto!");
          
          Serial.println("DEBUG: Reproduciendo Error...");
          myDFPlayer.playFolder(F_RESPUESTAS, A_INCORRECTO_FX);
          codigoParaMaster = 1; // Triste
          waitAudio();
          
          Serial.println("DEBUG: Reproduciendo Explicacion...");
          myDFPlayer.playFolder(F_RESPUESTAS, explanationAudio);
          waitAudio();
       }
       
       Serial.println("DEBUG: Avanzando estado...");
       codigoParaMaster = 0; 
       currentState = nextState;
    }
  }
}

void requestEvent() {
  Wire.write(codigoParaMaster);
}
