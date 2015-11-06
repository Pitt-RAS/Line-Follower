void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
//test
  

  Serial.begin(9600);
}

int leftMotor = 2;
int rightMotor = 3;

int leftCutoff = 100;
int rightCutoff = 300;

void loop() {

  int left = analogRead(A0);
  int right = analogRead(A1);
  

  digitalWrite(rightMotor, HIGH);
  digitalWrite(leftMotor, HIGH);

  if(left > leftCutoff)
  {
    digitalWrite(leftMotor, LOW);
    Serial.println("NOT RIGHT");
  }
  
  if(right > rightCutoff)
  {
    digitalWrite(rightMotor, LOW);
    Serial.println("NOT LEFT");
  }

  Serial.println();

}
