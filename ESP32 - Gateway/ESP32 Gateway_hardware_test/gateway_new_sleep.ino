#include "driver/gpio.h"
#include "painlessMesh.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include <string.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <stdio.h>
#include <stdbool.h>

HardwareSerial SerialPort(2); // use UART2

#define size 7000

const char* ssid = "Justin";
const char* password = "justintan";

//const char* ssid = "Sam’s IPhone";
//const char* password = "a1b2c3esp32";

const char* UARTRecieved = "GIF89a/x01/x00/x01/x00/x00/xff/x00,/x00/x00/x00/x00/x01/x00/x01/x00/x00/x02/x00;";
//const char* UARTRecieved = "1";
HTTPClient http;
//WiFiClient client;

/*
String host = "http://192.168.172.239";
//String host = "localhost/website";
String file = "/website/index.php";
String URL = host + file;
*/

String host = "https://talkingtoplants.online";
String file = "/insertData2.php";
String URL = host + file;
String file2 = "/GetRequest2.php?api_key=CvcCKjGnBYAQoEAWtNGb6BFAdsWATKZwdNip8LVunjdsbQro1VDpVHnwXZAs8Sog";
String URL2 = host + file2;
String ApiKey = "CvcCKjGnBYAQoEAWtNGb6BFAdsWATKZwdNip8LVunjdsbQro1VDpVHnwXZAs8Sog";


// Defines the LED pin locations
const int led_r=22; // Red LED
const int led_y=23; // Yellow LED
const int led_g=14; // Green LED
const int empty=32; // Avaliable for an LED / Button / Switch

void Blink(int pin, int ms){
  digitalWrite(pin, HIGH); // turn the LED on (HIGH is the voltage level)
  //delay(ms);               // wait for a second
  //digitalWrite(pin, LOW);  // turn the LED off (LOW is the voltage level)
}
void Off (int pin){
  digitalWrite(pin, LOW); 
}


bool flag = false; 
bool flag2 = true;
bool flag3 = false;
bool start = false;
uint8_t imageNum = 0;
uint8_t segmentNum = 0;
uint8_t totalSegmentNum = 0;
uint8_t runNum = 0;
uint8_t period = 0;
unsigned long number = 0;
uint8_t buff[7000];
char msg[size];
int bufferPoint = 0;
int nodeID = 123;
int period1 ;
int period2 ;
int period3 ;
uint8_t numImage = 2;
uint8_t freqCount = 0;
int timeDiff ;
uint8_t headerSize = 9;
// tables arrays for lookup fast
static bool __TABLE_INIT= false;
static unsigned int TABLE_EV[256];


unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

// input is start of char array address & n == number of bytes
unsigned int CRC32_Table(unsigned char  * bytes, int n)
{
    const unsigned int polynomial = 0x04C11DB7; /* divisor is 32bit */
    unsigned int crc = 0xffffffff; /* CRC value is 32bit */
    unsigned char b;
    unsigned char bk;

    if (__TABLE_INIT) {
        // nothing to do - follow by lookup
    } 
    else {
        // init all tables first 
            for (int j = 0; j < 256; j++){
             b = j; crc = 0;
                     crc ^= (unsigned int)(b << 24);  //move byte into MSB of 32bit CRC 
                     for (int k = 0; k < 8; k++)
                     {
                         if ((crc & 0x80000000) != 0)  //test for MSB = bit 31 
                         {
                             crc = (unsigned int)((crc << 1) ^ polynomial);
                         }
                         else
                         {
                             crc <<= 1;
                         }
                     }
             // now add value to table
             TABLE_EV[j] = crc;
        } // j 0-255
            __TABLE_INIT = true;
    }
    // start calculations
    crc = 0xffffffff; /* CRC value is 32bit */

    for (int j=0; j<n; j++)
    {
        b = bytes[j];
        b = reverse(b); bk = b;
        b = b ^ ((crc>>24) & 0x0ff);
        crc = (crc<<8) ^ TABLE_EV[b]; 
        //printf("2..Computing for %x %x is %x\n",bk,b,crc);

    }

    crc = crc ^ 0xffffffff;
    // NOW REVERSE COMPLETE 32 bit WORD BIT BY BIT 
    crc = 
        reverse((crc & 0xff000000)>>24)   + 
    (reverse((crc & 0x00ff0000)>>16)<<8)   + 
    (reverse((crc & 0x0000ff00)>>8)<<16)   + 
    (reverse((crc & 0x000000ff))<<24) ;

    return crc;
}

