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

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {13, 12, 11, 10}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// ==========================================
// CONFIGURACION AUDIO (DFPlayer)
// ==========================================
SoftwareSerial mySerial(2, 4);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
const int PIN_BUSY = 3; 

// CARPETAS
const int F_INTRO   = 10;   // Carpeta 10: Bienvenida
const int F_GRADE1  = 1;   // Carpeta 01: Primer Grado
const int F_FRASES  = 7;   // Frases Feedback
const int F_DESPEDIDA = 8; // Despedida

// SFX FEEDBACK MAPPING
const int F_CORRECTO   = 90; 
const int F_INCORRECTO = 91; 

// ARCHIVOS ESPECIALES
const int A_INTRO_LAST         = 4;  // 004.mp3 (Presiona 1 para continuar)
const int A_G1_MENU            = 16; // 016.mp3 (Menu de juegos)
const int A_G1_ERROR_MENU      = 39; // 039.mp3 (Deseas avanzar o explicar?)
const int A_G1_EXPLAIN_RETRY   = 40; // 040.mp3 (Explicacion 2)

// CODIGOS PARA MASTER (I2C)
const int MASTER_NEUTRAL = 0;
const int MASTER_SAD     = 1;
const int MASTER_HAPPY   = 2;
const int MASTER_TALK    = 20;

int codigoParaMaster = MASTER_NEUTRAL; 

// ==========================================
// DATOS DE JUEGOS
// ==========================================
struct Question {
  int fileNum;
  char correctAnswer;
};

// --- DRILL (Calentamiento) - Folder 1 ---
const int DRILL_TOTAL = 10; 
Question questionsDrill[DRILL_TOTAL] = {
   {3, '3'}, {4, '4'}, {7, '5'}, {8, '4'}, {9, '2'},
   {10, '3'}, {11, '5'}, {12, '6'}, {13, '1'}, {14, '2'}
};
int drillIndices[DRILL_TOTAL]; 

// Juego Seleccion Multiple (SM) - Folder 1
const int SM_TOTAL = 10;
Question questionsSM[SM_TOTAL] = {
  {18, 'B'}, {19, 'A'}, {20, 'B'}, {21, 'B'}, {22, 'A'},
  {23, 'B'}, {24, 'A'}, {25, 'A'}, {26, 'B'}, {27, 'A'}
};
int smIndices[SM_TOTAL];

// Juego Verdadero/Falso (VF) - Folder 1
const int VF_TOTAL = 10;
Question questionsVF[VF_TOTAL] = {
  {29, '2'}, {30, '2'}, {31, '1'}, {32, '2'}, {33, '2'},
  {34, '1'}, {35, '1'}, {36, '1'}, {37, '2'}, {38, '1'}
};
int vfIndices[VF_TOTAL];

const int QUESTIONS_PER_ROUND = 5; 

// ==========================================
// ESTADOS GLOBAL
// ==========================================
enum State {
  ST_SETUP,
  ST_INTRO_SEQ,
  ST_WAIT_GRADE1,
  ST_G1_INTRO_DRILL,
  ST_G1_DRILL_GAME, 
  ST_G1_MENU,
  ST_G1_SM_GAME,
  ST_G1_VF_GAME,
  ST_IDLE
};

State currentState = ST_SETUP;
int currentQuestionIdx = 0;

void(* resetFunc) (void) = 0;

// ==========================================
// HELPERS
// ==========================================

void requestEvent() {
  Wire.write(codigoParaMaster);
}

// talks = true -> Envia 20 (Master Talk) mientras reproduce
// talks = false -> Mantiene codigo anterior (Master Happy/Sad) mientras reproduce
void waitAudio(bool talks) {
  if(talks) codigoParaMaster = MASTER_TALK;
  
  unsigned long t = millis();
  while(digitalRead(PIN_BUSY) == HIGH && millis() - t < 1500) delay(10);
  while(digitalRead(PIN_BUSY) == LOW) delay(10);
  
  // Al terminar, siempre volvemos a Neutro si estabamos hablando
  // O reseteamos si estabamos riendo/llorando para limpiar la expresion
  codigoParaMaster = MASTER_NEUTRAL;
  delay(200); 
}

