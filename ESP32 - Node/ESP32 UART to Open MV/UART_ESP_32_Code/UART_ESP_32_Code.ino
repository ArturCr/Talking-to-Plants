#include <HardwareSerial.h>
HardwareSerial SerialPort(2); // use UART2

void setup(){
	SerialPort.begin(19200, SERIAL_8E1, 16, 17); // turn on UART
  Serial.begin(115200);

  Serial.println("Setup finished");
}

void loop() {

  if(SerialPort.available()) {
    char data = SerialPort.read();
    Serial.println(data);
  }

}
