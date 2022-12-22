#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>   

  

//const char* ssid     = "utk0z";      // SSID of local network
//const char* password = "21/6/2001";   // Password on network
String APIKEY = "1b1ac338bbb911f1d248027ccca4a67e";
//String CityID = "306571"; //konya
String CityID = "750598"; //bilecik


WiFiClient client;
char servername[]="api.openweathermap.org";  // bağlanacağımız server
String result;

boolean night = false;
int  counter = 360;
String weatherDescription ="";
String weatherLocation = "";
float Temperature;

extern  unsigned char  cloud[];
extern  unsigned char  thunder[];
extern  unsigned char  wind[];


void setup() {
  Serial.begin(9600);
  
  Serial.println("AP-Mode Activated");
WiFiManager wifiManager;
WiFi.mode(WIFI_STA);
wifiManager.setTimeout(120);
 if(!wifiManager.autoConnect("Mikroişlemci")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //resetleyip tekrar deneme
    ESP.reset();

    delay(5000);
 } 
  Serial.println("Connecting");
//  WiFi.begin(ssid, password);

  Serial.println("Connecting...");
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  }
  Serial.println("Connected");

}



void loop() {
 getWeatherData();
 delay(30000); 
}



void getWeatherData() //verinin alındığı fonksiyon
{
  Serial.println("Getting Weather Data");
  if (client.connect(servername, 80)) {  
    client.println("GET /data/2.5/forecast?id="+CityID+"&units=metric&cnt=1&APPID="+APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.println("connection failed"); //bağlantı hatası
    Serial.println();
  }

  while(client.connected() && !client.available()) delay(1); 
 
    Serial.println("Waiting for data");

  while (client.connected() || client.available()) { 
    char c = client.read(); //sunucudan verileri okuma
      result = result+c;
    }

  client.stop(); //sunucuyu durdurma 
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);

char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';


StaticJsonDocument<1024> doc2;
deserializeJson(doc2, result);
JsonObject doc = doc2.as<JsonObject>();


String location = doc["city"]["name"];
String temperature = doc["list"]["main"]["temp"];
String weather = doc["list"]["weather"]["main"];
String description = doc["list"]["weather"]["description"];
String idString = doc["list"]["weather"]["id"];


int length = temperature.length();
if(length==5)
{
  temperature.remove(length-1);
}

Serial.print("location: ");
  Serial.println(location);
  Serial.print("weather: ");
Serial.println(weather);
Serial.print("description: ");
Serial.println(description);
Serial.print("temperature: ");
Serial.print(temperature);
Serial.print("o");
Serial.println("C");



}
