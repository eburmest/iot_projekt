#include <M5Stack.h>
#include <WiFi.h>
#include "HTTPClient.h"
const char* Adresse = "";
const char* password = "";


//Das ist nur ein Versuch

void setup() 
{
  // put your setup code here, to run once:
}

void loop() 
{
  // put your main code here, to run repeatedly:
}

//Soll den M5 Stack mit dem Wlan verbinden um die Bilddaten zu übertragen
void connect() 
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("");
  M5.Lcd.println("WiFi connected!");
  // connect to http server
  client.begin(String(server_url));
  client.sendRequest("CONNECT");
  while (!client.connected()) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("connected to http server");
}
