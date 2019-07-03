/*
 * ESP8266 NodeMCU Real Time Graphs Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
#include "index.h" //Our HTML webpage contents with javascripts
 
#define LED LED_BUILTIN  //On board LED
#define gasPin A0
bool inits = false; 

//SSID and Password of your WiFi router
const char* ssid = "Leo_master";
const char* password = "abc12354";
 
ESP8266WebServer server(80); //Server on port 80
 
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {                                    
 String s = MAIN_page; //Read HTML contents
 digitalWrite(LED,LOW); 
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {
 int a = analogRead(gasPin);
 String adcValue = String(a);
 //digitalWrite(LED,!digitalRead(LED)); //Toggle LED on data request ajax
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void handleInit() {
  inits = true; // If a POST request is made to URI /LED
  Serial.println(inits);
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/readADC", handleADC); //This page is called by java Script AJAX
  server.on("/init", HTTP_POST, handleInit);
  
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
