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


#Arduino Pin-out


**Arduino-GPIO/7v battery**   **Element**

     GND         Ground
     VCC         5v

**Pins**

    0          reserved for Arduino
    1          reserved for Arduino
    
    5         PWM A (right motor PWM) @ 31250Hz
    6         PWM B (left  motor PWM) @ 31250Hz
    
    7          right motor -
    8          right motor +
    9          left  motor +
    10         left  motor -
    12         Servo signal
    
    A0          Sensor1 (IR)
    A1          Sensor2
    A2          Sensor3  
    A3          Sensor4  
    A4          Sensor5  
    A5          Sensor6  
    A6          Sensor7  
    A7          Sensor8




#H-Bridge-Pinout

    A01         right motor +
    A02         right motor -
    B01         left  motor +
    B02         left  motor -
    VMOT        12v battery
    GND         Ground




#Servo-Pinout

    GPIO        Signal
    5v          V+
    GND         V-
