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
const int F_GRADE1  = 1;    // Carpeta 01: Primer Grado
const int F_GRADE2  = 2;    // Carpeta 02: Segundo Grado
const int F_FRASES  = 7;    // Frases Feedback
const int F_DESPEDIDA = 8;  // Despedida

// SFX FEEDBACK MAPPING
const int F_CORRECTO   = 90; 
const int F_INCORRECTO = 91; 
const int A_G1_ERROR_MENU      = 39; // 039.mp3 (Error G1) -> Usaremos este como generico si aplica
const int A_G1_EXPLAIN_RETRY   = 40; // 040.mp3 (Explicacion 2 G1) -> Veremos como manejar error en G2

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

// === GRADE 1 DATA ===
const int G1_DRILL_COUNT = 10;
Question g1_drill[G1_DRILL_COUNT] = {
   {3,'3'},{4,'4'},{7,'5'},{8,'4'},{9,'2'},{10,'3'},{11,'5'},{12,'6'},{13,'1'},{14,'2'}
};

const int G1_SM_COUNT = 10;
Question g1_sm[G1_SM_COUNT] = {
  {18,'B'},{19,'A'},{20,'B'},{21,'B'},{22,'A'},{23,'B'},{24,'A'},{25,'A'},{26,'B'},{27,'A'}
};

const int G1_VF_COUNT = 10;
Question g1_vf[G1_VF_COUNT] = {
  {29,'2'},{30,'2'},{31,'1'},{32,'2'},{33,'2'},{34,'1'},{35,'1'},{36,'1'},{37,'2'},{38,'1'}
};

// === GRADE 2 DATA ===
const int G2_DRILL_COUNT = 10;
Question g2_drill[G2_DRILL_COUNT] = {
  {2,'1'},{3,'2'},{4,'1'},{5,'0'},{6,'0'},{7,'1'},{8,'3'},{9,'0'},{10,'1'},{11,'2'}
};

const int G2_SM_COUNT = 9; // CSV has 013-021
Question g2_sm[G2_SM_COUNT] = {
  {13,'A'},{14,'A'},{15,'A'},{16,'A'},{17,'A'},{18,'A'},{19,'B'},{20,'B'},{21,'A'}
};

const int G2_VF_COUNT = 10; // CSV has 022-031
Question g2_vf[G2_VF_COUNT] = {
  {22,'2'},{23,'1'},{24,'1'},{25,'2'},{26,'1'},{27,'1'},{28,'2'},{29,'1'},{30,'1'},{31,'2'}
};


// === RUNTIME INDICES (Max size safe) ===
const int MAX_Q = 15;
int activeIndices[MAX_Q]; 
const int QUESTIONS_PER_ROUND = 5; 

// ==========================================
// ESTADOS GLOBAL
// ==========================================
enum State {
  ST_SETUP,
  ST_INTRO_SEQ,
  ST_WAIT_GRADE_SELECT, // Nuevo estado
  ST_GRADE_INTRO,       // Carga intro especifica
  ST_DRILL_GAME, 
  ST_MENU,
  ST_SM_GAME,
  ST_VF_GAME,
  ST_IDLE
};

State currentState = ST_SETUP;
int selectedGrade = 1; // 1 or 2
int currentQuestionIdx = 0;

void(* resetFunc) (void) = 0;

// ==========================================
// HELPERS
// ==========================================

void requestEvent() {
  Wire.write(codigoParaMaster);
}

void waitAudio(bool talks) {
  if(talks) codigoParaMaster = MASTER_TALK;
  unsigned long t = millis();
  while(digitalRead(PIN_BUSY) == HIGH && millis() - t < 1500) delay(10);
  while(digitalRead(PIN_BUSY) == LOW) delay(10);
  codigoParaMaster = MASTER_NEUTRAL;
  delay(200); 
}

