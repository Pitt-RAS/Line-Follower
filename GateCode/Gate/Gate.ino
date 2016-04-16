#include <Time.h>
#include <stdlib.h>
#define MIN_VOLT_OFFSET 10

unsigned long stopWatch;
int startGate;
int finishGate;
int minVoltS;
int minVoltF;
unsigned long startTime;
unsigned long endTime;
unsigned long midTime;

//220Ohm LED, 1M Ohm Diode
void setup() {
  Serial.begin(9600);
  stopWatch=0;
  calibrate();
  //minVoltS=250;
  //minVoltF=250;
  finishGate=0;
  startGate=analogRead(A0);
  midTime=millis();
  while(startGate>minVoltS){
    startGate=analogRead(A0);
    if((midTime+30)<millis()){
      midTime=millis();
      Serial.println("READY");
    }
  }
}

void loop() {
  startGate=0;
  midTime=millis();

  while(startGate<minVoltS){
    startGate=analogRead(A0);
    if((midTime+30)<millis()){
      midTime=millis();
      Serial.println("READY ");
    }
  }

  stopWatch=0;
  //Serial.println("out start gate"+String(analogRead(A0))+" "+String(analogRead(A1)));
  finishGate=analogRead(A1);
  startTime=millis();
  midTime=millis()-startTime;
  Serial.println(String(midTime));

  while(finishGate>minVoltF){
    finishGate=analogRead(A1);
    stopWatch=millis()-startTime;
    if((midTime+30)<stopWatch){
      midTime=millis()-startTime;
      Serial.println(String(stopWatch));
    }
  }

  stopWatch=millis()-startTime;
  midTime=millis();
  while(startGate>minVoltS){
    if((midTime+30)<millis()){
      midTime=millis();
      Serial.println("Finish "+String(stopWatch));
    }
    startGate=analogRead(A0);
  }
}

void calibrate(){
  Serial.println("calibrating");
  long aveVolt1=0;
  long aveVolt2=0;
  for(int i=0;i<200;++i){
    aveVolt1=aveVolt1+analogRead(A0);
    aveVolt2=aveVolt2+analogRead(A1);
    delay(15);
  }
  minVoltS=(aveVolt1/200)- MIN_VOLT_OFFSET;
  Serial.print(String(minVoltS));
  minVoltF=(aveVolt2/200)- MIN_VOLT_OFFSET;
  Serial.println(" "+String(minVoltF));
}
