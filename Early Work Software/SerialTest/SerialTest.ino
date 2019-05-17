//Master 

void setup() {
 Serial.begin(300);
}

void loop() {
 delay(1000);
 Serial.write('H');
 delay(1000);
 Serial.write('p');
}
