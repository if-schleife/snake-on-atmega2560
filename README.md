# snake-on-atmega2560
The game snake implemented with a 

single linked list 

on an 
   atmega2560 with a 
   KY-023 Joystick Modul and a 
   1,8 Zoll SPI TFT-Display(ST7735) 128x160 V1.1


Dont forget the include the Libaries used in that code.


Disclamer:
This is a test project with the purpose of practice, the sample code is without any warranties. Use at your own risk!


Wiring:

Display                             MEGA 2560                                       Joystick      
GND         -->                      GND                        <--                   ground
VCC         -->                      5V                         <--                   +5v
                                     A10                        <--                   VRX
                                     A11                        <--                   VRY
CS         -->                       PWM 10
RESET      -->                       PWM 9
A0         -->                       PWM 8
SDA        -->                      Digital 51
SCK        -->                      Digital 52
LED        -->                      +3.3V
