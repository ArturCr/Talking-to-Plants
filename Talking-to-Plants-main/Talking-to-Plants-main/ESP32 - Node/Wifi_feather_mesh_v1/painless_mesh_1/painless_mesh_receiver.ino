// receiver

// Necessary Libraries

#include "painlessMesh.h"
#include <ArduinoJson.h>

// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
SimpleList<uint32_t> nodes;
char buff[5000];

// variables
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
payload_length = 3; // should be variable depending on image size  TEMP

void sendMessage(); 

Task taskSendMessage( TASK_SECOND * 10 , TASK_FOREVER, &sendMessage );



// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  // check what is received => if it is "S" => we send our payload

  if (msg[0]=="S"){
    for (int i = 0 ; i<payload_length ; i++){ // send payload
      sendSingle(from, payload[i]){

      }
    }
  }
  

}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
  // prints mesh subconnections
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  nodes = mesh.getNodeList();
  Serial.printf("Num nodes: %d\n", nodes.size());
  Serial.printf("Connection list:");
  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) 
    {
    Serial.printf(" %u", *node);
    node++;
    }
  Serial.println();
  //calc_delay = true;

  sprintf(buff,"Nodes:%d",nodes.size());
  //tft.drawString(buff, 0, 32);;
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

// done portion needed by painless mesh

void sendMessage(){
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // for debugging

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.setContainsRoot(true); // all sub nodes call this to know there is a root node(gateway)
 
 userScheduler.addTask(taskSendMessage);
 taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();

}
