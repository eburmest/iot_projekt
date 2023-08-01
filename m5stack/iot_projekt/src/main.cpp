#include <M5Stack.h>
#include <WiFi.h>
#include "HTTPClient.h"
const char* Adresse = "";
const char* password = "";

#include "motor_steuerung.h"
#include "licht_sensor.h"

//Das ist nur ein Versuch

void setup() 
{
  // put your setup code here, to run once:

  M5.Power.begin();
  M5.begin();

  M5.Lcd.print("Hello World!");

  MotorSteuerung::init(); // Motorsteuerung sollte mit Port B verbunden sein
  LichtSensor::init();    // Lichtsensor sollte mit Port B verbunden sein
}

/*//Soll den M5 Stack mit dem Wlan verbinden um die Bilddaten zu übertragen
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
}*/

void loop() {

  static bool LichtSteuerungAktiv = false;

  MotorSteuerung::update();
  LichtSensor::update();

  // Btn A startet den Motor manuell
  if(M5.BtnA.read()) {
    MotorSteuerung::manuell();
    LichtSteuerungAktiv = false;
  }

  // Btn B unterbricht den Motor
  if(M5.BtnB.read()) {
    MotorSteuerung::stop();
    LichtSteuerungAktiv = false;
  }

  if(M5.BtnC.read()) {
    LichtSteuerungAktiv = true;
  }


  // Lichtsteuerung
  if(LichtSteuerungAktiv) {

    if(LichtSensor::istDauerhaftHell(5000))
      MotorSteuerung::heben();
    
    if(LichtSensor::istDauerhaftDunkel(5000))
      MotorSteuerung::senken();

  }

  // Infos auf dem LCD
  M5.Lcd.clear();
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(3);

  if(LichtSteuerungAktiv)
    M5.Lcd.println("Lichtsteuerung:\nAN");
  else
    M5.Lcd.println("Lichtsteuerung:\nAUS");

  if(LichtSensor::istDunkel())
    M5.Lcd.println("Licht: Dunkel");
  else
    M5.Lcd.println("Licht: Hell");

  switch(MotorSteuerung::getStatus()) {
    case MotorSteuerung::Status::OBEN:
      M5.Lcd.println("Status: OBEN"); break;
    case MotorSteuerung::Status::UNTEN:
      M5.Lcd.println("Status: UNTEN"); break;
    case MotorSteuerung::Status::HEBEN:
      M5.Lcd.println("Status: HEBEN"); break;
    case MotorSteuerung::Status::SENKEN:
      M5.Lcd.println("Status: SENKEN"); break;    
    case MotorSteuerung::Status::STOP:
      M5.Lcd.println("Status: STOP"); break;
    case MotorSteuerung::Status::MANUELL:
      M5.Lcd.println("Status: MANUELL"); break;
    default:
      M5.Lcd.println("Status: ERROR"); break;
  }

  delay(200);
}