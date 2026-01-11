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

// VARIABLES RESPUESTAS ESPERADAS (MODIFICADO A String)
const String RESP_Q1 = "5";
const String RESP_Q2 = "8";
const String RESP_Q3 = "9";
const String RESP_Q4 = "7";

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

// Buffer para entrada acumulativa (Soporta N digitos)
String inputBuffer = "";

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  
  pinMode(PIN_BUSY, INPUT); 
  
  Wire.begin(8);  
  Wire.onRequest(requestEvent);
  
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Error al iniciar DFPlayer");
  }
  myDFPlayer.volume(25);
  
  currentState = STATE_INIT;
}

// FUNCION MAESTRA PARA ESPERAR AUDIO
// FUNCION MAESTRA PARA ESPERAR AUDIO
void waitAudio(bool isVoice) {
  // 1. Avisar al Master si es voz
  if (isVoice) {
    codigoParaMaster = 20; 
  }
  
  // ESPERAR A QUE EMPIECE (BUSY baje a LOW)
  // Damos hasta 1.5 segundos para que el DFPlayer procese y empiece a sonar
  unsigned long startTime = millis();
  while (digitalRead(PIN_BUSY) == HIGH && millis() - startTime < 1500) {
    delay(10);
  }
  
  // Si aun sigue HIGH, es que no arrancó (timeout), pero igual seguimos para no bloquear
  
  // ESPERAR A QUE TERMINE (BUSY suba a HIGH)
  while (digitalRead(PIN_BUSY) == LOW) {
    delay(10);
  }
  
  // 2. Avisar al Master que terminamos
  if (isVoice) {
    codigoParaMaster = 0;
  }
  Serial.println("DEBUG: Audio Terminado.");
}

void(* resetFunc) (void) = 0; // Declarar función de reset en dirección 0

void loop() {
  char key = customKeypad.getKey();
  
  if (key) {
    Serial.print("DEBUG: Tecla: ");
    Serial.println(key);
    
    // REINICIO TOTAL (Tecla 'D')
    if (key == 'D') {
      Serial.println("!!! REINICIO SOLICITADO !!!");
      codigoParaMaster = 99; // Codigo especial para reiniciar Master
      
      // Esperamos un momento para asegurar que el Master lea el código vía I2C
      // El Master lee cada 100ms, damos 1 segundo de margen.
      long startWait = millis();
      while(millis() - startWait < 1000) {
         // Mantener bus I2C activo por si acaso
         delay(10);
      }
      
      resetFunc(); // Reiniciar Slave
    }
  }
  
  switch (currentState) {
    case STATE_INIT:
      Serial.println("Estado: INTRO 1");
      myDFPlayer.playFolder(F_INFO, A_INTRO_1); 
      waitAudio(true); 
      currentState = STATE_INTRO_2;
      break;
      
    case STATE_INTRO_2:
       Serial.println("Estado: INTRO 2");
       myDFPlayer.playFolder(F_INFO, A_INTRO_2);
       waitAudio(true);
       currentState = STATE_INTRO_3;
       break;

    case STATE_INTRO_3:
       Serial.println("Estado: INTRO 3 (Esperando confirmacion)");
       myDFPlayer.playFolder(F_INFO, A_INTRO_3);
       waitAudio(true);
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
      waitAudio(true); 
      currentState = STATE_WAIT_A1;
      inputBuffer = ""; // Resetear buffer
      break;

    case STATE_WAIT_A1:
      handleInputAuto(key, RESP_Q1, STATE_PLAY_Q2, A_CORRECTA_IS_5);
      break;

    // --- PREGUNTA 2 ---
    case STATE_PLAY_Q2:
      Serial.println("Pregunta 2");
      delay(1000); 
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q2);
      waitAudio(true);
      currentState = STATE_WAIT_A2;
      inputBuffer = "";
      break;

    case STATE_WAIT_A2:
      handleInputAuto(key, RESP_Q2, STATE_PLAY_Q3, A_CORRECTA_IS_8);
      break;

     // --- PREGUNTA 3 ---
     case STATE_PLAY_Q3:
      Serial.println("Pregunta 3");
      delay(1000);
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q3);
      waitAudio(true);
      currentState = STATE_WAIT_A3;
      inputBuffer = "";
      break;

    case STATE_WAIT_A3:
      handleInputAuto(key, RESP_Q3, STATE_PLAY_Q4, A_CORRECTA_IS_9);
      break;

     // --- PREGUNTA 4 ---
     case STATE_PLAY_Q4:
      Serial.println("Pregunta 4");
      delay(1000);
      myDFPlayer.playFolder(F_EJERCICIOS, A_Q4);
      waitAudio(true);
      currentState = STATE_WAIT_A4;
      inputBuffer = "";
      break;

    case STATE_WAIT_A4:
      handleInputAuto(key, RESP_Q4, STATE_FINISH, A_CORRECTA_IS_7);
      break;

    case STATE_FINISH:
      Serial.println("Fin");
      delay(1000);
      myDFPlayer.playFolder(F_FINAL, A_DESPEDIDA);
      codigoParaMaster = 5; 
      waitAudio(true);
      delay(5000); 
      currentState = STATE_INIT; 
      break;
  }
}

// Función con Validación AUTOMÁTICA (Sin '#')
void handleInputAuto(char key, String correctAnswer, RobotState nextState, int explanationAudio) {
  if (key) {
    if (key >= '0' && key <= '9') {
       inputBuffer += key; // Agregar dígito
       Serial.print("Buffer actual: "); Serial.println(inputBuffer);
       
       // Verificar Longitud
       if (inputBuffer.length() >= correctAnswer.length()) {
         Serial.println("DEBUG: Longitud alcanzada. Verificando...");
         
         if (inputBuffer == correctAnswer) {
            // CORRECTO
            Serial.println("Correcto!");
            
            Serial.println("DEBUG: Reproduciendo Exito...");
            myDFPlayer.playFolder(F_RESPUESTAS, A_EXITO_FX);
            codigoParaMaster = 2; // Feliz
            waitAudio(false); // NO HABLAR (Solo SFX)
            
            Serial.println("DEBUG: Reproduciendo Muy Bien...");
            myDFPlayer.playFolder(F_RESPUESTAS, A_MUY_BIEN);
            waitAudio(true); // SI HABLAR
            
            codigoParaMaster = 0; 
            currentState = nextState;
            
         } else {
            // INCORRECTO
            Serial.println("Incorrecto!");
            
            Serial.println("DEBUG: Reproduciendo Error...");
            myDFPlayer.playFolder(F_RESPUESTAS, A_INCORRECTO_FX);
            codigoParaMaster = 1; // Triste
            waitAudio(false); // NO HABLAR (Solo SFX)
            
            Serial.println("DEBUG: Reproduciendo Explicacion...");
            myDFPlayer.playFolder(F_RESPUESTAS, explanationAudio);
            waitAudio(true); // SI HABLAR
            
            // Avanzar aunque falló (regla definida previamente)
            codigoParaMaster = 0; 
            currentState = nextState;
         }
         
         // Limpiar buffer por si acaso (aunque cambiamos de estado)
         inputBuffer = "";
       }
    }
    // Ignoramos '#' aquí, ya que es auto-validación
  }
}

void requestEvent() {
  Wire.write(codigoParaMaster);
}
