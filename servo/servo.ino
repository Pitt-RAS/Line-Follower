#include <Servo.h>

Servo s1;

void setup()
{
        s1.attach(3);
        Serial.begin(9600);
}
/*#define DELAY 20*/
#define MAX 170
#define MIN 40
#define INC 10
void loop()
{
        for(int DELAY = 20; DELAY > 0; DELAY--){
                for (int i = MIN; i <= MAX; i+= INC) {
                        s1.write(i);
                        delay(DELAY);
                }
                for (int i = MAX; i >= MIN; i-= INC) {
                        s1.write(i);
                        delay(DELAY);
                }
                if(DELAY == 1) DELAY++;
        }
}