void sendMessage(){
  // can be used to add functionality
  // generate msg
  flag = true;
  Serial.println(bufferPoint);
  // read buffer to update ID number & segment number
  // 1st 4 bytes == nodeID
  // 5th = imageNum
  // 6th = segment Num
  // 7th = totalSegNum
  // 8th = Period
  // 9th = runNum
  char byte1 = buff[0];
  char byte2 = buff[1];
  char byte3 = buff[2];
  char byte4 = buff[3];
  number = (static_cast<unsigned long>(byte1) << 24) |
                           (static_cast<unsigned long>(byte2) << 16) |
                           (static_cast<unsigned long>(byte3) << 8) |
                           static_cast<unsigned long>(byte4);
  imageNum = buff[4];
  segmentNum = buff[5];
  totalSegmentNum = buff[6];
  period = buff[7];
  runNum = buff[8];

  //strncpy(msg, buff, bufferPoint); // copy first ith elements from buff into msg
  // set msg to be null terminator
  //buff[bufferPoint] = '\0';
  msg[0] = 0;
  // start from 4th index => 1&2 are image number , 3&4 are segment number
  //sprintf(msg+strlen(msg),"0x");
  for (int i = headerSize; i < bufferPoint; i++){
    sprintf(msg + strlen(msg), "%02.2X", buff[i]);
  }
  bufferPoint = 0;
  Off(led_r);
  Off(led_y);
  Off(led_g);
  // change LED depending on period
  if ((period & (1 << 0)) != 0){
    // blink red
    Blink(led_r,10);
  }
  if ((period & (1 << 1)) != 0){
    // blink Yellow
    Blink(led_y,10);
  }
  if ((period & (1 << 2)) != 0){
    // blink Green
    Blink(led_g,10);
  }
};

void setup() {
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8E1, 16, 17); 
  // 16 is RX
  // 17 is TX
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000); // delay 1 sec
    Serial.print(WiFi.status());
    Serial.println(" ...");
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  pinMode(led_r, OUTPUT); //Configure the Red LED ...
  pinMode(led_y, OUTPUT);
  pinMode(led_g, OUTPUT);
}

