#include <M5Stack.h>
#include "HTTPClient.h"
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WiFi.h>


#include "motor_steuerung.h"
#include "licht_sensor.h"
#include "power_management.h"
#include <Adafruit_NeoPixel.h>

WiFiClient wClient;
PubSubClient client(wClient);

const char *mqtt_server = "test.mosquitto.org";
const char *subscripedTopic ="HSOSBarn/door";
const char *ssid = "WiFiBarn";
const char *password = "12345678AB";

{
  M5.Lcd.print("WiFi verbinden");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.print("Wlan verbunden");
  delay(2000);
  M5.Lcd.clear();
}

void callback(char *topic, byte *message, unsigned int length)
{
  if(message[0])
  {
    //M5.Lcd.print("Oeffnen");
    MotorSteuerung::heben();
  }
  else{
    //M5.Lcd.print("Schliessen");
    MotorSteuerung::senken();
  }
}

void setup() 
{
  M5.Power.begin();
  M5.begin();

  M5.Lcd.print("Hello World!");

  startWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  MotorSteuerung::init(); // Motorsteuerung sollte mit Port B verbunden sein
  LichtSensor::init();    // Lichtsensor sollte mit Port B verbunden sein
  App::init();
  PowerManager::init(10 * 60 * 1000, 10000); // der esp32 wird in 10 sekunden für 10 minuten in den Schlafmodus versetzt

}



void loop() {

  static bool LichtSteuerungAktiv = false;
  static Adafruit_NeoPixel LEDs(10, 15, NEO_GRB + NEO_KHZ800);

  MotorSteuerung::update();
  LichtSensor::update();

  if(PowerManager::update()) {
    // der M5 Stack ist gerade wieder aufgewacht
    PowerManager::resetSchlafTimer(10000); // 10 sekunden bis der m5 stack wieder einschläft
    LichtSensor::reset();

    // kurzes blinken nach dem aufwachen
    LEDs.fill(LEDs.Color(0, 255, 0), 0, 5);
    LEDs.show();
    delay(300);
    LEDs.clear();
    LEDs.show();
  }

  // Btn A startet den Motor manuell
  if(M5.BtnA.read()) {
    MotorSteuerung::manuell();
    PowerManager::disableSchlafTimer();
    LichtSteuerungAktiv = false;
  }

  // Btn B unterbricht den Motor
  if(M5.BtnB.read()) {
    MotorSteuerung::stop();
    PowerManager::disableSchlafTimer();
    LichtSteuerungAktiv = false;
  }

  if(M5.BtnC.read()) {
    LichtSteuerungAktiv = true;
    PowerManager::enableSchlafTimer();
    PowerManager::resetSchlafTimer(20000);
  }

  // Lichtsteuerung
  if(LichtSteuerungAktiv) {

    if(LichtSensor::istDauerhaftHell(5000))
      MotorSteuerung::heben();
    
    if(LichtSensor::istDauerhaftDunkel(5000))
      MotorSteuerung::senken();

  }

  // schlaf timer aussetzen, solange der motor angesteuert wird
  if((MotorSteuerung::getStatus() == MotorSteuerung::OBEN) || (MotorSteuerung::getStatus() == MotorSteuerung::UNTEN)) {
      PowerManager::enableSchlafTimer();
  } else {
      PowerManager::disableSchlafTimer();
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

  if(PowerManager::istSchlafEnabled()) {
    M5.Lcd.print("Schlaf Modus in: ");
    M5.Lcd.println(PowerManager::getMillisekundenBisSchlaf() / 1000);
  } else {
    M5.Lcd.println("Schlaf Modus ist AUS");
  }

  M5.Lcd.println("Akku: " + String(M5.Power.getBatteryLevel()) + " % ");


  delay(200);
}