#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

// DEFINICION ESTANDAR
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Pines actuales (que vamos a probar)
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {13, 12, 11, 10}; 

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  Serial.println("=== KEYPAD MAPPER ===");
  Serial.println("Por favor presiona las teclas EN ORDEN:");
  Serial.println("Fila 1: 1, 2, 3, A");
  Serial.println("Fila 2: 4, 5, 6, B");
  Serial.println("Fila 3: 7, 8, 9, C");
  Serial.println("Fila 4: *, 0, #, D");
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.print("Tecla Presionada: ");
    Serial.println(customKey);
  }
}
