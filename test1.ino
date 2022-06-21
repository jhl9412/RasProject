#include <Arduino.h>

#include <WiFi.h>


#include <HTTPClient.h>
#include <DFRobot_DHT11.h>
#include <Arduino_JSON.h>

DFRobot_DHT11 DHT;

#define DHT11_PIN 16
#define USE_SERIAL Serial

boolean sensor_read = 0;
char msg[100] = {};
String payload;

String a;

//char* url = "https://api.openweathermap.org/data/2.5/group?id=1835848,1835235,1838524,1846266&main=temp&units=metric&appid=d957ce1d7046f700101b12e6c718d5c6";
char* url1 = "https://api.openweathermap.org/data/2.5/weather?q=Seoul&units=metric&appid=d957ce1d7046f700101b12e6c718d5c6";
char* url2 = "https://api.openweathermap.org/data/2.5/weather?q=Daejeon&units=metric&appid=d957ce1d7046f700101b12e6c718d5c6";
char* url3 = "https://api.openweathermap.org/data/2.5/weather?q=Busan&units=metric&appid=d957ce1d7046f700101b12e6c718d5c6";
char* url4 = "https://api.openweathermap.org/data/2.5/weather?q=Jeju&units=metric&appid=d957ce1d7046f700101b12e6c718d5c6";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;      

const char * host = "10.10.141.236";
const uint16_t port = 5000;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.begin("iot1", "iot10000");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

   if (!client.connect(host, port)) {
 
      Serial.println("Connection to host failed");
      delay(1000);
      return;
    }
 
    Serial.println("Connected to server successful!");
    client.print("[1:PASSWD]");
}

void loop() {
    // wait for WiFi connection
  if(WiFi.status()== WL_CONNECTED){

    while(client.available())
     {
      
      a = client.readString();
      Serial.println(a);
      if(a == "[2]stop\n"){
      Serial.println("Disconnecting...");
      client.stop();}    
    }
      // seoul data => temp, humi, weather
      getPublicData(url1);
      JSONVar seoulData = JSON.parse(payload);
      if(JSON.typeof(seoulData) =="undefined"){
        //Serial.println("Parsing input failed!");
        return;  
      }

      double tempSeoul = seoulData["main"]["temp"];
      int humiSeoul = seoulData["main"]["humidity"];
      //Serial.println(seoulData["weather"][0]["main"]);
//      char weatherSeoul[20] = seoulData["weather"]["main"];

      payload.clear();
      
      // daejeon data => temp, humi
      getPublicData(url2);
      JSONVar daejeonData = JSON.parse(payload);
      if(JSON.typeof(daejeonData) =="undefined"){
       // Serial.println("Parsing input failed!");
        return;  
      }
      //Serial.println(daejeonData["main"]["temp"]);
      double tempDaejeon = daejeonData["main"]["temp"];
      int humiDaejeon = daejeonData["main"]["humidity"];
      payload.clear();

      // busan data => temp, humi
      getPublicData(url3);
      JSONVar busanData = JSON.parse(payload);
      if(JSON.typeof(busanData) =="undefined"){
      //  Serial.println("Parsing input failed!");
        return;  
      }
      //Serial.println(busanData["main"]["temp"]);
      double tempBusan = busanData["main"]["temp"];
      int humiBusan = busanData["main"]["humidity"];
      payload.clear();

      // jeju data => temp, humi
      getPublicData(url4);
      JSONVar jejuData = JSON.parse(payload);
      if(JSON.typeof(jejuData) =="undefined"){
    //    Serial.println("Parsing input failed!");
        return;  
      }
     // Serial.println(jejuData["main"]["temp"]);
      double tempJeju = jejuData["main"]["temp"];
      int humiJeju = jejuData["main"]["humidity"];
      payload.clear();

      //indoor temp, humi
      double temp; 
      int humi;
      DHT.read(DHT11_PIN);
      temp = DHT.temperature;
      humi = DHT.humidity;
      //msg = "[2]";
      sprintf(msg, "[19]%.1lf@%d@%.1lf@%d@%.1lf@%d@%.1lf@%d@%.1lf@%dL\n"
      ,temp,humi,tempSeoul,humiSeoul,tempDaejeon,humiDaejeon,
      tempBusan,humiBusan,tempJeju,humiJeju);
      Serial.println(msg);
     
      client.print(msg);
  }
  
    fflush(stdout);
    delay(5000);


}

void getPublicData(char* url)
{
 
        HTTPClient http;
        http.begin(url); //HTTP
        int httpCode = http.GET();
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                payload = http.getString();
            }
        } else {
      //      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();

}