// bool isLast: Si es true, NO reproduce la frase de 'Siguiente pregunta'
void playFeedback(bool correct, bool isLast) {
  if (correct) {
    // 1. Sonido FX Correcto + CARA FELIZ
    codigoParaMaster = MASTER_HAPPY;
    myDFPlayer.playFolder(F_CORRECTO, 1);
    waitAudio(false); // NO sobrescribir cara con Talk. Mantener Happy.
    
    // 2. Frase Motivadora (Solo si no es la ultima)
    if (!isLast) {
      int phrase = random(1, 10); 
      myDFPlayer.playFolder(F_FRASES, phrase);
      waitAudio(true); // Aqui si Talk
    } else {
      // Si es la ultima, aseguramos volver a neutral manual pq waitAudio(false) arriba quizas dejo Happy
      codigoParaMaster = MASTER_NEUTRAL;
    }
    
  } else {
    // 1. Sonido FX Incorrecto + CARA TRISTE
    codigoParaMaster = MASTER_SAD;
    myDFPlayer.playFolder(F_INCORRECTO, 1);
    waitAudio(false); 

    // 2. Frase Animo (Solo si no es la ultima)
    if (!isLast) {
      int phrase = random(10, 20); 
      myDFPlayer.playFolder(F_FRASES, phrase);
      waitAudio(true);
    } else {
      codigoParaMaster = MASTER_NEUTRAL;
    }
  }
}

void playFarewell() {
  Serial.println("--- FAREWELL ---");
  int bye = random(1, 9); 
  myDFPlayer.playFolder(F_DESPEDIDA, bye);
  waitAudio(true);
}

void handleErrorFlow(bool isLast) {
  playFeedback(false, isLast); 
  
  // Si fue la ultima pregunta, el feedback ya se dio (solo sfx) y no hay "siguiente".
  // Pero la logica original ofrece repetir explicacion...
  // Si es la ultima, quizas ya no deberiamos ofrecer menu?
  // El usuario pidio "no reproducir sonido feedback de siguiente".
  // Asumiremos que el menu de error (Deseas avanzar o explicar) sigue siendo valido
  
  myDFPlayer.playFolder(F_GRADE1, A_G1_ERROR_MENU);
  waitAudio(true);
  
  char key = 0;
  while(true) {
    key = customKeypad.getKey();
    if (key == 'D') resetFunc(); 
    
    if (key == '1') {
       return;
    } else if (key == '2') {
       myDFPlayer.playFolder(F_GRADE1, A_G1_EXPLAIN_RETRY);
       waitAudio(true);
       return; 
    }
  }
}

void shuffleArray(int* array, int size) {
  for (int i = size - 1; i > 0; i--) {
    int j = random(i + 1);
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

void prepareIndices() {
  for(int i=0; i<DRILL_TOTAL; i++) drillIndices[i] = i;
  shuffleArray(drillIndices, DRILL_TOTAL);
  
  for(int i=0; i<SM_TOTAL; i++) smIndices[i] = i;
  shuffleArray(smIndices, SM_TOTAL);
  
  for(int i=0; i<VF_TOTAL; i++) vfIndices[i] = i;
  shuffleArray(vfIndices, VF_TOTAL);
}

// ==========================================
// MAIN
// ==========================================

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(PIN_BUSY, INPUT);
  randomSeed(analogRead(A1)); 
  
  Wire.begin(8);
  Wire.onRequest(requestEvent); 
  
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println(F("Error DFPlayer"));
  }
  myDFPlayer.volume(25);
  delay(3000);
  
  prepareIndices(); 
  
  currentState = ST_INTRO_SEQ;
}

