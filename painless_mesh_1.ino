// Necessary Libraries

#include "painlessMesh.h"
#include <ArduinoJson.h>
#include "HashMap.h"


// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

// variables

SimpleList<uint32_t> nodes;
char buff[512];
StaticJsonDocument<1024> doc;
//StaticJsonDocument<384> doc;
//DynamicJsonDocument doc(1024);
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
int numNodes = 0; // stores number of nodes
int layered_numNodes[10] = {0}; // num of nodes on each layer => 0 = layer1
int nodeList[100] = {0}; // store list of nodeID // done
int nodeTreeList[10][10] = {0}; // depth at most 10 // max 1000 nodes in mesh
bool nodeReceivedState[10][10][3] = {0};
const byte HASH_SIZE = 100; 
// hashmap for all nodes, [iter]=> <nodeID, nth within layer>
HashType<uint32_t,int> hashRawArray[HASH_SIZE]; 
HashMap<uint32_t,int> hashMap = HashMap<uint32_t,int>( hashRawArray , HASH_SIZE ); 
int layer_state = 0;

//40m range 

// array for each 
// matrix for each layer status
//[nth][status1,2,3]
// when received => status[hashMap.getValueOf(nodeID)][message x] = true
void sendMessage(); 

Task taskSendMessage( TASK_SECOND * 10 , TASK_FOREVER, &sendMessage );
// every 5 second we renew request for our layer
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  bool complete = true;
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  // state machine (state 1 = layer 1 ...)
  buff = msg;
  // 1st character in message denotes which message
  // check if received, if received ignore, if not store to memory
  if (nodeReceivedState[layer_state][hashMap.getValueOf(from)][buff[1]]== 0 ){ // if message not received => save to buffer
    // save to buffer
    Serial.println("save to buffer");
  } 

  // check whether this layer is done
  for (int i=0 ; i<layered_numNodes[layer_state] ; ++i ) { // go through all nodes within layer
    for(int j = 0 ; j <3 ; ++i ){
      if (nodeReceivedState[layer_state][i][j]==0){// checks message state
        complete = false; // so if we all 1, our complete flag stays true means we completed
      } 
    }
  }
  // if complete flag still true we go to next layer
  if (complete == true){
    layer_state += 1;  // move on to next layer
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
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

// done portion needed by painless mesh

void sendMessage(){

  
  int length = 6;
  char msg[length]={0,}; // initialise x sized char => with 0's
  for (int i = 0 ;i <length-1; i++  ){
    msg[i] = 'a';
    //Serial.println(i);
  } 
  msg[length -1] = '\0';
  //msg = "This is a trial";
  Serial.println(sizeof(msg));
  Serial.println(msg[5]);
  //serializeJson(doc, msg);
  mesh.sendBroadcast( msg );
  Serial.print("Mesh Broadcast - "); Serial.println(msg);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
  
  
  // parsing Json
  

  DeserializationError error = deserializeJson(doc, mesh.subConnectionJson(true).c_str());

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  long long nodeId = doc["nodeId"]; // 2127231457
  bool root = doc["root"]; // true

  // setting up dataset of node tree
  int layer_counts[10]= {0};
  int hashMapCount = 0;
  for (JsonObject sub : doc["subs"].as<JsonArray>()) { // I can know number of 1st layer nodes
    // iterates through all 1st layer nodes
    
    long long sub_nodeId = sub["nodeId"];
    Serial.print("subs1 =  ");
    Serial.println(sub_nodeId);
    Serial.println(sub.size());
    // if no more sub node => size ==1, >1 -> more subnodes
    nodeTreeList[0][layer_counts[0]]= sub_nodeID;
    layer_counts[0]+=1;
    for (JsonObject sub2 : sub["subs"].as<JsonArray>()) { // iterates through all 2nd layer
      // iterates through all 2nd layer nodes
      Serial.println("Sub2");
      long long sub_nodeId = sub2["nodeId"];
      Serial.print("subs2 =  ");
      Serial.println(sub_nodeId);
      Serial.println(sub2.size());
      nodeTreeList[1][layer_counts[1]]= sub_nodeID;
      layer_counts[1]+=1;
    
      for (JsonObject sub3 : sub2["subs"].as<JsonArray>()) { // iterates through all 3rd layer
      // iterates through all 3rd layer nodes
      Serial.println("Sub3");
      long long sub_nodeId = sub3["nodeId"];
      Serial.print("subs3 =  ");
      Serial.println(sub_nodeId);
      Serial.println(sub3.size());
      nodeTreeList[2][layer_counts[2]]= sub_nodeID;
      layer_counts[2]+=1;
      
        for (JsonObject sub4 : sub3["subs"].as<JsonArray>()) { // iterates through all 4th layer
        // iterates through all 4th layer nodes
        Serial.println("Sub4");
        long long sub_nodeId = sub4["nodeId"];
        Serial.print("subs4 =  ");
        Serial.println(sub_nodeId);
        Serial.println(sub4.size());
        nodeTreeList[3][layer_counts[3]= sub_nodeID;
        layer_counts[3]+=1;
        
          for (JsonObject sub5 : sub4["subs"].as<JsonArray>()) { // iterates through all 5th layer
          // iterates through all 5th layer nodes
          Serial.println("Sub5");
          long long sub_nodeId = sub5["nodeId"];
          Serial.print("subs5 =  ");
          Serial.println(sub_nodeId);
          Serial.println(sub5.size());
          nodeTreeList[4][layer_counts[4]= sub_nodeID;
          layer_counts[4]+=1;
          
            for (JsonObject sub6 : sub5["subs"].as<JsonArray>()) { // iterates through all 6th layer
            // iterates through all 6th layer nodes
            Serial.println("Sub6");
            long long sub_nodeId = sub6["nodeId"];
            Serial.print("subs6 =  ");
            Serial.println(sub_nodeId);
            Serial.println(sub6.size());
            nodeTreeList[5][layer_counts[5]= sub_nodeID;
            layer_counts[5]+=1;
            
              for (JsonObject sub7 : sub6["subs"].as<JsonArray>()) { // iterates through all 7th layer
              // iterates through all 7th layer nodes
              Serial.println("Sub7");
              long long sub_nodeId = sub7["nodeId"];
              Serial.print("subs7 =  ");
              Serial.println(sub_nodeId);
              Serial.println(sub7.size());
              nodeTreeList[6][layer_counts[6]= sub_nodeID;
              layer_counts[6]+=1;
              
                for (JsonObject sub8 : sub7["subs"].as<JsonArray>()) { // iterates through all 7th layer
                // iterates through all 7th layer nodes
                Serial.println("Sub8");
                long long sub_nodeId = sub8["nodeId"];
                Serial.print("subs8 =  ");
                Serial.println(sub_nodeId);
                Serial.println(sub8.size());
                nodeTreeList[7][layer_counts[7]= sub_nodeID;
                layer_counts[7]+=1;
                
                  for (JsonObject sub9 : sub8["subs"].as<JsonArray>()) { // iterates through all 7th layer
                  // iterates through all 7th layer nodes
                  Serial.println("Sub9");
                  long long sub_nodeId = sub9["nodeId"];
                  Serial.print("subs9 =  ");
                  Serial.println(sub_nodeId);
                  Serial.println(sub9.size());
                  nodeTreeList[8][layer_counts[8]= sub_nodeID;
                  layer_counts[8]+=1;
                    
                    for (JsonObject sub10 : sub9["subs"].as<JsonArray>()) { // iterates through all 7th layer
                    // iterates through all 7th layer nodes
                    Serial.println("Sub10");
                    long long sub_nodeId = sub10["nodeId"];
                    Serial.print("subs10 =  ");
                    Serial.println(sub_nodeId);
                    Serial.println(sub10.size());
                    nodeTreeList[9][layer_counts[9]= sub_nodeID;
                    layer_counts[9]+=1;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  Serial.print("nodeID =  ");
  Serial.println(nodeId);

  Serial.print("root =  ");
  Serial.println(root);
  Serial.print("stuff");
 taskSendMessage.setInterval((TASK_SECOND * 10));
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
  mesh.setRoot(true); // set this node as root node
  mesh.setContainsRoot(true); // calls this so that node knows there is a root node
 userScheduler.addTask(taskSendMessage);
 taskSendMessage.enable();

}

void loop() {
  // it will run the user scheduler as well
  mesh.update();

}
