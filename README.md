#RAS-Line-Follower

University of Pittsburgh Robot and Automation Society's Line-Follower

    TO-DO: add sensor location to pin-out
           add Sensor/H-bridge/motor/arduino specs
           add SOLIDWORKS/CAD/3D-Printer files
           add license


**Part-List**

    Board:      Arduino Uno
    H-bridge:   TB6612FNG
    Motors:
    Battery:
    IR-sensors:


**Arduino Pin-out**


**Arduino-GPIO/7v battery**   **Element**

     GND         Ground
     VCC         5v

*Analog Pins

  1. A0           AIN1 (right motor +)
  2. A1           AIN2 (right motor -)
  3. A2           BIN1 (left  motor +)
  4. A3           BIN2 (left  motor -)

*Digital Pins

    0. DP0 reserved for Arduino
    1. DP1 reserved for Arduino
    2. DP2          Sensor1 (IR)
    3. DP3          Sensor2
    4. DP4          Sensor3  
    5. DP5          Sensor4  
    6. DP6          Sensor5  
    7. DP7          Sensor6  
    8. DP8          Sensor7  
    9. DP9          Sensor8 
   10. DP10           PWM A (right motor PWM) @ 31250Hz
   11. DP11           PWM B (left  motor PWM) @ 31250Hz
   12. DP12          Servo signal




**H-Bridge-Pinout**

    1. A01         right motor +
    2. A02         right motor -
    3. B01         left  motor +
    4. B02         right motor -
    5. VMOT        12v battery
    6. GND         Ground




**Servo-Pinout**

    1. GPIO        Signal
    2. 5v          V+
    3. GND         V-
