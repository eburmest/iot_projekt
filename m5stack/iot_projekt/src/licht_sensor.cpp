#include "licht_sensor.h"
#include "M5Stack.h"

// Port A: kein Analog-Digital Wandler
//#define LICHT_SENSOR_DIGITAL_PIN 21 // Port A, gelbes kabel
//#define LICHT_SENSOR_ANALOG_PIN 22 // Port A, weißes kabel (kein Analog-Digital Wandler)

// Port B:
#define LICHT_SENSOR_DIGITAL_PIN 26 // Port B, gelbes kabel
#define LICHT_SENSOR_ANALOG_PIN 36 // Port B, weißes kabel 

// Port C: kein Analog-Digital Wandler
//#define LICHT_SENSOR_DIGITAL_PIN 17 // Port C, gelbes kabel
//#define LICHT_SENSOR_ANALOG_PIN 16 // Port C, weißes kabel (kein Analog-Digital Wandler)


unsigned long LichtSensor::last_hell_millis;
unsigned long LichtSensor::last_dunkel_millis;

void LichtSensor::init() {

    pinMode(LICHT_SENSOR_DIGITAL_PIN, INPUT);
    pinMode(LICHT_SENSOR_ANALOG_PIN, INPUT);

    last_hell_millis = 0;
    last_dunkel_millis = 0;

}

void LichtSensor::update() {

    if(digitalRead(LICHT_SENSOR_DIGITAL_PIN))
        last_dunkel_millis = millis();
    else
        last_hell_millis = millis();

}

int LichtSensor::getLichtLevel() {
    // funktioniert nur, wenn der Licht Sensor an Port B angeschlossen ist

    return analogRead(LICHT_SENSOR_ANALOG_PIN);
}

bool LichtSensor::istDunkel() {

    return digitalRead(LICHT_SENSOR_DIGITAL_PIN);
}

bool LichtSensor::istDauerhaftDunkel(unsigned long milli_sekunden) {

    return (millis() - last_hell_millis) > milli_sekunden;
}

bool LichtSensor::istDauerhaftHell(unsigned long milli_sekunden) {

    return (millis() - last_dunkel_millis) > milli_sekunden;
}