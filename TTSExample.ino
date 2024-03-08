// This example exposes some of the functionality of the EMIC2 library

#include <SoftwareSerial.h>  // Needed by the EMIC2 library
#include <SD.h>  // Needed by the EMIC2 library, though not utilized in this example
#include "EMIC2.h"

// Read here: http://arduino.cc/en/Reference/SoftwareSerial
// for the available pins on your board
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
}

void playGuidedBreathing() {
  // Play a guided breathing message
  emic.speak("A continuacion iniciaremos una serie de ejercicios de respiracion guiada");
  delay(1000);

  emic.speak("Sigue mis instrucciones por favor");
  delay(1000);

  emic.speak("Inhala profundamente y cuenta hasta cuatro");
  delay(5000);

  emic.speak("Reten la respiracion durante cuatro segundos");
  delay(5000);

  emic.speak("Exhala lentamente durante seis segundos");
  delay(5000);

  emic.speak("Repite este ciclo durante unos minutos");
}

void playTipsForStress() {
  // Play tips to relieve stress
  emic.speak("Sufres de estres");
  delay(2000);

  emic.speak("Entonces sigue estos consejos que te ayudaran a combatirlo");
  delay(2000);

  emic.speak("Respira profundamente y relajate");
  delay(2000);

  emic.speak("Tomate un momento para meditar o practicar la atencion plena");
  delay(2000);

  emic.speak("Haz ejercicio regularmente para liberar endorfinas");
  delay(2000);

  emic.speak("Escucha musica relajante para calmar la mente");
  delay(2000);

  emic.speak("Habla con alguien cercano sobre tus sentimientos");
  delay(2000);

  emic.speak("Desconectate de las pantallas y descansa");
  delay(2000);

  emic.speak("Practica la gratitud y enfocate en lo positivo");
  delay(2000);
}

void playImproveSelfEsteem() {
  // Tips to improve self-esteem
  emic.speak("Si quieres mejorar tu autoestima ten en cuenta lo siguiente");
  delay(2000);

  emic.speak("Identifica tus fortalezas y habilidades unicas");
  delay(2000);

  emic.speak("Practica el autocuidado y la autorreflexion");
  delay(2000);

  emic.speak("Aprende a gestionar tus emociones y pensamientos negativos");
  delay(2000);

  emic.speak("Establece metas realistas y alcanzables");
  delay(2000);

  emic.speak("Reconoce tus logros y celebra tus exitos");
  delay(1000);
  emic.speak("por pequeños que sean");
  delay(2000);

  emic.speak("Evita las comparaciones con los demas");
  delay(1000);
  emic.speak("cada persona tiene su propio camino");
  delay(2000);

  emic.speak("No te dejes llevar por razonamientos sociales o expectativas externas");
  delay(2000);

  emic.speak("No trates de adivinar lo que piensan de ti");
  delay(1000);
  emic.speak("concentrate en tu propia opinion");
  delay(2000);
}


void loop()
{
    // The speak method sends a message to the module
    // and then returns while the message plays on the module
    // emic.speak("Welcome to the /\\/\\__EMIC2\\/\\/ library. This is an Arduino library \
    //             that simplifies the use of the __Emic __2 __Text __to __Speech __Module");
    // emic.speak(":-)1 Let's now look at some ##pause of the features that the library offers");
    
    guidedBreathing();

    playTipsForStress();

    playImproveSelfEsteem();

    delay(2500);
    ~emic;  // Pauses playback
    delay(500);
    ~emic;  // Unpauses playback when paused
    
  //   // Sets the speaking rate to 100words/minute (range: [75,600])
  //   // and then resets it to 200words/minute
  //   emic.setRate(100);
  //   emic.speak("You can set and reset the speaking rate");
  //   emic.resetRate();
    
  //   // Increases the speaking rate by 150words/minute
  //   // and then decreases it by 150words/minute
  //   emic >> 150;
  //   emic.speak("You can change the speaking rate by the use of operators");
  //   emic << 150;
    
  //   // Sets the volume level to 15dB (range: [-48,18])
  //   // and then resets it to 0dB
  // //  emic.setVolume(18);
  // //  emic.speak("You can set and reset the volume level");
  // //  emic.resetVolume();
    
  //   // Lowers the volume level by 10dB
  //   // and then raises it by 10dB
  //   // emic -= 9;
  //   // --emic;
  //   // emic.speak("You can change the volume level by the use of operators");
  //   // emic += 9;
  //   // ++emic;
    
  //   emic.speak("You can provide any kind of data");
  //   emic.speak("Even doubles");
  //   emic.speak(3.14);  // It accepts various alphanumeric or numeric data types
    
  //   emic.speak(":-)0 Let's now sing a song");
  //   emic.speakDemo(1);
  //   delay(2000);
  //   !emic;  // Stops playback
    
  //   emic.speak("/\\/\\__DON'T EVEN THINK ABOUT IT");
    // Some of the texts above make use of special control symbols for dynamic changes of
    // pitch (/\ - increase, \/ - decrease), rate (>> - increase, << - decrease),
    // emphasis (__ - emphasize next word), whispering (## - whisper next word),
    // and voice (:-)x, x:[0,8])
    
    while (true) ;
}