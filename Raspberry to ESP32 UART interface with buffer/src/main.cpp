#include <Arduino.h>

#define uart3

#ifdef uart3
//missing start & stop message as well as checksum
#define SerailPort
#define RaspAuth
#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // use UART2

//Serial buffer 250 char
const size_t asize=1000; //test up to 3k
char rawdata[asize];
unsigned int aindex=0;

void setup() {
  // initialize the serial communication:
  Serial.begin(115200);
  delay(100);
  Serial.print("Serial Ready");

  #ifdef SerailPort
  SerialPort.begin(115200, SERIAL_8N1, 16, 17); 
  delay(1000);
  Serial.print("Rasp Ready");

  #ifdef RaspAuth
  SerialPort.println("pi");
  delay(1000);

  SerialPort.println("raspberry");
  delay(1000);
  Serial.print("Rasp AUTH Ready");
  #endif
  #endif
}

void loop() {
  #ifdef SerailPort 
  int a=SerialPort.available();
  if (a) {
    // read the most recent byte (which will be from 0 to 255):
    //a=SerialPort.available();
    //bvalue = Serial.read();
    //Serial.printf("N:%d M:%d \n",a,bvalue);
    a=SerialPort.available();
    Serial.printf("N:%d ",a);
    aindex=aindex+SerialPort.readBytes(rawdata+aindex,asize-aindex);
    a=SerialPort.available();
    Serial.printf("N:%d  \n",a);
  }
  if (aindex==asize){
    // for (int i = 0; i < asize; i++) {
    //     Serial.print(rawdata[i]);
    // }
    Serial.print("\n");
    aindex=0;
  }
  delay(1);

  if (Serial.available()) {
    byte bvalue = Serial.read();
    Serial.print(bvalue);
    if (bvalue==90){
      Serial.println("flush");
      SerialPort.flush();
      aindex=0;
    }
  }
#endif

}
#endif

#ifdef uart2
//#define RaspAuth
//#define SerailPort
#include <HardwareSerial.h>

//Serial buffer 250 char
const size_t asize=500;
char rawdata[asize];
unsigned int aindex=0;

void setup() {
  // initialize the serial communication:
  Serial.begin(115200);

}

void loop() {
  byte bvalue;
  // check if data has been sent from the computer:
  int a=Serial.available();
  if (a) {
    // read the most recent byte (which will be from 0 to 255):
    bvalue = Serial.read();
    Serial.printf("N:%d M:%d \n",a,bvalue);

    aindex=aindex+Serial.readBytes(rawdata+aindex,asize-aindex);
    int a=Serial.available();
    Serial.printf("N:%d  \n",a);
  }
  if (aindex==asize){
    for (int i = 0; i < asize; i++) {
        Serial.print(rawdata[i]);
    }
    Serial.print("\n");
    aindex=0;
  }
  delay(1000);
}

#endif


#ifdef uart1
//#define RaspAuth
//#define SerailPort
#include <HardwareSerial.h>

#ifdef SerailPort
HardwareSerial SerialPort(2); // use UART2
#endif

//const size_t BUFF_SIZE=10;
//char UART_Message_IN_BUFF[BUFF_SIZE];
//int UART_Message_IN_BUFF_INDEX;

//int count=1;

void setup()  
{
#ifdef SerailPort
  SerialPort.begin(115200, SERIAL_8N1, 16, 17); 
#endif
  Serial.begin(115200);
  delay(1000);
  Serial.print("Ready");

#ifdef RaspAuth
  SerialPort.println("pi");
  delay(1000);

  SerialPort.println("raspberry");
  delay(1000);
#endif

} 
void loop()  
{ 
  // if (count>1000){
  //   SerialPort.println("Hello World");
  //   //delay(1000);
  //   count=0;
  // }
  // count++;

#ifdef SerailPort
  if (SerialPort.available()) { //rasp
    //int inByte = SerialPort.read();
    //Serial.write(inByte);
    //Serial.print(inByte, DEC);
    //UART_Message_IN_BUFF_INDEX=UART_Message_IN_BUFF_INDEX+Serial.read(UART_Message_IN_BUFF+UART_Message_IN_BUFF_INDEX,BUFF_SIZE-UART_Message_IN_BUFF_INDEX);
  }
#endif

  if (Serial.available()) {  //Comp
    int inByte = Serial.read();
    if (inByte==0x43){//4 or C
      Serial.print("HIT!");
    }
    //SerialPort.write(inByte);
  }
  delay(1);
}


#endif

#ifdef pin_test
int CS0,CLK0,MOSI0,MISO0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  
  // initialize the pushbutton pin as an input
  pinMode(14, INPUT);
  pinMode(32, INPUT);
  pinMode(15, OUTPUT);
  pinMode(33, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // HSPI = CS: 15, CLK: 14, MOSI: 13, MISO: 12
  // CS: blue, CLK: White, MOSI: yellow, MISO: orange
  CS0 = digitalRead(33);
  CLK0 = digitalRead(14);
  MOSI0 = digitalRead(32);
  digitalWrite(15,MOSI0); //MISO0
  Serial.printf("CS:%d, CLK:%d, MOSI:%d, MISO:%d \n",CS0,CLK0,MISO0,MOSI0);
}
#endif

#ifdef slave
#include <ESP32SPISlave.h>

ESP32SPISlave slave;

static constexpr uint32_t BUFFER_SIZE {32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];

void setup() {
    Serial.begin(115200);
    delay(2000);

    // begin() after setting
    // HSPI = CS: 15, CLK: 14, MOSI: 13, MISO: 12 -> default
    // VSPI = CS: 5, CLK: 18, MOSI: 23, MISO: 19
    slave.setDataMode(SPI_MODE0);
    slave.begin();
    // slave.begin(VSPI);   // you can use VSPI like this

    // clear buffers
    memset(spi_slave_tx_buf, 0, BUFFER_SIZE);
    memset(spi_slave_rx_buf, 0, BUFFER_SIZE);
}

void loop() {
    // if there is no transaction in queue, add transaction
    if (slave.remained() == 0) {
        slave.queue(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);
    }

    // if transaction has completed from master,
    // available() returns size of results of transaction,
    // and buffer is automatically updated

    while (slave.available()) {
        // show received data
        for (size_t i = 0; i < slave.size(); ++i) {
            printf("%d ", spi_slave_rx_buf[i]);
        }
        printf("\n");

        // copy received bytes to tx buffer
        memcpy(spi_slave_tx_buf, spi_slave_rx_buf, BUFFER_SIZE);

        slave.pop();
    }
}
#endif