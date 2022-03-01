#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

int trigPin = 14; 
int echoPin = 12; 
int led1 = 5;
long duration;
int distance;

const char* ssid="AJNIN";
const char* passwd="f!oxyou!!!.";

String url = "";
//String url = "http://info.cern.ch/";
//String postData = "{\"username\":\"Karla\",\"content\":\"hoi\"}";

String host = "https://discord.com";
int httpsPort = 443;
const char* fingerpr = "7F 8C 3C 11 CB B1 87 28 AA 1B Bf 7B E0 D0 49 64 A6 D3 DA 3A";

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led1, OUTPUT);
  Serial.begin(115200);

  Serial.println("Connecting.");
  WiFi.begin(ssid, passwd);

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

}


void loop() {
  HTTPClient https;
  
  WiFiClientSecure client;
  client.setInsecure(); //the magic line, use with caution
  client.connect(host, httpsPort);


  //Serial.println("Starting JSON");
  StaticJsonBuffer<69> jsonBuffer;
  char json[] = "{\"username\":\"Karla\",\"content\":\"hoi\"}";
  JsonObject& root = jsonBuffer.parseObject(json);
  if(!root.success()) {
    Serial.println("parseObject() failed");
  } else {
    Serial.println("JSON OK");
  }
  
  String postData;
  root.printTo(postData);

  https.begin(client, url);
  https.addHeader("Content-Type", "application/json");

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  duration = pulseIn(echoPin, HIGH);

  distance= duration*0.034/2;

  Serial.print("Distance: ");
  Serial.println(distance);
  if(distance <= 13){
    digitalWrite(led1, HIGH);
    int httpCode = https.POST(postData);
/*  if (httpCode > 0) {
      Serial.println("post sent!");
      //Get the request response payload
      String payload = https.getString();
      //Print the response payload
      Serial.println(payload);
      }
      //Close connection
      https.end();
      Serial.println(httpCode); */
    Serial.println("post sent!");
  }else{
    digitalWrite(led1, LOW);
  }
  delay(500);
}

