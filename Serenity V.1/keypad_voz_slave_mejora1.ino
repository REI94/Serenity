// UNIVERSIDAD DE LOS ANDES
// MÉRIDA - VENEZUELA
// 17 de Mayo del 2024
// ESTUDIANTE: LAURA MATURANA

#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Wire.h>


const int ROWS = 4; // Número de filas del keypad
const int COLS = 4; // Número de columnas del keypad
const int buzzerPin = 3; // Pin del buzzer
int codigo;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};



byte rowPins[ROWS] = {9, 8, 7, 6};    // Conexiones de las filas del keypad a los pines digitales del Arduino
byte colPins[COLS] = {13, 12, 11, 10};    // Conexiones de las columnas del keypad a los pines digitales del Arduino

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

SoftwareSerial mySerial(2, 4); // RX, TX

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent);
  randomSeed(analogRead(1));

  pinMode(buzzerPin, OUTPUT); // Configura el pin del buzzer como salida


  myDFPlayer.begin(mySerial);
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(15); // Ajusta el volumen (0-30)
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL); // Establece el modo de ecualización
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); // Establece el dispositivo de salida como tarjeta SD
  myDFPlayer.enableDAC(); // Habilita el DAC (convertidor digital-analógico)

  codigo = 1;
  
  // 
  myDFPlayer.playFolder(1, 1);
  codigo = 4;
  delay(10000);
  //codigo = 5;
 
  myDFPlayer.stop();
  codigo = 1;
  myDFPlayer.playFolder(1, 2);
 
  delay(7000);
  codigo = 0;
  myDFPlayer.stop();

}