void loop() {
  char key = customKeypad.getKey();
  
  if (key == 'D') {
      Serial.println("Reset");
      codigoParaMaster = 99;
      long w = millis();
      while(millis() - w < 1500) delay(10);
      resetFunc(); 
  }

  switch(currentState) {
    
    case ST_INTRO_SEQ:
      Serial.println("--- INTRO ---");
      for(int i=1; i<=3; i++){
        myDFPlayer.playFolder(F_INTRO, i);
        waitAudio(true);
      }
      myDFPlayer.playFolder(F_INTRO, A_INTRO_LAST);
      waitAudio(true);
      
      Serial.println("Waiting for 1...");
      currentState = ST_WAIT_GRADE1;
      break;
      
    case ST_WAIT_GRADE1:
      if (key == '1') {
        Serial.println("1 detected.");
        currentState = ST_G1_INTRO_DRILL;
      }
      break;
      
   case ST_G1_INTRO_DRILL:
      Serial.println("--- G1 DRILL INTRO ---");
      for(int i=1; i<=2; i++){
         myDFPlayer.playFolder(F_GRADE1, i);
         waitAudio(true);
      }
      currentQuestionIdx = 0;
      currentState = ST_G1_DRILL_GAME;
      break;
   
   case ST_G1_DRILL_GAME:
      // Round 1
      if (currentQuestionIdx >= QUESTIONS_PER_ROUND) {
         myDFPlayer.playFolder(F_GRADE1, 15); // Cierre Drill
         waitAudio(true);
         currentState = ST_G1_MENU;
         break;
      }
      
      {
         int realIdx = drillIndices[currentQuestionIdx];
         Question q = questionsDrill[realIdx];
         // Check if this is the last question of the round
         bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
         
         Serial.print("Drill Q: "); Serial.println(q.fileNum);
         myDFPlayer.playFolder(F_GRADE1, q.fileNum);
         waitAudio(true);
         
         bool answered = false;
         while(!answered) {
            char k = customKeypad.getKey();
            if (k == 'D') resetFunc();
            
            if (k >= '0' && k <= '9') {
               if (k == q.correctAnswer) {
                  playFeedback(true, isLast);
               } else {
                  handleErrorFlow(isLast);
               }
               answered = true;
            }
         }
         currentQuestionIdx++;
      }
      break;
      
    case ST_G1_MENU:
      Serial.println("--- MENU ---");
      myDFPlayer.playFolder(F_GRADE1, A_G1_MENU);
      waitAudio(true);
      
      while(true) {
        char k = customKeypad.getKey();
        if (k == 'D') resetFunc();
        
        if(k == '1') {
          currentQuestionIdx = 0;
          currentState = ST_G1_SM_GAME;
          myDFPlayer.playFolder(F_GRADE1, 17); // Intro SM
          waitAudio(true);
          break;
        } else if (k == '2') {
          currentQuestionIdx = 0;
          currentState = ST_G1_VF_GAME;
          myDFPlayer.playFolder(F_GRADE1, 28); // Intro VF
          waitAudio(true);
          break;
        }
      }
      break;
      
    case ST_G1_SM_GAME:
      // Round 2 (SM)
      if (currentQuestionIdx >= QUESTIONS_PER_ROUND) {
         playFarewell();
         currentState = ST_IDLE; 
         break;
      }
      
      {
        int realIdx = smIndices[currentQuestionIdx];
        Question q = questionsSM[realIdx];
        bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
        
        Serial.print("SM Q: "); Serial.println(q.fileNum);
        myDFPlayer.playFolder(F_GRADE1, q.fileNum);
        waitAudio(true);
        
        bool answered = false;
        while(!answered) {
           char k = customKeypad.getKey();
           if (k == 'D') resetFunc();
           
           if (k == 'A' || k == 'B') {
              if (k == q.correctAnswer) playFeedback(true, isLast);
              else handleErrorFlow(isLast);
              answered = true;
           }
        }
        currentQuestionIdx++;
      }
      break;
      
    case ST_G1_VF_GAME:
      // Round 2 (VF)
      if (currentQuestionIdx >= QUESTIONS_PER_ROUND) {
         playFarewell();
         currentState = ST_IDLE; 
         break;
      }
      
      {
        int realIdx = vfIndices[currentQuestionIdx];
        Question q = questionsVF[realIdx];
        bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
        
        Serial.print("VF Q: "); Serial.println(q.fileNum);
        myDFPlayer.playFolder(F_GRADE1, q.fileNum);
        waitAudio(true);
        
        bool answered = false;
        while(!answered) {
           char k = customKeypad.getKey();
           if (k == 'D') resetFunc();
           
           if (k == '1' || k == '2') {
              if (k == q.correctAnswer) playFeedback(true, isLast);
              else handleErrorFlow(isLast);
              answered = true;
           }
        }
        currentQuestionIdx++;
      }
      break;
      
    case ST_IDLE:
      // Robot Sleeps. Waiting for 'D' (Reset).
      break;
  }
}
