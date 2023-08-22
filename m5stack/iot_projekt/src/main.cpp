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
const char *statusTopic="HSOSBarn/status";
const char *ssid = "WiFiBarn";
const char *password = "12345678AB";
unsigned long letztes_status_update = 0; // zeitpunkt, zu dem das letzte mal ein status update per mqtt versendet wurde
unsigned long status_update_intervall = 10 * 1000; // 20 Sekunden (also jedes mal wenn der m5 aufwacht)

bool LichtSteuerungAktiv = false;
Adafruit_NeoPixel LEDs(10, 15, NEO_GRB + NEO_KHZ800);

void startWiFi()
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
  if (message[0]==48)
  {
    LichtSteuerungAktiv = false;
    MotorSteuerung::senken();
  }
  else if(message[0]==49)
  {
    LichtSteuerungAktiv = false;
    MotorSteuerung::heben();
  }
  else if(message[0]==50)
  {
    // automatik modus
    LichtSteuerungAktiv = true;
  }   
  else if(message[0]==51)
  {
    // stop
    LichtSteuerungAktiv = false;
    MotorSteuerung::stop();
  }
  else
  {
    M5.Lcd.print("error");  
  }
}

void setup() 
{
  M5.Power.begin();
  M5.begin();

  M5.Lcd.print("Hello World!");

  MotorSteuerung::init(); // Motorsteuerung sollte mit Port B verbunden sein
  LichtSensor::init();    // Lichtsensor sollte mit Port B verbunden sein
  PowerManager::init(10 * 60 * 1000, 10000); // der esp32 wird in 10 sekunden für 10 minuten in den Schlafmodus versetzt

  startWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}



void loop() {

  while (!client.connected())
  {
      if (client.connect("HSMW3614"))
      {
        client.subscribe(subscripedTopic);
        client.flush();
      }
  }

  client.loop();

  // mqtt status update versenden
  if(letztes_status_update + status_update_intervall < millis()) {
    client.publish(statusTopic, MotorSteuerung::getStatusString().c_str());
    letztes_status_update = millis();
  }

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

  M5.Lcd.print("Status: ");
  M5.Lcd.println(MotorSteuerung::getStatusString());

  if(PowerManager::istSchlafEnabled()) {
    M5.Lcd.print("Schlaf Modus in: ");
    M5.Lcd.println(PowerManager::getMillisekundenBisSchlaf() / 1000);
  } else {
    M5.Lcd.println("Schlaf Modus ist AUS");
  }

  M5.Lcd.println("Akku: " + String(M5.Power.getBatteryLevel()) + " % ");

  delay(200);
}