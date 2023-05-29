#include <arduino.h>
#include <string.h>
#include <GNSS.h>
#include <RTC.h>
#include <SDHCI.h>
#include <File.h>

// Initialise SpGnss object
SpGnss Gnss;
SDClass SD;
File CurrentFile;
char input = '-1';
String inputString;
String location;
String data;
int indexString;

void setup() {
  // Setup Serial Port for printing/reading
  Serial.begin(115200);
  
  //Initialise GPS
  // Gnss.setInterval(int) can be used to set the interval at which the GNSS module updates postioning data (lowest is 1 second)
  Gnss.begin();
  // Select GPS mode, dual modes can be used to improve accuracy
  Gnss.select(GPS);

  Gnss.start();

  while(!SD.begin()){
    Serial.println("SD Card not detected");
  }

  SD.format(32);
  SD.mkdir("dir/");
    
  Serial.println("setup complete");

}

void loop() {
  
  if (Serial.available() > 0) {

    input = Serial.read();

    if (Serial.read() == '\n' || input == 'r' || input == 'w') {
      switch (input) {
        case 'g':
           if(Gnss.isUpdate()) {
              SpNavData NavData;
              Gnss.getNavData(&NavData);

              Serial.print("Lat=");
              Serial.print(NavData.latitude, 6);
              Serial.print(", Lon=");
              Serial.print(NavData.longitude, 6);
              Serial.print(", Satellites=");
              Serial.print(NavData.numSatellites);
              Serial.println("");    
            }
            else {
              Serial.println("GNSS data not ready");
            }
            break;
                    
        case 't':
            if(Gnss.isUpdate()) {
              SpNavData NavData;
              Gnss.getNavData(&NavData);

              Serial.print("Time: ");
              Serial.print(NavData.time.hour);
              Serial.print(":");
              Serial.print(NavData.time.minute);
              Serial.print(":");
              Serial.print(NavData.time.sec);
              Serial.print("\nDate: ");
              Serial.print(NavData.time.day);
              Serial.print("/");
              Serial.print(NavData.time.month);
              Serial.print("/");
              Serial.print(NavData.time.year);
              Serial.println("");
            }
            else {
              Serial.println("GNSS data not ready");
          }
          break;

        case 'w':
            inputString = Serial.readString();
            
            indexString = inputString.indexOf(' ');
            if (indexString == -1) {
              Serial.println("Incorrect instruction format");
              break;
            }

            location = inputString.substring(0,indexString);
            data = inputString.substring(indexString + 1,inputString.length());   
            
            CurrentFile = SD.open("dir/" + location, FILE_WRITE);

            if(SD.exists("dir/" + location)) {
              Serial.println("File created...");
            }
            else {
              Serial.println("File cannot be created");
              //break;
            }
                                               
            if (CurrentFile) {
              Serial.println("Writing to file...");
              CurrentFile.println(data);
              Serial.println("File wrote to");
            }
            else {
              Serial.println("Error writing to file");
            }

            CurrentFile.close();       

            break;

        case 'r':
            location = Serial.readString();  
            location = location.substring(0,location.length()-1);
            
            CurrentFile = SD.open("dir/" + location);

            if(SD.exists("dir/" + location)) {
            }
            else {
              Serial.println("This file does not exist");
              break;
            }

            if (CurrentFile) {
              while(CurrentFile.available()){
                Serial.write(CurrentFile.read());
              }
            }
            else {
              Serial.println("File could not be opened");
            }

            CurrentFile.close();

            break;
                    
        default:
          Serial.println("Invalid command (g - GPS, t - time, r %location% - read SD, w %location% %data%  - write SD)");
          break;          
      } 
      // waitUpdate can be used to block on the GPS data until ready
      
    }
    else {
      Serial.println("Enter one character only please :)");
      while (Serial.read() != '\n'){}
    }
  }
}
