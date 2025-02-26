
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <NewPing.h>
#include <ArduinoJson.h>
// IMPORT ALL REQUIRED LIBRARIES

#include <math.h>
   
//**********ENTER IP ADDRESS OF SERVER******************//

#define HOST_IP     "localhost"       // REPLACE WITH IP ADDRESS OF SERVER ( IP ADDRESS OF COMPUTER THE BACKEND IS RUNNING ON) 
#define HOST_PORT   "8080"            // REPLACE WITH SERVER PORT (BACKEND FLASK API PORT)
#define route       "api/update"      // LEAVE UNCHANGED 
#define idNumber    "620155671"       // REPLACE WITH YOUR ID NUMBER 

// WIFI CREDENTIALS
#define SSID        "MonaConnect"      // "REPLACE WITH YOUR WIFI's SSID"   
#define password    ""  // "REPLACE WITH YOUR WiFi's PASSWORD" 

#define stay        100
 
//**********PIN DEFINITIONS******************//

 
#define espRX         10
#define espTX         11
#define espTimeout_ms 300
#define TRIGGER_PIN 9
#define ECHO_PIN 8
#define MAX_DISTANCE 200

 
 
/* Declare your functions below */
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
double readRadar();
double getWaterheight(float maxWaterHeight, float radar);
double getReserve(float waterheight, float maxWaterHeight);
double getPercentage(float waterheight, float maxWaterHeight);

SoftwareSerial esp(espRX, espTX); 

double maxWaterheight = 77.763;
double waterheight;
double reserve; 
double percentage;
 

void setup(){

  Serial.begin(115200); 
  // Configure GPIO pins here
  pinMode(espRX, INPUT);
  pinMode(espTX, OUTPUT);
 

  espInit();  
 
}

void loop(){ 
   
  // send updates with schema ‘{"id": "student_id", "type": "ultrasonic", "radar": 0, "waterheight": 0, "reserve": 0, "percentage": 0}’
  char msg[100] = {0};
  JsonObject msgJson = jsonBuffer.createObject();
  msgJson["id"] = idNumber;
  msgJson["type"] = "ultrasonic";
  msgJson["radar"] = readRadar();
  msgJson["waterheight"] = getWaterheight();
  msgJson["reserve"] = getReserve();
  msgJson["percentage"] = getPercentage();
  msgJson.printTo(mssg, sizeof(mssg));
  espUpdate(mssg);
  
  delay(1000);  
}

 
void espSend(char command[] ){   
    esp.print(command); // send the read character to the esp    
    while(esp.available()){ Serial.println(esp.readString());}    
}


void espUpdate(char mssg[]){ 
    char espCommandString[50] = {0};
    char post[290]            = {0};

    snprintf(espCommandString, sizeof(espCommandString),"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",HOST_IP,HOST_PORT); 
    espSend(espCommandString);    //starts the connection to the server
    delay(stay);

    // GET REQUEST 
    // snprintf(post,sizeof(post),"GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n",route,HOST_IP,strlen(mssg),mssg);

    // POST REQUEST
    snprintf(post,sizeof(post),"POST /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n",route,HOST_IP,strlen(mssg),mssg);
  
    snprintf(espCommandString, sizeof(espCommandString),"AT+CIPSEND=%d\r\n", strlen(post));
    espSend(espCommandString);    //sends post length
    delay(stay);
    Serial.println(post);
    espSend(post);                //sends POST request with the parameters 
    delay(stay);
    espSend("AT+CIPCLOSE\r\n");   //closes server connection
   }

void espInit(){
    char connection[100] = {0};
    esp.begin(115200); 
    Serial.println("Initiallizing");
    esp.println("AT"); 
    delay(1000);
    esp.println("AT+CWMODE=1");
    delay(1000);
    while(esp.available()){ Serial.println(esp.readString());} 

    snprintf(connection, sizeof(connection),"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,password);
    esp.print(connection);

    delay(3000);  //gives ESP some time to get IP

    if(esp.available()){   Serial.print(esp.readString());}
    
    Serial.println("\nFinish Initializing");    
   
}

//***** Design and implement all util functions below ******
 
// Function to read the radar sensor
double readRadar(){
  delay(50);
   unsigned int radar = sonar.ping_cm();
   Serial.print(radar);
   //Serial.println("cm");
  return radar;
}

double getWaterheight(double maxWaterheight, double radar){
  return maxWaterheight - radar;
}
double getReserve(double waterheight, double maxWaterheight){
  return (waterheight/ maxWaterheight) * 1000;
}
double getPercentage(double waterheight, double maxWaterheight){
  return (waterheight/ maxWaterheight) * 100;
}
