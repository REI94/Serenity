void setup() {
  Serial.begin(9600);
  Serial.println("=== PIN FINDER ===");
  Serial.println("Presiona una tecla y MANTENLA presionada.");
  Serial.println("Aparecera que par de pines se conectan.");
  Serial.println("Anota el par para cada tecla (ej: 1 -> 8-3).");
}

void loop() {
  // Pines a escanear (Ajusta si usas otros)
  // Usamos 2 a 12 (evitando 0, 1, 13 led)
  int pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  int numPins = 11;

  // Reset all to Input Pullup
  for(int i=0; i<numPins; i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }

  for(int i=0; i<numPins; i++) {
    // Drive one pin Low
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);

    // Check others
    for(int j=i+1; j<numPins; j++) {
      if(digitalRead(pins[j]) == LOW) {
        Serial.print("DETECTADO: Pin ");
        Serial.print(pins[i]);
        Serial.print(" - Pin ");
        Serial.println(pins[j]);
        delay(500); // Debounce visual
      }
    }

    // Restore
    pinMode(pins[i], INPUT_PULLUP);
  }
}
