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
const int F_GRADE3  = 3;    // Carpeta 03: Tercer Grado
const int F_GRADE4  = 4;    // Carpeta 04: Cuarto Grado
const int F_FRASES  = 7;    // Frases Feedback
const int F_DESPEDIDA = 8;  // Despedida

// SFX FEEDBACK MAPPING
const int F_CORRECTO   = 90; 
const int F_INCORRECTO = 91; 
const int A_G1_ERROR_MENU      = 39; 
const int A_G1_EXPLAIN_RETRY   = 40; 

// CODIGOS PARA MASTER (I2C)
const int MASTER_NEUTRAL = 0;
const int MASTER_SAD     = 1;
const int MASTER_HAPPY   = 2;
const int MASTER_TALK    = 20;

int codigoParaMaster = MASTER_NEUTRAL; 

// NAVIGATION CODES
const int NAV_NONE = 0;
const int NAV_NEXT = 999;
const int NAV_PREV = -999;

// ==========================================
// DATOS DE JUEGOS
// ==========================================
struct Question {
  int fileNum;
  int correctAnswer; 
};

// === GRADE 1 DATA ===
const int G1_DRILL_COUNT = 10;
Question g1_drill[G1_DRILL_COUNT] = {
   {3,3},{4,4},{7,5},{8,4},{9,2},{10,3},{11,5},{12,6},{13,1},{14,2}
};
const int G1_SM_COUNT = 10;
Question g1_sm[G1_SM_COUNT] = {
  {18,2},{19,1},{20,2},{21,2},{22,1},{23,2},{24,1},{25,1},{26,2},{27,1} 
};
const int G1_VF_COUNT = 10;
Question g1_vf[G1_VF_COUNT] = {
  {29,2},{30,2},{31,1},{32,2},{33,2},{34,1},{35,1},{36,1},{37,2},{38,1}
};

// === GRADE 2 DATA ===
const int G2_DRILL_COUNT = 10;
Question g2_drill[G2_DRILL_COUNT] = {
  {2,1},{3,2},{4,1},{5,0},{6,0},{7,1},{8,3},{9,0},{10,1},{11,2}
};
const int G2_SM_COUNT = 9; 
Question g2_sm[G2_SM_COUNT] = {
  {13,1},{14,1},{15,1},{16,1},{17,1},{18,1},{19,2},{20,2},{21,1}
};
const int G2_VF_COUNT = 10; 
Question g2_vf[G2_VF_COUNT] = {
  {22,2},{23,1},{24,1},{25,2},{26,1},{27,1},{28,2},{29,1},{30,1},{31,2}
};

// === GRADE 3 DATA ===
const int G3_DRILL_COUNT = 10;
Question g3_drill[G3_DRILL_COUNT] = {
  {2,2},{3,3},{4,6},{5,4},{6,1},{7,10},{8,8},{9,9},{10,0},{11,0} 
};

const int G3_SM_COUNT = 10;
Question g3_sm[G3_SM_COUNT] = {
  {14,2},{15,1},{16,2},{17,1},{18,2},{19,1},{20,2},{21,1},{22,1},{23,1}
};

const int G3_SD_COUNT = 10; 
Question g3_sd[G3_SD_COUNT] = {
  {25,14},{26,8},{27,9},{28,12},{29,8},{30,9},{31,15},{32,12},{33,14},{34,2}
};

// === GRADE 4 DATA ===
const int G4_DRILL_COUNT = 10;
Question g4_drill[G4_DRILL_COUNT] = {
  {2,12}, {3,20}, {4,45}, {5,100}, {6,88}, {7,99}, {8,70}, {9,84}, {10,0}, {11,0}
};

const int G4_SM_COUNT = 9;
Question g4_sm[G4_SM_COUNT] = {
  {13,1}, {14,2}, {15,1}, {16,2}, {17,1}, {18,2}, {19,1}, {20,1}, {21,1}
  // 14 map to Option B (2) per user req. 
};

