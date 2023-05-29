#include <Arduino.h>

#define uart1

#ifdef uart1
#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // use UART2


void setup()  
{
  SerialPort.begin(115200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200);
  delay(1000);
  Serial.write("hello");
} 
void loop()  
{ 
  //SerialPort.println("Hello World");
  //delay(1000);

  if (SerialPort.available()) {
    int inByte = SerialPort.read();
    Serial.write(inByte);
  }

  if (Serial.available()) {
    int inByte = Serial.read();
    SerialPort.write(inByte);
  }
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