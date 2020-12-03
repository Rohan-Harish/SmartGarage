#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define led 2

WiFiServer connectionServer(2020);
bool realNetwork = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Port Opened");
  //WiFi.mode(WIFI_AP);
  if(wifiSetUp()){
    Serial.println("Successfully set up");
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

//Opens temporary AccessPoint for Wifi Set up
bool wifiSetUp(){
  const char* ssid;
  const char* pass;
  
  WiFi.softAP("SmartGarage", "testpassword");
  Serial.println(WiFi.softAPIP());
  Serial.print("\nConnecting");
  connectionServer.begin();
  while(!realNetwork){
    WiFiClient client = connectionServer.available();
    if (client){
      Serial.println("Client found");
      client.println("Yo Good job Connecting");
      while(client.available()){
        String text = client.readStringUntil('-');
        Serial.println("b"+text+"b");
        if(text == "data:"){
          Serial.println("inData");
          String text = client.readStringUntil('-');
          Serial.println(text);
          DynamicJsonDocument doc(text.length()+150);
          deserializeJson(doc, text);
          //JsonObject data = doc.as<JsonObject>();
          
          ssid = doc["ssid"];
          pass = doc["password"];
          Serial.println(ssid);
          realNetwork = true;
        }
        else{
          Serial.println("No Data Found");
        }
      }
    }
  }
  Serial.println("Connecting to new Connection");
  WiFi.begin(ssid, pass);
  Serial.println(WiFi.localIP());
  return true;
}
