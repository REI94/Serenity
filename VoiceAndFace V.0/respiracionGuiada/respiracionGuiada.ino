#include <EMIC2.h>

#include <SoftwareSerial.h>  // Needed by the EMIC2 library
#include <SD.h>  // Needed by the EMIC2 library, though not utilized in this example
#include "EMIC2.h"

#define RX_PIN 0  // Connect SOUT pin of the Emic 2 module to the RX pin
#define TX_PIN 1  // Connect SIN pin of the Emic 2 module to the TX pin

EMIC2 emic;  // Creates an instance of the EMIC2 library

void setup()
{
    // Initializes the EMIC2 instance
    // The library sets up a SoftwareSerial port
    // for the communication with the Emic 2 module
    emic.begin(RX_PIN, TX_PIN);
    

    emic.setVoice(8);  // Sets the voice (9 choices: 0 - 8)

    emic.setVolume(18);

    emic.setLanguage(2);
    
    emic.setRate(220);
}

void loop()
{
    // The speak method sends a message to the module
    // and then returns while the message plays on the module
    // emic.speak("Welcome to the /\\/\\__EMIC2\\/\\/ library. This is an Arduino library \
    //             that simplifies the use of the __Emic __2 __Text __to __Speech __Module");
    // emic.speak(":-)1 Let's now look at some ##pause of the features that the library offers");
    
    // emic.speak("Hola como estaas"); 
    // emic.speak("Mi nombre es Sereenity ."); 
    // emic.speak("sere su robot de ayudaa .");

    // emic.speak("He notado que su nivel cardiaco esta un poco alto");
    // //delay(500);
    // emic.speak("Permiitaaame ayudarle con unos ejercicios de respiiiraaaciooon");
    // //delay(1000);
    // emic.speak("ejercicio 1 ");
    // emic.speak("Respiraaaciooon cuatro siete ocho");
    // //delay(1000);
    // emic.speak("ejercicio 2 ");
    // emic.speak("respiraaaciooon abdominal");
    // //emic.speak("Respiraaaciooon compleeetaaa");
    // //delay(1000);
    // // emic.speak("ejercicio 3");
    // // emic.speak("Respiraaaciooon matinal");
    // //delay(1000);
    // emic.speak("que ejercicio desea practicar?");

    //respiracionAbdominal();
    respiracion478();

}

void respiracion478(){
  emic.speak("Le guiare a traves de la respiraaaciooon cuatro siete ocho");
  emic.speak("Pooongaseee en una posiciiooon comoda");
  emic.speak("Puede estar sentado o acostado");
  emic.speak("Para comenzar, coloque una mano sobre el abdomen");
  emic.speak("y su otra mano sobre el pecho");
  emic.speak("Respire profunda y lentamente deeesde el abdomen");
  emic.speak("y cuente hasta cuatro a medida que inhala");

  emic.speak("1");
  emic.speak("2");
  emic.speak("3");
  emic.speak("4");

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
  emic.speak("2");
  emic.speak("3");
  emic.speak("4");
  emic.speak("5");
  emic.speak("6");
  emic.speak("7");
  emic.speak("8");

  emic.speak("repita este ejercicio entre tres y siete veces");
  emic.speak("hasta que se sienta tranquilo");

  emic.speak("se siente usted mas tranquilo?");
  emic.speak("desea repetir el ejercicio?");
}

void respiracionAbdominal(){
  
  emic.speak("Le guiare a traves de la respiraaaciooon abdominal");
  emic.speak("Pooongaseee en una posicion comoda");
  emic.speak("Puede estar sentado o acostado");

  emic.speak("Coloque una mano sobre el abdomen justo debajo de las costillaaas y la otra mano sobre el peeechooo");
  emic.speak("Respire hondo por la nariz y deeeje que el abdomen le empuje la mano." );
  emic.speak("El peechooo no deberaaa moverse.");
  emic.speak("Exhale a traves de los labiooos fruuunciiidos como si estuviese silbando");
  emic.speak("Sienta como se huuunde la mano sobre su abdomen y utiliiicelaaa para expulsar todo el aire hacia afuera.");
  emic.speak("Realice este ejercicio de respiraciiiooon entre 3 y 10 veces. Tooomeeeseee su tiempo con cada respiraciiiooon.");
  emic.speak("Note como se siente al final del ejercicio");

  emic.speak("se siente usted mas tranquilo?");
  emic.speak("desea repetir el ejercicio?");
  

}