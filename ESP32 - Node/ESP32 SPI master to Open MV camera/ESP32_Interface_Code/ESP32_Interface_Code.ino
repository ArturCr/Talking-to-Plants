#include <SPI.h>

#define CHAR_BUF 128
#define SS 4
#define MISO 19 
#define MOSI 18
#define SCLK 5

static const int spiClk = 1;

SPIClass * hspi = NULL;

void setup() {
  
  Serial.begin(115200);

  hspi = new SPIClass(HSPI);
  pinMode(SCLK, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SS, OUTPUT);
  
  hspi->begin(SCLK, MISO, MOSI, SS);

  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  
}

void loop() {
  uint8_t len = 0;
  uint32_t len_temp = 0;
  uint8_t buff2[CHAR_BUF] = {0};

  digitalWrite(SS, LOW);
  Serial.println("Requesting data.....");
  delay(1); // Give the OpenMV Cam some time to setup to send data.

  if(hspi->transfer(1) == 85) { // saw sync char?
    Serial.println("Correct sync char.....");
    delay(1000);
    hspi->transfer(&len, 4); // get length
    if (len) {
      len_temp = len;              
      if (len > CHAR_BUF) {
        len_temp = 128;
      }
      hspi->transfer(buff2, len_temp);
      len -= len_temp;
    }
    while (len--) hspi->transfer(0); // eat any remaining bytes
  }

  digitalWrite(SS, HIGH);

  //unsigned int val = 45;
  char aux[250];
  sprintf(aux, "%u", buff2);

  Serial.println(aux);
  delay(10); // Don't loop to quickly.
}