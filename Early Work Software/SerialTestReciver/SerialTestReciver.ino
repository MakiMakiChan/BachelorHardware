void setup() {
  Serial.begin(300);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  char x;
  if (Serial.available()) {
    x = Serial.read();
    if (x == 'H')
    {
      Serial.write('U');
      digitalWrite(13, HIGH);
    }
    if (x == 'p')
    {
      digitalWrite(13, LOW);
      Serial.write('P');
    }


  }

}
