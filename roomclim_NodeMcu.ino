#include <ESP8266WiFi.h>
#include <DHT.h>
#define DHTPin 4
#define DHTTYPE DHT22
DHT dht(DHTPin, DHTTYPE);
WiFiClient client;
///Set up Thingspeak API
String apiKey = "QA3G4M1WJHCBXY0A";
const char* ssid = "UPCFDDB348";
const char* password = "pG3htndcppd3";
const char* server = "api.thingspeak.com";

//Calculates Dew Point
//refernce: just wikipedia...
//approximation Magnus Formula
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}


//------------------------------Setup----------------------------------//
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(50);
  Serial.println("Weather Station Test");
  dht.begin();
  WiFi.begin(ssid,password);
  Serial.println();
  Serial.println();
  Serial.print("Connection to:  ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Continue if we have Wifi Confirmation
  Serial.println("Wifi connected");
}



//LOOP 
//---------------------------------------------------------------------//
void loop() {

  //---------DHT22---------//
  delay(20000); //Wait a few second betweens the measurements

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float dp = dewPointFast(t,h);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT22");
  }
      
  Serial.println("Great Manu! New Meaurments!");
  Serial.print("Humidity (%): ");
  Serial.println(h);
  Serial.print("Temperature °C ");
  Serial.println(t);
  Serial.print("Dew Point °C  ");
  Serial.println(dp);

  //------Thingspeak-------//
  if (client.connect(server,80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "&field3=";
    postStr += String(dp);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n\n");
    client.print(postStr);

    Serial.println("ThingSpeak - Uploadstring");
    Serial.println(postStr);
  }

  //---------SerialCheckup-----//

  // END OF LOOP
}


