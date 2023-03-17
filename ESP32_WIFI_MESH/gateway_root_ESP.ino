// gateway root
#include "driver/gpio.h"
#include "painlessMesh.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <HardwareSerial.h>
HardwareSerial SerialPort(2); // use UART2

// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

// variables

SimpleList<uint32_t> nodes;
char buff[70000];
int root = 0;
int numNodes = 0;
int nodeList[100];
StaticJsonDocument<1024> doc;
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void sendMessage(); 

// Routine service
//Task taskSendMessage( TASK_SECOND * 10 , TASK_FOREVER, &sendMessage );

void sendMessage(){
  // can be used to add functionality
};

// save message packet and send to SPRESENSE => then send message back once completed
void receivedCallback( uint32_t from, String &msg ) {
  /*
  // send msg through UART to SPRESENSE
  for (int i = 0 ; i<msg.length ; i++){
    SerialPort.write(msg[i]);
    if (i%128==0){
      delay(500) ; // delay 0.5s every 128 bytes
    }
  }
  // done sending through UART
  // send message to "from" to inform ready for next packet
  String str = "D";
  mesh.sendSingle(from, str);
  Serial.println("EndTime");
  */
  // [TODO] Send datapacket to web server [add]
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
  // prints mesh subconnections
}

// create & save layout of node tree
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  nodes = mesh.getNodeList();
  numNodes = nodes.size();
  Serial.printf("Num nodes: %d\n", numNodes);
  Serial.printf("Connection list:");
  // does not contain itself
  SimpleList<uint32_t>::iterator node = nodes.begin();
  int nodeCount = 0;
  while (node != nodes.end()) 
    {
    nodeList[nodeCount] = *node ; // adds subnodes into list
    //Serial.println(nodeList[nodeCount]);
    Serial.printf(" %u", *node);
    node++;
    nodeCount +=1;
    }
  
  Serial.println();
  
  long nodeId = mesh.getNodeId();
  Serial.print("nodeID =  ");
  Serial.println(nodeId);
  Serial.print("stuff");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200); // for debugging
  SerialPort.begin(115200, SERIAL_8N1, 16, 17); 
  // 16 is RX
  // 17 is TX
  gpio_set_direction(GPIO_NUM_0, GPIO_MODE_OUTPUT);
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.setRoot(true); // set this node as root node
  mesh.setContainsRoot(true); // calls this so that node knows there is a root node
  // 
  /*
  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
  */

}

void loop() {
  mesh.update();

}
