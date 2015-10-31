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

**Analog Pins**
    
    A0           AIN1 (right motor +)
    A1           AIN2 (right motor -)
    A2           BIN1 (left  motor +)
    A3           BIN2 (left  motor -)

**Digital Pins**

    DP0 reserved for Arduino
    DP1 reserved for Arduino
    DP2          Sensor1 (IR)
    DP3          Sensor2
    DP4          Sensor3  
    DP5          Sensor4  
    DP6          Sensor5  
    DP7          Sensor6  
    DP8          Sensor7  
    DP9          Sensor8 
    DP10           PWM A (right motor PWM) @ 31250Hz
    DP11           PWM B (left  motor PWM) @ 31250Hz
    DP12          Servo signal




#H-Bridge-Pinout

    A01         right motor +
    A02         right motor -
    B01         left  motor +
    B02         right motor -
    VMOT        12v battery
    GND         Ground




#Servo-Pinout

    GPIO        Signal
    5v          V+
    GND         V-