void loop() {
  //Check WiFi connection case 2 
  if (flag){
    if(WiFi.status()== WL_CONNECTED){
      // Start Connection to Server

      bool httpBegin = http.begin(URL);

      int httpResponseCode = 0;
      String httpPayloadResponse = "";

      // Check HTTP connection
      if (httpBegin) {
        Serial.println("HTTP Connected");
        // while HTTP Response is wrong => we repeat 3 times
        int httpCounter = 0;
        while (httpResponseCode != 200 ){
          if ( httpCounter >= 3){
            break;
          }
          //String ImageData = String(UARTRecieved);
          String ImageData = String(msg);
          Serial.println("this is packet data");
          Serial.println(number);
          Serial.println(runNum);
          Serial.println(imageNum);
          Serial.println(segmentNum);
          Serial.println(totalSegmentNum);
          Serial.println(period);
          Serial.println(ImageData);
          
          //String payload = "ID=" + String(ID) + "&ImageData=" + ImageData;
          //String payload = "api_key=" + ApiKey + "&ID=" + String(ID) + "&ImageData=" + ImageData;
          //http.addHeader("Content-Type", "application/x-www-form-urlencoded");

          String payload = "api_key=" + ApiKey + "&NodeID=" + String(number) + "&RunNumber=" + String(runNum) + "&ImageNumber=" + String(imageNum) + "&SegmentNumber=" + String(segmentNum) + "&TotalSegmentNumber="+ String(totalSegmentNum)+"&Period="+String(period)+ "&Data=" + String(ImageData);
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");

          // Send message, get response code and message
          httpResponseCode = http.sendRequest("POST", payload);
          httpPayloadResponse = http.getString();
          httpCounter +=1;
        }
        // End connection
        http.end();
      }
      else {
        Serial.println("HTTP Connection Unsuccessful");
      }
      // Print response code and message
      Serial.println("");
      Serial.println("Data Sent, Response Code: ");
      Serial.println(httpResponseCode);
      Serial.println("Payload Response: ");
      Serial.println(httpPayloadResponse);


      Serial.println("Check values");
      if (imageNum == numImage){
        // one frequency run is done
        freqCount +=1;
      }
      if (imageNum == numImage && segmentNum == totalSegmentNum && freqCount == 3){
        Serial.println("Getting new Freq values");
        flag3 = true;

        // update Period through GET request
        // new server name => 
        bool httpBegin2 = http.begin(URL2);

        int httpResponseCode2 = 0;
        String httpPayloadResponse2 = "";
        // Check HTTP connection
        if (httpBegin2) {
          Serial.println("HTTP GET Connected");
          // while HTTP Response is wrong => we repeat 3 times
          int httpCounter2 = 0;
          while (httpResponseCode2 != 200 ){
            if ( httpCounter2 >= 3){
              break;
            }

            httpResponseCode2 = http.sendRequest("GET");
            httpPayloadResponse2 = http.getString();
            Serial.println(httpPayloadResponse2);
            httpCounter2+=1;
          }
          DynamicJsonDocument doc(1000);
          // Deserialize the JSON string
          DeserializationError error = deserializeJson(doc, httpPayloadResponse2);
          if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
          }
          // Access each individual periods
          period1 = doc["Frequency1"].as<int>();
          period2 = doc["Frequency2"].as<int>();
          period3 = doc["Frequency3"].as<int>();
          numImage = doc["NumImage"].as<uint8_t>();
          timeDiff = doc["timeDiff"].as<int>();
          // Print the frequencies
          Serial.print("Period 1: ");
          Serial.println(period1);
          Serial.print("Period 2: ");
          Serial.println(period2);
          Serial.print("Period 3: ");
          Serial.println(period3);
          Serial.print("Run Image Num: ");
          Serial.println(numImage);
          Serial.print("Sleep Time: ");
          Serial.println(timeDiff);

          // End connection
          http.end();
        }
        else {
          Serial.println("HTTP Connection Unsuccessful");
        }
        // Print response code and message
        Serial.println("");
        Serial.println("Period Received, Response Code: ");
        Serial.println(httpResponseCode2);
        Serial.println("Payload Response: ");
        Serial.println(httpPayloadResponse2 );
      }
    }
    else {
      Serial.println("WiFi Not Connected");
    }
    
    flag = false;
    start = false;
    // send back signal to uart => to start next cycle
    
    if(flag3){
      flag3 = false;
      Serial.println("Sending Period Values");
      SerialPort.write("\n");
      SerialPort.write(period1);
      SerialPort.write(period2);
      SerialPort.write(period3);
      SerialPort.write(numImage);
      // sending all 4 bytes of timediff
      char byte1 = (timeDiff >> 24) & 0xFF;
      char byte2 = (timeDiff >> 16) & 0xFF;
      char byte3 = (timeDiff >> 8) & 0xFF;
      char byte4 = timeDiff & 0xFF;
      SerialPort.write(byte1);
      SerialPort.write(byte2);
      SerialPort.write(byte3);
      SerialPort.write(byte4);
      //SerialPort.write(timeDiff);
    }
    else{
      Serial.println("Sending backPropagation Message");
      SerialPort.write("\r");
    }
  }
  // case 2 => 
  if (!flag){
    //Serial.println("Waiting for Bytes");
    if (flag2){
      Serial.println("checking UART");
      flag2= false;
    }
    
    if (SerialPort.available()){
      
      uint8_t inByte = SerialPort.read();
      //Serial.println(inByte);
      
      // carriage return character (\r) 
      if (inByte == '\r'&& start == false){
        // start of sending
        Serial.println("start");
        bufferPoint = 0;
        start = true;
      }
      // newline character (\n) 
      else if (inByte == '\r' && start==true){
        // end of sending packet send message funciton
        // check next 2 bytes for '\r'
        Serial.println("Checking");
        //Serial.println(inByte);
        /*
        uint8_t inByte2 = SerialPort.read();
        Serial.println(inByte2);
        uint8_t inByte3 = SerialPort.read();
        Serial.println(inByte3);
        uint8_t inByte4 = SerialPort.read();
        Serial.println(inByte4);
        */
        //uint8_t inByte3 = SerialPort.read();
        //delay(100);
        // can also try while buffer is empty
        // check byte 2
        while (!SerialPort.available()){
        }
        uint8_t inByte2 = SerialPort.read();
        //Serial.println(inByte2);
        // check byte 3
        while (!SerialPort.available()){
        }
        uint8_t inByte3 = SerialPort.read();
        //Serial.println(inByte3);

        // check byte 4
        while (!SerialPort.available()){
        }
        uint8_t inByte4 = SerialPort.read();
        //Serial.println(inByte4);

        if (inByte2 == '\n' && inByte3 == '\r' && inByte4 == '\n'){
          Serial.println("Done Segment");
          sendMessage();          
        }
        else{
          // save all 4 bytes into buffer and move forward
          char charByte = (char)inByte;
          buff[bufferPoint] = charByte;
          bufferPoint+=1;
          char charByte2 = (char)inByte2;
          buff[bufferPoint] = charByte2;
          bufferPoint+=1;
          char charByte3 = (char)inByte3;
          buff[bufferPoint] = charByte3;
          bufferPoint+=1;
          char charByte4 = (char)inByte4;
          buff[bufferPoint] = charByte4;
          bufferPoint+=1;
          
        }
      }
      /*
      else if (inByte == '\0' && start==true){
        // end of message packet
        Serial.println("Done image");
        sendMessage();
      }
      */
      else if (start){
        // only start if received "\r in buffer"
        // fill up buffer if not
        // cast uint8_t to char => openMV sends it as uint8_t
        char charByte = (char)inByte;
        //Serial.println(charByte);
        buff[bufferPoint] = charByte;
        bufferPoint +=1;
      }
    }
  }
}