void loop() {
  char key = keypad.getKey();
  
  static char input[4]; // Almacenar el valor ingresado (hasta 3 dígitos + terminador nulo)
  static int index = 0; // Índice para rastrear la posición actual en el arreglo input
  
  if (key) {

    Serial.println("Introduzca un número");
    Serial.println(key);

    // tone(buzzerPin, 1000);
    // delay(100);
    // noTone(buzzerPin);

    if (key >= '0' && key <= '9' && index < 3) {
        tone(buzzerPin, 1000);
        delay(100);
        noTone(buzzerPin);
      // Si se presionó un dígito y aún hay espacio en el arreglo input
      input[index] = key;
      index++;
    } else if (key == '#' && index > 0) {
      // Si se presionó el símbolo "#" y se ingresó al menos un dígito
      input[index] = '\0'; // Agregar terminador nulo al final del arreglo input
      int value = atoi(input); // Convertir el arreglo input a un entero
      
      tone(buzzerPin, 1000);
      delay(100);
      tone(buzzerPin, 3000);
      delay(100);
      noTone(buzzerPin);

      Serial.println(value);
      // Realizar acciones en función del valor ingresado
      if (value > 80) {

        Serial.println("Acción para valores mayores a 80");
        codigo = 1;
        myDFPlayer.playFolder(1, 3);
        delay(28000);
        codigo = 0;
        myDFPlayer.stop();

        // tone(buzzerPin, 1000);
        // delay(100);
        // noTone(buzzerPin);
        
        Serial.println("Introduzca la opción");

        int num2 = 0;
        while (true) {
          char key = keypad.getKey();
          if (key >= '0' && key <= '9') {

            tone(buzzerPin, 1000);
            delay(100);
            noTone(buzzerPin);
            num2 = num2 * 10 + (key - '0');

          } else if (key == '#') {

            tone(buzzerPin, 1000);
            delay(100);
            tone(buzzerPin, 3000);
            delay(100);
            noTone(buzzerPin);

            break;
          }
        }
        
        // Realizar acciones basadas en el número ingresado
        Serial.print("Número ingresado: ");
        Serial.println(num2);

          //MÚSICA RELAJANTE
          if(num2 == 1){ 
            codigo = 1;
            myDFPlayer.playFolder(1,4);
            delay(6000);
            codigo = 0;
            //myDFPlayer.stop();
            int number = 0;
            number = random(1,3);

            Serial.println(number);
            if (number == 1 or number == 2){
              codigo = 4;
              myDFPlayer.playFolder(3, number);
              delay(240000);
              codigo = 0;

              codigo = 1;
              myDFPlayer.playFolder(5,1);
              delay(7100);
              codigo = 0;
              //myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(60000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(80000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
            }
            else if(number == 3 or number == 6){
              myDFPlayer.playFolder(3, number);
              delay(186000);

              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
            }
            else if(number == 4 or number == 7){
              myDFPlayer.playFolder(3, number);
              delay(120000);

              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
            }
            else if(number == 5){
              myDFPlayer.playFolder(3, number);
              delay(367000);

              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                //myDFPlayer.stop();
              }
            }
          } //SONIDOS DE LAS NATURALEZAS
          else if(num2 == 3){
            codigo = 1;
            myDFPlayer.playFolder(1,7);
            delay(6000);
            codigo = 0;
            myDFPlayer.stop();
            int number = random(1,3);
            Serial.println(number);

            if (number == 1 or number == 2 or number == 3){
              myDFPlayer.playFolder(2, number);
              delay(60000);

              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);

              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
            }
            else if(number == 5){
              myDFPlayer.playFolder(2, number);
              delay(78000);

              codigo = 1;
              myDFPlayer.playFolder(5,1);
              delay(7100);
              codigo = 0;
              myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
            }
            else if(number == 4){
              codigo = 1;
              myDFPlayer.playFolder(2, number);
              delay(145200);

              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              codigo = 0;

              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
            }
          } //RESPIRACIÓN GUIADA
          else if(num2 == 2){
            codigo = 1;
            myDFPlayer.playFolder(1,5);
            delay(9100);
            //myDFPlayer.stop();

            myDFPlayer.playFolder(4,1);
            delay(11000);
            codigo = 6;
            Serial.println(codigo);
            //delay(8000);
            //codigo = 6;
            //Serial.println(codigo);
            //delay(10000);
            //codigo = 5;
            // delay(12000);
            // codigo = 3;
            //delay(97000);
            delay(53800);
            codigo = 1;
            //myDFPlayer.stop();
            //codigo = 1;
            myDFPlayer.playFolder(5,1);
            delay(7100);
            myDFPlayer.stop();
            codigo = 0;

              int number2 = random(2,8);
              Serial.println(number2);

              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
          }
          // memset(input, 0, sizeof(input));
          // index = 0;
        
        }else {
        // Acción para valores menores o iguales a 80
        Serial.println("Acción para valores menores o iguales a 80");
        codigo = 1;
        myDFPlayer.playFolder(1, 6);
        delay(27000);
        codigo = 0;
        myDFPlayer.stop();

        Serial.println("Introduzca la opción");
        int num3 = 0;
    
        while (true) {
          char key = keypad.getKey();
          if (key >= '0' && key <= '9') {
            tone(buzzerPin, 1000);
            delay(100);
            noTone(buzzerPin);
            num3 = num3 * 10 + (key - '0');
          } else if (key == '#') {
            
            tone(buzzerPin, 1000);
            delay(100);
            tone(buzzerPin, 3000);
            delay(100);
            noTone(buzzerPin);
            break;
          }
        }
        // Realizar acciones basadas en el número ingresado
        Serial.print("Número ingresado: ");
        Serial.println(num3);

          //MUSICA RELAJANTE < 80
          if(num3 == 1){

            codigo = 1;
            myDFPlayer.playFolder(1,4);
            delay(6000);
            myDFPlayer.stop();
            codigo = 0;

            int number = random(1,3);
            Serial.println(number);

            if (number == 1 or number == 2){
              codigo = 1;
              myDFPlayer.playFolder(3, number);
              delay(240000);

              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              codigo = 0;

              int number2 = random(2,8);
              Serial.println(number2);

              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
              //myDFPlayer.stop();
            }
            else if(number == 3 or number == 6){
              myDFPlayer.playFolder(3, number);
              delay(186000);

              codigo = 1;
              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              codigo = 0;

              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
              //myDFPlayer.stop();
            }
            else if(number == 4 or number == 7){
              myDFPlayer.playFolder(3, number);
              delay(120000);

              codigo = 1;
              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              codigo = 0;

              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
              //myDFPlayer.stop();
            }
            else if(number == 5){
              myDFPlayer.playFolder(3, number);
              delay(366000);

              codigo = 1;
              myDFPlayer.playFolder(5,1);
              delay(7100);
              codigo = 0;
              myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
              //myDFPlayer.stop();
            }
          } // SONIDOS DE LA NATURALEZA < 60
          else if(num3 == 3){
            codigo = 1;
            myDFPlayer.playFolder(1,7);
            delay(6000);
            codigo = 0;
            myDFPlayer.stop();

            int number = random(1,3);
            Serial.println(number);

            if (number == 1 or number == 2 or number == 3){
              myDFPlayer.playFolder(2, number);
              delay(60000);

              codigo = 1;
              myDFPlayer.playFolder(5,1);
              delay(7100);
              codigo = 0;
              myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
              //myDFPlayer.stop();
            }
            else if(number == 5){
              myDFPlayer.playFolder(2, number);
              delay(78000);

              codigo = 1;
              myDFPlayer.playFolder(5,1);
              delay(7100);
              codigo = 0;
              myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
              //myDFPlayer.stop();
            }
            else if(number == 4){
              myDFPlayer.playFolder(2, number);
              delay(145200);

              codigo = 1;
              myDFPlayer.playFolder(5,1);
              delay(7100);
              myDFPlayer.stop();
              codigo = 0;

              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
              //myDFPlayer.stop();
            }
          } //RESPIRACION GUIADA < 60
          else if(num3 == 2){
            codigo = 1;
            myDFPlayer.playFolder(1,5);
            delay(9000);
            codigo = 0;
            //myDFPlayer.stop();

            codigo = 1;
            myDFPlayer.playFolder(4,1);

            delay(11000);
            codigo = 6;
            Serial.println(codigo);
            delay(53800);



            codigo = 1;
            myDFPlayer.playFolder(5,1);
            delay(7100);
            codigo = 0;
            myDFPlayer.stop();
              
              int number2 = random(2,8);
              Serial.println(number2);
              if (number2 == 5 or number2 == 6 or number2 == 8){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(6000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 4 or number2 == 7){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(8000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 3){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(15000);
                codigo = 0;
                //myDFPlayer.stop();
              }
              else if(number2 == 2){
                codigo = 1;
                myDFPlayer.playFolder(5, number2);
                delay(3100);
                codigo = 0;
                //myDFPlayer.stop();
              }
            //myDFPlayer.stop();
          }
          else if(num3 == 4){
            codigo = 1;
            myDFPlayer.playFolder(5,1);
            delay(7100);
            myDFPlayer.stop();
            codigo = 0;

            int number = random(2,8);
            Serial.println(number);
            if (number == 5 or number == 6 or number == 8){
              codigo = 1;
              myDFPlayer.playFolder(5, number);
              delay(6000);
              codigo = 0;
              //myDFPlayer.stop();
            }
            else if(number == 4 or number == 7){
              codigo = 1;
              myDFPlayer.playFolder(5, number);
              delay(8000);
              codigo = 0;
              //myDFPlayer.stop();
            }
            else if(number == 3){
              codigo = 1;
              myDFPlayer.playFolder(5, number);
              delay(15000);
              codigo = 0;
              //myDFPlayer.stop();
            }
            else if(number == 2){
              codigo = 1;
              myDFPlayer.playFolder(5, number);
              delay(3100);
              codigo = 0;
              //myDFPlayer.stop();
            }
          }

      }
      // Restablecer el arreglo input y el índice
      memset(input, 0, sizeof(input));
      index = 0;
    }
  }
}

void requestEvent() {
  Wire.write(codigo); // respond with message of 6 bytes
  // as expected by master
}

