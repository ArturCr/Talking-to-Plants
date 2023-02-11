/*
Author: Artur Cruz  Date:10/02/2023
Project: Talking to Plants
Title: ESP32 Feather SPI Master To Communicate to OPENMV H7 Camera
Notes: 

*Idealy this should be turned into a Class for Abstraction but the speed 
of this method should be proven before further resources are spent
*Post Timeout from the Camera we get a long sequence of zeros.

*/

#define CHAR_BUF 128
#define SS 4
#define MISO 19 
#define MOSI 18
#define SCLK 5
#define delay_t 1 //This is keept at >1 due to serial & Cam limits (Works at 0.1![maybe more])

#define Version 3
/*Internal version control for sanity
Version 0-1 Use phase 0 in camera code
Version 2 onwards uses phase 1 in camera code

*Version 2 is capable of validating the synk bit,get message length(Serial out)
remove pading and then extra the message form the camera code example 
*/

void tick(){
  //Cycle the CLock onto the next instruction
  delay(delay_t);
  digitalWrite(SCLK,HIGH);
  delay(delay_t);
  digitalWrite(SCLK,LOW);
}

void tock(){ //OUTDATED used in version 0 and 1
  //Cycle the CLock onto the next instruction ofset by 1/2 cycle
  delay(delay_t);
  digitalWrite(SCLK,LOW);
  delay(delay_t);
  digitalWrite(SCLK,HIGH);
}

char getSPIchar(void){
  //Reads a Char of data On the SPI line
  uint8_t charbuilder=0;                            //holds the bits while building the char
  for(int b=0;b<7;b++){                             
    charbuilder=(charbuilder+digitalRead(MISO))<<1; //shifts in each bit
    tick();                                         //Apply changes
  }
  charbuilder=charbuilder+digitalRead(MISO);        //last bit added
  tick();                                           //Apply changes
  return charbuilder;
}

void array_printer(char * message,uint8_t message_length,uint8_t debugmode=0){
  //Serial.println();
  for(int b=0;b<message_length;b++){
    if (debugmode==1){
      Serial.print(message[b],HEX);
      Serial.print(' ');
    }else{
      Serial.print(message[b]);
    }
  }   
  Serial.println();
}

char din[CHAR_BUF];

void setup() {
  Serial.begin(115200);

  pinMode(SCLK, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SS, OUTPUT);
  //digitalWrite(SS,HIGH);

}

#if Version==3
  void loop() {
    
    digitalWrite(SS,HIGH);        //Deselect camera to Stop Communication
    tick();                       //Apply changes
    digitalWrite(SS,LOW);         //State intent to begin communication
    tick();                       //Apply changes
    
    if (getSPIchar()=='U'){                     //Test for synk character
      uint8_t message_length=(int)getSPIchar(); //Get message Length
      Serial.println(message_length);           //Print message length
      for(int a=0;a<3;a++){getSPIchar();}       //Remove paddding zeros
      for(int a=0;a<message_length;a++){        
        din[a]=getSPIchar();                    //Get message character
        //Serial.print(din[a]);                 //Print message character (out for speed tests)
      }
      array_printer(din,message_length);        //Print message
      //delay(100);//Must be added in speed test for Serial print
    }  
  }
#endif





//------------------Out-Dated------------------//
#if Version==2
  void loop() {
    
    digitalWrite(SS,HIGH);        //Deselect camera to Stop Communication
    tick();                       //Apply changes
    digitalWrite(SS,LOW);         //State intent to begin communication
    tick();                       //Apply changes
    
    if (getSPIchar()=='U'){                     //Test for synk character
      uint8_t message_length=(int)getSPIchar(); //Get message Length
      Serial.println(message_length);           //Print message length
      for(int a=0;a<3;a++){getSPIchar();}       //Remove paddding zeros
      for(int a=0;a<13;a++){                    //Get message
        din[a]=getSPIchar();                    //Get message character
        Serial.print(din[a]);                   //Print message character
      } 
    }  
  }
#endif

#if Version==1 //Depricated
  void loop() {
    
    digitalWrite(SS,HIGH);
    tick();
    tick();
    tick();
    tick();
    tick();
    digitalWrite(SS,LOW);
    digitalWrite(MOSI,HIGH);

    tock();
    if (getSPIchar()=='U'){
      Serial.println(getSPIchar(),DEC);
      Serial.println(getSPIchar(),HEX);
      Serial.println(getSPIchar(),HEX);
      Serial.println(getSPIchar(),HEX);
      }
    for(int a=0;a<13;a++){   
      din[a]=getSPIchar();
      Serial.print(din[a]);
    } 
  }
#endif

#if Version==0
  void loop() {
    
    digitalWrite(SS,HIGH);
    tick();
    tick();
    tick();
    tick();
    tick();
    digitalWrite(SS,LOW);
    digitalWrite(MOSI,HIGH);

    tock();
    for(int a=0;a<18;a++){
      uint8_t charbuilder=0;
      for(int b=0;b<7;b++){
        charbuilder=charbuilder+digitalRead(MISO);
        charbuilder=charbuilder<<1;      
        tock();  
      }
      charbuilder=charbuilder+digitalRead(MISO);
      din[a]=charbuilder;
      Serial.print(din[a]);
      //Serial.printf("MI:%u\n", digitalRead(MISO));
      tock();  
    } 
  }
#endif