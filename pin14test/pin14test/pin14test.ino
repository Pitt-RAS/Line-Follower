void setup() {
  // put your setup code here, to run once:
Serial.begin(9600)
}

void loop() {
  // put your main code here, to run repeatedly:
int pin = analogRead(A14);
Serial.println(pin);
delay(1);
}