const int G4_SD_COUNT = 10;
Question g4_sd[G4_SD_COUNT] = {
  {22,42}, {23,88}, {24,60}, {25,26}, {26,40}, {27,45}, {28,75}, {29,40}, {30,0}, {31,90}
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
  ST_WAIT_GRADE_SELECT,
  ST_GRADE_INTRO,
  ST_DRILL_GAME, 
  ST_MENU,
  ST_SM_GAME,
  ST_VF_GAME, 
  ST_IDLE
};

State currentState = ST_SETUP;
int selectedGrade = 1; 
int currentQuestionIdx = 0;

void(* resetFunc) (void) = 0;

// ==========================================
// HELPERS
// ==========================================

void requestEvent() {
  Wire.write(codigoParaMaster);
}

// UPDATE: Takes INT STATUS to maintain on I2C
int waitAudio(int status) {
  codigoParaMaster = status;
  
  unsigned long t = millis();
  
  // 1. Wait for START (BUSY goes LOW)
  while(digitalRead(PIN_BUSY) == HIGH && millis() - t < 2500) {
     char k = customKeypad.getKey();
     if (k == '#') { myDFPlayer.stop(); return NAV_NEXT; }
     if (k == '*') { myDFPlayer.stop(); return NAV_PREV; }
     if (k == 'D') resetFunc();
     delay(10);
  }
  
  // 2. Wait for END (BUSY goes HIGH)
  unsigned long highStart = 0;
  while(true) {
     char k = customKeypad.getKey();
     if (k == '#') { myDFPlayer.stop(); return NAV_NEXT; }
     if (k == '*') { myDFPlayer.stop(); return NAV_PREV; }
     if (k == 'D') resetFunc();
     
     if (digitalRead(PIN_BUSY) == HIGH) {
        if (highStart == 0) highStart = millis();
        if (millis() - highStart > 200) break; // 200ms debounce
     } else {
        highStart = 0; 
     }
     delay(10);
  }
  
  codigoParaMaster = MASTER_NEUTRAL;
  delay(300); 
  return NAV_NONE;
}

// UPDATE: Takes INT STATUS
int playFolderSafe(int folder, int file, int status) {
  myDFPlayer.playFolder(folder, file);
  
  // Blind wait but checking keys
  unsigned long t = millis();
  while(millis() - t < 500) {
     char k = customKeypad.getKey();
     if (k == '#') { myDFPlayer.stop(); return NAV_NEXT; }
     if (k == '*') { myDFPlayer.stop(); return NAV_PREV; }
     if (k == 'D') resetFunc();
     delay(10);
  }
  
  return waitAudio(status);
}

void playFeedback(bool correct, bool isLast) {
  if (correct) {
    // Send HAPPY status while playing SFX
    myDFPlayer.playFolder(F_CORRECTO, 1);
    waitAudio(MASTER_HAPPY); 
    
    if (!isLast) {
      int phrase = random(1, 10); 
      // Send TALK status while phrases
      playFolderSafe(F_FRASES, phrase, MASTER_TALK);
    } else {
      codigoParaMaster = MASTER_NEUTRAL;
    }
  } else {
    // Send SAD status while playing SFX
    myDFPlayer.playFolder(F_INCORRECTO, 1);
    waitAudio(MASTER_SAD); 
    
    if (!isLast) {
      int phrase = random(10, 20); 
      // Send TALK status while phrases
      playFolderSafe(F_FRASES, phrase, MASTER_TALK);
    } else {
      codigoParaMaster = MASTER_NEUTRAL;
    }
  }
}

void playFarewell() {
  Serial.println("--- FAREWELL ---");
  int bye = random(1, 9); 
  playFolderSafe(F_DESPEDIDA, bye, MASTER_TALK);
}