void playFeedback(bool correct, bool isLast) {
  if (correct) {
    codigoParaMaster = MASTER_HAPPY;
    myDFPlayer.playFolder(F_CORRECTO, 1);
    waitAudio(false); 
    if (!isLast) {
      int phrase = random(1, 10); 
      myDFPlayer.playFolder(F_FRASES, phrase);
      waitAudio(true); 
    } else {
      codigoParaMaster = MASTER_NEUTRAL;
    }
  } else {
    codigoParaMaster = MASTER_SAD;
    myDFPlayer.playFolder(F_INCORRECTO, 1);
    waitAudio(false); 
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
  
  // Usamos el menu de error de G1 (039) como generico pq G2 no tiene.
  // "Deseas avanzar (1) o explicar (2)?"
  // folder: F_GRADE1 es 1.
  myDFPlayer.playFolder(F_GRADE1, A_G1_ERROR_MENU);
  waitAudio(true);
  
  char key = 0;
  while(true) {
    key = customKeypad.getKey();
    if (key == 'D') resetFunc(); 
    
    if (key == '1') {
       return;
    } else if (key == '2') {
       // Explicacion Rety.
       // G1: 40. G2: ??? G2 does not have "Explicacion 2" in CSV logic.
       // Fallback: Just skip for G2 or replay intro explanation?
       // For now, only G1 has dedicated retry explanation.
       if (selectedGrade == 1) {
          myDFPlayer.playFolder(F_GRADE1, A_G1_EXPLAIN_RETRY);
          waitAudio(true);
       } else {
          // G2: Play initial explanation again? (012)
          myDFPlayer.playFolder(F_GRADE2, 12);
          waitAudio(true);
       }
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

void prepareIndices(int count) {
  for(int i=0; i<count; i++) activeIndices[i] = i;
  shuffleArray(activeIndices, count);
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
      // Reproducir File 1: Bienvenida + "Presiona grado"
      myDFPlayer.playFolder(F_INTRO, 1);
      waitAudio(true);
      
      Serial.println("Waiting for Grade (1 or 2)...");
      currentState = ST_WAIT_GRADE_SELECT;
      break;

    case ST_WAIT_GRADE_SELECT:
      if (key == '1') {
        selectedGrade = 1;
        Serial.println("Selected Grade 1");
        // Play "Excelente" (Folder 10, File 2)
        myDFPlayer.playFolder(F_INTRO, 2);
        waitAudio(true);
        currentState = ST_GRADE_INTRO;
      } else if (key == '2') {
        selectedGrade = 2;
        Serial.println("Selected Grade 2");
         // Play "Excelente"
        myDFPlayer.playFolder(F_INTRO, 2);
        waitAudio(true);
        currentState = ST_GRADE_INTRO;
      }
      break;

    case ST_GRADE_INTRO:
       // 1. Play Grade Intro
       // G1: 001. G2: 001.
       int f_grade;
       if (selectedGrade==1) f_grade = F_GRADE1; else f_grade = F_GRADE2;

       myDFPlayer.playFolder(f_grade, 1);
       waitAudio(true);

       // 2. Play Generic "Comencemos calentar" (F10 - 003)
       myDFPlayer.playFolder(F_INTRO, 3);
       waitAudio(true);

       // 3. Play Explanations
       // G1: 002. G2: 012.
       if (selectedGrade == 1) myDFPlayer.playFolder(F_GRADE1, 2);
       else myDFPlayer.playFolder(F_GRADE2, 12);
       waitAudio(true);

       // 4. Play "Presiona 1 para continuar" (F10 - 004)
       myDFPlayer.playFolder(F_INTRO, 4);
       waitAudio(true);
       
       // Wait for '1'
       {
         bool waiting = true;
         while(waiting) {
           char k = customKeypad.getKey();
           if (k == 'D') resetFunc();
           if (k == '1') waiting = false;
         }
       }

       // Setup Drill
       if (selectedGrade == 1) prepareIndices(G1_DRILL_COUNT);
       else prepareIndices(G2_DRILL_COUNT);
       
       currentQuestionIdx = 0;
       currentState = ST_DRILL_GAME;
       break;
   
   case ST_DRILL_GAME:
      if (currentQuestionIdx >= QUESTIONS_PER_ROUND) {
         // Cierre: Usar G1/15 ("Hemos terminado...") para ambos, as requested/implied
         myDFPlayer.playFolder(F_GRADE1, 15); 
         waitAudio(true);
         currentState = ST_MENU;
         break;
      }
      
      {
         int realIdx = activeIndices[currentQuestionIdx];
         Question q;
         int f_grade = (selectedGrade==1) ? F_GRADE1 : F_GRADE2;
         if (selectedGrade==1) q = g1_drill[realIdx];
         else q = g2_drill[realIdx];

         bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
         
         Serial.print("Drill Q: "); Serial.println(q.fileNum);
         myDFPlayer.playFolder(f_grade, q.fileNum);
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
      
    case ST_MENU:
      Serial.println("--- MENU ---");
      // Menu G1/16 ("Presiona 1 SM, 2 VF") generico para ambos
      myDFPlayer.playFolder(F_GRADE1, 16);
      waitAudio(true);
      
      while(true) {
        char k = customKeypad.getKey();
        if (k == 'D') resetFunc();
        
        if(k == '1') {
          // SM
          if(selectedGrade == 1) prepareIndices(G1_SM_COUNT);
          else prepareIndices(G2_SM_COUNT);
          
          // Intro: G1/17 (SM Explicacion). Usar G1 para G2 tambien si no hay otro?
          // G1 CSV: 17 "Explicacion Dinamica SM". 
          // G2 CSV: No explicacion. Starts 013.
          // Usaremos G1/17 para ambos.
          myDFPlayer.playFolder(F_GRADE1, 17);
          waitAudio(true);
          
          currentQuestionIdx = 0;
          currentState = ST_SM_GAME;
          break;
        } else if (k == '2') {
          // VF
          if(selectedGrade == 1) prepareIndices(G1_VF_COUNT);
          else prepareIndices(G2_VF_COUNT);

          // Intro: G1/28 "Explicacion Dinamica VF".
          myDFPlayer.playFolder(F_GRADE1, 28);
          waitAudio(true);

          currentQuestionIdx = 0;
          currentState = ST_VF_GAME;
          break;
        }
      }
      break;
      
    case ST_SM_GAME:
      if (currentQuestionIdx >= QUESTIONS_PER_ROUND) {
         playFarewell();
         currentState = ST_IDLE; 
         break;
      }
      
      {
        int realIdx = activeIndices[currentQuestionIdx];
        Question q;
        int f_grade = (selectedGrade==1) ? F_GRADE1 : F_GRADE2;
        if (selectedGrade==1) q = g1_sm[realIdx];
        else q = g2_sm[realIdx];

        bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
        
        Serial.print("SM Q: "); Serial.println(q.fileNum);
        myDFPlayer.playFolder(f_grade, q.fileNum);
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
      
    case ST_VF_GAME:
       if (currentQuestionIdx >= QUESTIONS_PER_ROUND) {
         playFarewell();
         currentState = ST_IDLE; 
         break;
       }
      
      {
        int realIdx = activeIndices[currentQuestionIdx];
        Question q;
        int f_grade = (selectedGrade==1) ? F_GRADE1 : F_GRADE2;
        if (selectedGrade==1) q = g1_vf[realIdx];
        else q = g2_vf[realIdx];

        bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
        
        Serial.print("VF Q: "); Serial.println(q.fileNum);
        myDFPlayer.playFolder(f_grade, q.fileNum);
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
