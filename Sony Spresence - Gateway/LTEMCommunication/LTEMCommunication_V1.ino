#include <LTE.h>
#include <LTEUDP.h>

// Connection Settings - Alter if changed on Soracom website (Default - APN: soracom.io , Username: sora , Password: sora)
#define APP_LTE_APN "soracom.io"
#define APP_LTE_USERNAME "sora"
#define APP_LTE_PASSWORD "sora"

// Connectivity Type
// - IPv4: LTE_NET_IPTYPE_V4
// - IPv4v6: LTE_NET_IPTYPE_V4V6
// - IPv6: LTE_NET_IPTYPE_V6
#define APP_LTE_IP_TYPE (LTE_NET_IPTYPE_V4)

// Authentication Method
// - CHAP: LTE_NET_AUTHTYPE_CHAP
// - PAP: LTE_NET_AUTHTYPE_PAP
// - NONE: LTE_NET_AUTHTYPE_NONE
#define APP_LTE_AUTH_TYPE (LTE_NET_AUTHTYPE_CHAP)

// Use LTE-M Coverage
#define APP_LTE_RAT (LTE_NET_RAT_CATM)

// Server Settings (Default - Server: harvest.soracom.io , Port: 8514)
char server[] = "harvest.soracom.io"; // Soracom Server Address
int port = 8514;                      // Soracom UDP Port

// Initliase Objects
LTE lteAccess;
LTEClient client;
LTEUDP udp;
LTEScanner scannerNetworks;
LTEModem modem;
LTENetworkRatType RAT = LTE_NET_RAT_UNKNOWN;

// Function to attach Modem to LTE-M
void doAttach()
{
  
  // Turn on the MODEM and search for LTE-M Network
  if (lteAccess.begin() != LTE_SEARCHING) {
      Serial.println("Could not transition to LTE_SEARCHING.");
      Serial.println("Check LTE board");
      while (true) {
        // If connection cant be made stop program
        sleep(1);
      }
    }

  while (true) {
    if (lteAccess.attach(APP_LTE_RAT,
                         APP_LTE_APN,
                         APP_LTE_USERNAME,
                         APP_LTE_PASSWORD,
                         APP_LTE_AUTH_TYPE,
                         APP_LTE_IP_TYPE) == LTE_READY) {
      Serial.println();
      Serial.println("Connected to Network");
      Serial.println();
 
      // Turn on LED #2 (LED1) to indicate cellular network connection
      digitalWrite(LED1, HIGH);
 
      // Print out connection information
      Serial.println("--------------- Connection Information ---------------");
      Serial.print("Current Carrier: ");
      Serial.println(scannerNetworks.getCurrentCarrier());
 
      Serial.print("IP Address: ");
      Serial.println(lteAccess.getIPAddress());
 
      Serial.print("Current RAT: ");
      RAT = modem.getRAT();
      Serial.println(RAT == LTE_NET_RAT_CATM ? "LTE-M (LTE Cat-M1)" :
                     RAT == LTE_NET_RAT_NBIOT ? "NB-IoT" : "Unknown type [" + String(RAT) + "]");
 
      Serial.print("Signal Strength: ");
      Serial.print(scannerNetworks.getSignalStrength());
      Serial.println(" dBm");
 
      // Exit this while loop
      break;
    }
 
    // Following runs if connection cannot be made
    Serial.println("Error, restarting connection and retrying ....");
    lteAccess.shutdown();
    sleep(1);
  }
}

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial connection
  }

  Serial.println("Starting LTE-M Connection process");

  doAttach();  

  // Wait for the modem to connect to the LTE network.
  Serial.println("Waiting for successful attach.");
  LTEModemStatus modemStatus = lteAccess.getStatus();

  while(LTE_READY != modemStatus) {
    if (LTE_ERROR == modemStatus) {

      Serial.println("Error, restarting connection and retrying ....");
      lteAccess.shutdown();
      sleep(1);
      doAttach();
      
    }
    
    sleep(1);
    modemStatus = lteAccess.getStatus();
    
  }

  Serial.println("Successfuly attachment to LTE-M Network");

  // !!!!! DELETE IN NEXT VERSION, BELOW LINE STOPS CONNECTION AFTER SUCCESSFUL CONNECTION PURELY FOR DEMONSTRATION PURPOSES !!!!!
  lteAccess.shutdown();

}

void loop() {
  
  // No code, as only successful connection is wanted to be tested

}
