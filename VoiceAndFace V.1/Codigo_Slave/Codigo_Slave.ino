#include <EMIC2.h>
#include <SoftwareSerial.h>  // Needed by the EMIC2 library
#include <SD.h>  // Needed by the EMIC2 library, though not utilized in this example
#include "EMIC2.h"

#include <Wire.h>

#define RX_PIN 3  // Connect SOUT pin of the Emic 2 module to the RX pin
#define TX_PIN 4  // Connect SIN pin of the Emic 2 module to the TX pin


EMIC2 emic;  // Creates an instance of the EMIC2 library

int sensorPin = A0; // Pin analógico al que está conectado el sensor KY-039
unsigned long inicio;
unsigned long tiempoTranscurrido;
unsigned long minuto = 60000; // 1 minuto en milisegundos
unsigned long quince = 15000;
int pulso;
int umbral = 990; // Umbral ajustado para detectar el pulso de manera más sensible
int umbral2 = 1011;
int pulsaciones = 0; // Contador de pulsaciones
int lastPulse = 0; // Último pulso registrado
bool cuentaPulso = false; // Variable para controlar la cuenta de pulsos

int codigo;

void setup() {
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  
  inicio = millis(); // Guardar el tiempo de inicio
  Serial.println("Iniciando medición del pulso durante 1 minuto...");

  emic.begin(RX_PIN, TX_PIN);
  emic.setVoice(8);  // Sets the voice (9 choices: 0 - 8)
  emic.setVolume(10);
  emic.setLanguage(2);
  emic.setRate(220);
}

void loop() {
  tiempoTranscurrido = millis() - inicio; // Calcular el tiempo transcurrido en milisegundos

  if (tiempoTranscurrido >= quince) {
    float bpm = ((float)pulsaciones) * 0.4; // Ajustar el factor de escala para reflejar mediciones más precisas
    Serial.print("Pulsaciones por minuto (bpm): ");
    Serial.println(bpm);
    Serial.println("Medición completada");
    if(bpm >= 70){
      codigo = 1;

      emic.speak("Le guiare, a traves de la respiraaaciooon, cuatro, siete, ocho,");
      emic.speak("Pooongaseee en una posiciiooon comoda");
      emic.speak("Puede estar sentado o acostado");
      emic.speak("Para comenzar, coloque una mano sobre el abdomen");
      emic.speak("y su otra mano sobre el pecho");
      emic.speak("Respire profunda y lentamente deeesde el abdomen");
      emic.speak("y cuente hasta cuatro a medida que inhala");

      codigo = 2;
      emic.speak("1");
      emic.speak("2");
      emic.speak("3");
      emic.speak("4");
      codigo = 1;
      emic.speak("contenga la respiraaaciooon y cuente en silencio del uno al siete");

      emic.speak("1");
      emic.speak("2");
      emic.speak("3");
      emic.speak("4");
      emic.speak("5");
      emic.speak("6");
      emic.speak("7");

      emic.speak("Exhale por completo a medida que cuenta en silencio del uno al ocho");
      emic.speak("Intente sacar todo el aire de los pulmones para cuando llegue a ocho");

      emic.speak("1");
      codigo = 3;
      emic.speak("2");
      emic.speak("3");
      emic.speak("4");
      emic.speak("5");
      emic.speak("6");
      emic.speak("7");
      emic.speak("8");
      codigo = 1;
      emic.speak("repita este ejercicio entre tres y siete veces");
      emic.speak("hasta que se sienta tranquilo");

      emic.speak("se siente usted mas tranquilo?");
      // emic.speak("desea repetir el ejercicio?");
      // codigo = 0;

    }
    while (true) {} // Detener el programa una vez que haya pasado 1 minuto
  }

  pulso = analogRead(sensorPin); // Leer el valor del sensor

  if (pulso > umbral && pulso < umbral2) {
    pulsaciones++; // Incrementar el contador de pulsaciones
    cuentaPulso = true; // Indicar que se está contando un pulso
  }
  if (pulso < umbral) {
    cuentaPulso = false; // Indicar que se ha dejado de contar un pulso
  }

  Serial.print("Pulso: ");
  Serial.print(pulso);
  Serial.print("   Pulsaciones acumuladas: ");
  Serial.println(pulsaciones);

  delay(10); // Pequeña pausa para estabilizar la lectura

}

void requestEvent() {
  Wire.write(codigo); // respond with message of 6 bytes
  // as expected by master
}