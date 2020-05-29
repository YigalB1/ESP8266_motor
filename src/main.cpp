#include <Arduino.h>


 /*
 * LED blink program testing  ESP8266
 * blinks ON, OFF, ON, OFF, then long ON, OFF
 * 
 * Watch video instruction : https://youtu.be/4QpOErbx0nM or https://youtu.be/nCMdvscEipo
 * Written by Ahmad Shamshiri on Aug 01, 2019
 * at 17:42 in Ajax, Ontario, Canada
 * for Robojax.com
 * 
 * Get this code and other Arduono codes from Robojax.com
If you found this tutorial helpful, please support me so I can continue creating 
content like this. You can support me on Patreon http://robojax.com/L/?id=63
or make donation using PayPal http://robojax.com/L/?id=64
*/


int LEDpin = LED_BUILTIN;
void setup() {
  pinMode(LEDpin, OUTPUT);     // Initialize the LEDpin pin as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LEDpin, LOW);// Turns the blue LED ON  (see video why)
  delay(300);// Wait for 300 milliseconds
  digitalWrite(LEDpin, HIGH);// Turns the blue LED OFF  (see video why)
  delay(300);// Wait for 300 milliseconds  
  

  digitalWrite(LEDpin, LOW);
  delay(300);
  digitalWrite(LEDpin, HIGH);
  delay(300); 

  digitalWrite(LEDpin, LOW);
  delay(1000);// Wait for a second (keep it ON)
  digitalWrite(LEDpin, HIGH);
  delay(1000); // Wait for a second (keep it OFF)

}