#include <GNSS.h>

// Initialise SpGnss object
static SpGnss Gnss;
char input = '-1';

void setup() {
  // Setup Serial Port for printing/reading
  Serial.begin(115200);
  
  //Initialise GPS
  // Gnss.setInterval(int) can be used to set the interval at which the GNSS module updates postioning data (lowest is 1 second)
  Gnss.begin();
  Gnss.select(GPS);
  Gnss.start();

  Serial.println("setup complete");

}

void loop() {
  
  if (Serial.available() > 0) {
    input = Serial.read();

    if (Serial.read() == '\n') {
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
