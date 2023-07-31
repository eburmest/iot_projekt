#include <M5Stack.h>
#include <WiFi.h>
#include "HTTPClient.h"
const char* Adresse = "";
const char* password = "";

#include "motor_steuerung.h"

//Das ist nur ein Versuch

void setup() 
{
  // put your setup code here, to run once:

  M5.Power.begin();
  M5.begin();

  M5.Lcd.print("Hello World!");

  MotorSteuerung::init();
  digitalWrite(17, 0);

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

void loop() {
  // heben und senken per knopfdruck starten (halbwegs erfolgreich)

  M5.Lcd.clear();
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(3);

  if(M5.BtnB.read()) {
    Serial.println("heben ...");
    M5.Lcd.println("heben ...");
    while(MotorSteuerung::heben());
  } 
  
  if(M5.BtnC.read()) {
    Serial.println("senken ...");
    M5.Lcd.println("senken ...");
    while(MotorSteuerung::senken());
  }

  M5.Lcd.println("warten ...");

  delay(200);
}