void handleErrorFlow(bool isLast) {
  playFeedback(false, isLast); 
  
  int res;
  if (selectedGrade == 3) res = playFolderSafe(F_GRADE3, 35, MASTER_TALK);
  else if (selectedGrade == 4) res = playFolderSafe(F_GRADE4, 12, MASTER_TALK); // Usar Explicacion como fallback
  else res = playFolderSafe(F_GRADE1, A_G1_ERROR_MENU, MASTER_TALK);
  
  if (res != NAV_NONE) return; 
  
  char key = 0;
  while(true) {
    key = customKeypad.getKey();
    if (key == 'D') resetFunc(); 
    
    if (key == '1') return;
    if (key == '2') {
       if (selectedGrade == 1) playFolderSafe(F_GRADE1, A_G1_EXPLAIN_RETRY, MASTER_TALK);
       else if (selectedGrade == 2) playFolderSafe(F_GRADE2, 12, MASTER_TALK); 
       else if (selectedGrade == 3) playFolderSafe(F_GRADE3, 12, MASTER_TALK);
       else playFolderSafe(F_GRADE4, 12, MASTER_TALK);
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

int getGradeFolder() {
  if (selectedGrade == 1) return F_GRADE1;
  if (selectedGrade == 2) return F_GRADE2;
  if (selectedGrade == 3) return F_GRADE3;
  if (selectedGrade == 4) return F_GRADE4;
  return F_GRADE1;
}

int waitAnswerLogic(int expected) {
   int digitsNeeded = 1;
   if (expected >= 100) digitsNeeded = 3;
   else if (expected >= 10) digitsNeeded = 2;
   
   if (digitsNeeded == 1) {
      while(true) {
         char k = customKeypad.getKey();
         if (k == 'D') resetFunc();
         if (k == '#') return NAV_NEXT;
         if (k == '*') return NAV_PREV;
         
         if (k) {
            if (k >= '0' && k <= '9') return k - '0';
            if (k == 'A') return 1; 
            if (k == 'B') return 2; 
         }
      }
   } else if (digitsNeeded == 2) {
      int val = 0;
      // Digit 1
      while(true) {
         char k = customKeypad.getKey();
         if (k == 'D') resetFunc();
         if (k == '#') return NAV_NEXT;
         if (k == '*') return NAV_PREV;
         
         if (k >= '0' && k <= '9') {
            val = (k - '0') * 10;
            break; 
         }
      }
      // Digit 2
      while(true) {
         char k = customKeypad.getKey();
         if (k == 'D') resetFunc();
         if (k == '#') return NAV_NEXT; 
         if (k == '*') return NAV_PREV; 
         
         if (k >= '0' && k <= '9') {
            val += (k - '0');
            break; 
         }
      }
      return val;
   } else {
      // 3 Digits (Example: 100)
      int val = 0;
      // Digit 1
      while(true) {
         char k = customKeypad.getKey();
         if (k == 'D') resetFunc();
         if (k == '#') return NAV_NEXT;
         if (k == '*') return NAV_PREV;
         if (k >= '0' && k <= '9') { val = (k - '0') * 100; break; }
      }
      // Digit 2
      while(true) {
         char k = customKeypad.getKey();
         if (k == 'D') resetFunc();
         if (k == '#') return NAV_NEXT;
         if (k == '*') return NAV_PREV;
         if (k >= '0' && k <= '9') { val += (k - '0') * 10; break; }
      }
      // Digit 3
      while(true) {
         char k = customKeypad.getKey();
         if (k == 'D') resetFunc();
         if (k == '#') return NAV_NEXT;
         if (k == '*') return NAV_PREV;
         if (k >= '0' && k <= '9') { val += (k - '0'); break; }
      }
      return val;
   }
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
  myDFPlayer.volume(20); 
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

  // Common Nav check not needed here as loops handle it
  
  switch(currentState) {
    
    case ST_INTRO_SEQ:
      Serial.println("--- INTRO ---");
      if (playFolderSafe(F_INTRO, 1, MASTER_TALK) == NAV_NEXT) { /* Next */ }
      
      Serial.println("Waiting for Grade (1, 2, 3, 4)...");
      currentState = ST_WAIT_GRADE_SELECT;
      break;

    case ST_WAIT_GRADE_SELECT:
      if (key == '1' || key == '2' || key == '3' || key == '4') {
        selectedGrade = key - '0';
        Serial.print("Selected Grade: "); Serial.println(selectedGrade);
        
        playFolderSafe(F_INTRO, 2, MASTER_TALK);
        currentState = ST_GRADE_INTRO;
      }
      break;

    case ST_GRADE_INTRO:
       // 1. Grade Intro
       playFolderSafe(getGradeFolder(), 1, MASTER_TALK);

       // 2. Generic Intro
       playFolderSafe(F_INTRO, 3, MASTER_TALK);

       // 3. Explanation
       if (selectedGrade == 1) playFolderSafe(F_GRADE1, 2, MASTER_TALK);
       else if (selectedGrade == 2) playFolderSafe(F_GRADE2, 12, MASTER_TALK);
       else if (selectedGrade == 3) playFolderSafe(F_GRADE3, 12, MASTER_TALK);
       else playFolderSafe(F_GRADE4, 12, MASTER_TALK);

       // 4. Press 1
       playFolderSafe(F_INTRO, 4, MASTER_TALK);
       
       {
         bool waiting = true;
         while(waiting) {
           char k = customKeypad.getKey();
           if (k == 'D') resetFunc();
           if (k == '1' || k == '#') waiting = false; 
         }
       }

       if (selectedGrade == 1) prepareIndices(G1_DRILL_COUNT);
       else if (selectedGrade == 2) prepareIndices(G2_DRILL_COUNT);
       else if (selectedGrade == 3) prepareIndices(G3_DRILL_COUNT);
       else prepareIndices(G4_DRILL_COUNT);
       
       currentQuestionIdx = 0;
       currentState = ST_DRILL_GAME;
       break;
   
   case ST_DRILL_GAME:
      if (currentQuestionIdx >= QUESTIONS_PER_ROUND) {
         playFolderSafe(F_GRADE1, 15, MASTER_TALK); 
         currentState = ST_MENU;
         break;
      }
      
      {
         int realIdx = activeIndices[currentQuestionIdx];
         Question q;
         if (selectedGrade==1) q = g1_drill[realIdx];
         else if (selectedGrade==2) q = g2_drill[realIdx];
         else if (selectedGrade==3) q = g3_drill[realIdx];
         else q = g4_drill[realIdx];

         bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
         int folder = getGradeFolder();
         
         Serial.print("Drill Q: "); Serial.println(q.fileNum);
         int nav = playFolderSafe(folder, q.fileNum, MASTER_TALK);
         
         if (nav == NAV_NEXT) { currentQuestionIdx++; return; }
         if (nav == NAV_PREV) { if(currentQuestionIdx > 0) currentQuestionIdx--; return; }
         
         int userAns = waitAnswerLogic(q.correctAnswer);
         
         if (userAns == NAV_NEXT) { currentQuestionIdx++; return; }
         if (userAns == NAV_PREV) { if(currentQuestionIdx > 0) currentQuestionIdx--; return; }
         
         if (userAns == q.correctAnswer) {
             playFeedback(true, isLast);
         } else {
             handleErrorFlow(isLast);
         }
         currentQuestionIdx++;
      }
      break;
      
    case ST_MENU:
      Serial.println("--- MENU ---");
      if (selectedGrade == 3) playFolderSafe(F_GRADE3, 13, MASTER_TALK);
      else if (selectedGrade == 4) playFolderSafe(F_GRADE4, 13, MASTER_TALK); // Using F04/13 as Menu Prompt? (Check CSV)
      // CSV 13 is "Selection Multiple Question 1" actually... 
      // Grade 3 used F03/13 "Explanation". 
      // Grade 4 doesn't have a specific "Menu" prompt listed in CSV 
      // We will re-use Grade 3's menu prompt logic if file missing or reuse Grade 1 generic.
      // Actually CSV says 04_13 is "Cuarto grado: Pregunta - SM". 
      // The flow expects a "Select Game" prompt. 
      // For now, let's use F_GRADE1, 16 (Generic "Elige un juego") for G4 too.
      else playFolderSafe(F_GRADE1, 16, MASTER_TALK);
      
      while(true) {
        char k = customKeypad.getKey();
        if (k == 'D') resetFunc();
        
        if(k == '1') {
          if(selectedGrade == 1) prepareIndices(G1_SM_COUNT);
          else if(selectedGrade == 2) prepareIndices(G2_SM_COUNT);
          else if(selectedGrade == 3) prepareIndices(G3_SM_COUNT);
          else prepareIndices(G4_SM_COUNT);
          
          if (selectedGrade != 3 && selectedGrade != 4) playFolderSafe(F_GRADE1, 17, MASTER_TALK);
          else if (selectedGrade == 3) playFolderSafe(F_GRADE1, 17, MASTER_TALK);
          else playFolderSafe(F_GRADE1, 17, MASTER_TALK); // Generic "Vamos a jugar SM"
          
          currentQuestionIdx = 0;
          currentState = ST_SM_GAME;
          break;
        } else if (k == '2') {
          if(selectedGrade == 1) prepareIndices(G1_VF_COUNT);
          else if(selectedGrade == 2) prepareIndices(G2_VF_COUNT);
          else if(selectedGrade == 3) prepareIndices(G3_SD_COUNT);
          else prepareIndices(G4_SD_COUNT);

          if (selectedGrade == 3) {
             playFolderSafe(F_GRADE3, 24, MASTER_TALK);
          } else if (selectedGrade == 4) {
             playFolderSafe(F_GRADE4, 22, MASTER_TALK); // Fallback: First question of SD? 
             // Or maybe generic. G4 CSV 022 is "Pregunta SD". 
             // Let's just play nothing or generic intro? 
             // Let's play G1/28 "Verdadero o Falso" or G1/something?
             // Since G3 SD had a specific intro (F03/24), G4 SD (022) is a question.
             // We will skip intro prompt for G4 SD for now or use G1 generic.
          } else {
             playFolderSafe(F_GRADE1, 28, MASTER_TALK);
          }

          if (selectedGrade >= 3) currentState = ST_VF_GAME; // Reuse VF state for SD (Structure is same)
          else currentState = ST_VF_GAME; 
          
          currentQuestionIdx = 0;
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
        if (selectedGrade==1) q = g1_sm[realIdx];
        else if (selectedGrade==2) q = g2_sm[realIdx];
        else if (selectedGrade==3) q = g3_sm[realIdx];
        else q = g4_sm[realIdx];

        bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
        int folder = getGradeFolder();
        
        Serial.print("SM Q: "); Serial.println(q.fileNum);
        int nav = playFolderSafe(folder, q.fileNum, MASTER_TALK);
        
        if (nav == NAV_NEXT) { currentQuestionIdx++; return; } 
        if (nav == NAV_PREV) { if(currentQuestionIdx>0) currentQuestionIdx--; return; } 
        
        int userAns = waitAnswerLogic(q.correctAnswer);
        if (userAns == NAV_NEXT) { currentQuestionIdx++; return; } 
        if (userAns == NAV_PREV) { if(currentQuestionIdx>0) currentQuestionIdx--; return; } 
        
        if (userAns == q.correctAnswer) {
           playFeedback(true, isLast);
        } else {
           handleErrorFlow(isLast);
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
        if (selectedGrade==1) q = g1_vf[realIdx];
        else if (selectedGrade==2) q = g2_vf[realIdx];
        else if (selectedGrade==3) q = g3_sd[realIdx];
        else q = g4_sd[realIdx];

        bool isLast = (currentQuestionIdx == QUESTIONS_PER_ROUND - 1);
        int folder = getGradeFolder();
        
        Serial.print("VF/SD Q: "); Serial.println(q.fileNum);
        int nav = playFolderSafe(folder, q.fileNum, MASTER_TALK);
        
        if (nav == NAV_NEXT) { currentQuestionIdx++; return; } 
        if (nav == NAV_PREV) { if(currentQuestionIdx>0) currentQuestionIdx--; return; } 
        
        int userAns = waitAnswerLogic(q.correctAnswer);
        if (userAns == NAV_NEXT) { currentQuestionIdx++; return; } 
        if (userAns == NAV_PREV) { if(currentQuestionIdx>0) currentQuestionIdx--; return; } 
        
        if (userAns == q.correctAnswer) {
           playFeedback(true, isLast);
        } else {
           handleErrorFlow(isLast);
        }
        currentQuestionIdx++;
      }
      break;
      
    case ST_IDLE:
      break;
  }
}
