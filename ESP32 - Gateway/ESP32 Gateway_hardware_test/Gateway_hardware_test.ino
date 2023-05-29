/*
   Title: Gateway_hardware_test
   Author: Artur Cruz

   Summary:
   Declares LEDs by colour
   Blinnking Each LED for 1 secound a means to test connections and loop [requires visual inspection]

   To do:
   * Add a Uart test
   * Have both gateway ESP32s share a common code differenciated by input GPIO Pin 
*/

//--------- Global variable declaration -----------//

// Defines the LED pin locations
const int led_r=22; // Red LED
const int led_y=23; // Yellow LED
const int led_g=14; // Green LED
const int empty=32; // Avaliable for an LED / Button / Switch

//------------- Functions ----------------//

void Blink(int pin, int ms){
  digitalWrite(pin, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(ms);               // wait for a second
  digitalWrite(pin, LOW);  // turn the LED off (LOW is the voltage level)
}

//------------- Set UP ----------------//

// the setup function runs once when you press reset or power the board
void setup() {  
   pinMode(led_r, OUTPUT); //Configure the Red LED ...
   pinMode(led_y, OUTPUT);
   pinMode(led_g, OUTPUT);
   //pinMode(empty, OUTPUT); // Avaliable for an LED / Button / Switch
}

//------------- Main loop ----------------//

// the loop function runs over and over again forever
void loop() {
   Blink(led_r,1000); //Blink the Red LED for 1000 ms ...
   Blink(led_y,1000);
   Blink(led_g,1000);
}
