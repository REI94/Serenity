#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Wire.h>

int codigo;

SoftwareSerial mySerial(2, 4);  // RX, TX

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  Wire.begin(8);  // join i2c bus with address #8
  Wire.onRequest(requestEvent);
  randomSeed(analogRead(1));

  myDFPlayer.begin(mySerial);
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(30);                        // Ajusta el volumen (0-30)
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);            // Establece el modo de ecualización
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);  // Establece el dispositivo de salida como tarjeta SD
  myDFPlayer.enableDAC();                       // Habilita el DAC (convertidor digital-analógico)

  // codigo = 1;

  //
  myDFPlayer.playFolder(1, 1);
  // codigo = 4;
  delay(4000);
  //codigo = 5;

  myDFPlayer.stop();
  // codigo = 1;
  myDFPlayer.playFolder(1, 2);

  delay(3000);
  // codigo = 0;
  myDFPlayer.stop();
}

void loop() {
  
  codigo = 0;
  myDFPlayer.playFolder(1, 3);
  codigo = 1;
  delay(4000);

  myDFPlayer.playFolder(1, 4);
  codigo = 2;
  delay(4000);
  //codigo = 0;

  myDFPlayer.playFolder(1, 5);
  delay(3000);
  
  codigo = 3;

  myDFPlayer.playFolder(1, 6);
  delay(8000);
  codigo = 2;
  codigo = 4;

  myDFPlayer.playFolder(1, 7);
  codigo = 4;
  delay(8000);
  codigo = 2;
  
  myDFPlayer.playFolder(1, 8);
  delay(4000);
  
  codigo = 5;
  myDFPlayer.playFolder(3, 1);
  codigo = 5;
  delay(12000);
  codigo = 2;

  myDFPlayer.playFolder(1, 9);
  delay(4000);

  myDFPlayer.playFolder(2, 1);
  delay(13000);

  myDFPlayer.playFolder(2, 2);
  codigo = 6;
  delay(5000);
  
  myDFPlayer.playFolder(2, 3);
  //codigo = 6;
  delay(3000);

  myDFPlayer.playFolder(2, 4);
  delay(5000);
  codigo = 2;

  myDFPlayer.playFolder(2, 5);
  delay(3000);

  myDFPlayer.playFolder(2, 6);
  codigo = 7;
  delay(5000);

  myDFPlayer.playFolder(2, 7);
  delay(4000);

  myDFPlayer.playFolder(2, 8);
  codigo = 8;
  delay(5000);

  myDFPlayer.playFolder(2, 9);
  
  delay(2000);

  myDFPlayer.playFolder(4, 1);
  codigo = 9;
  delay(4000);

  myDFPlayer.playFolder(4, 2);
  codigo = 10;
  delay(4000);

  codigo = 2;
  myDFPlayer.playFolder(4, 3);
  delay(6000);

  myDFPlayer.playFolder(4, 4);
  delay(14000);

  //break;
}

void requestEvent() {
  Wire.write(codigo);  // respond with message of 6 bytes
  // as expected by master
}
