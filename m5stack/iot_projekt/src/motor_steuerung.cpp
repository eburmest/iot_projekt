#include "motor_steuerung.h"
#include "M5Stack.h"

// Port B nutzen um motor anzusteuern
#define MOTOR_ENABLE_PIN 26 // Port B, gelbes kabel
#define MOTOR_STATUS_PIN 36 // Port B, weißes kabel


// Port C nutzen um motor anzusteuern (funktioniert aus gründen irgendwie nicht so richtig)
//#define MOTOR_ENABLE_PIN 17 // Port C, gelbes kabel
//#define MOTOR_STATUS_PIN 16 // Port C, weißes kabel

// globaler (static) Status der Klappe
MotorSteuerung::Status MotorSteuerung::status;
int MotorSteuerung::last_status_pin; // zustand des status pins beim letzten mal lesen

void MotorSteuerung::init() {
    // konfiguriert die gpio pins

    pinMode(MOTOR_ENABLE_PIN, OUTPUT);
    pinMode(MOTOR_STATUS_PIN, INPUT);

    status = UNBEKANNT;
    last_status_pin = digitalRead(MOTOR_STATUS_PIN);

} 

MotorSteuerung::Status MotorSteuerung::getStatus() {

    return status;
}

bool MotorSteuerung::heben() {
    // steuert den motor an, falls der Status noch nicht "OBEN" ist
    // @return true, solange der Motor angesteuert wird

    if(status == OBEN)
        return false;

    // lesen des status pins
    int current_status_pin = digitalRead(MOTOR_STATUS_PIN);

    if((last_status_pin == 0) && (current_status_pin == 1)) {
        // wenn der status pin von low nach high wechselt, ist die klappe oben
        status = OBEN;
        digitalWrite(MOTOR_ENABLE_PIN, 0); // motor anhalten
        last_status_pin = current_status_pin;
        return false;
    }

    // bewegen der klappe
    digitalWrite(MOTOR_ENABLE_PIN, 1);
    delay(500);

    // status aktualisieren
    if(current_status_pin == 0)
        status = HEBEN;
    else
        status = SENKEN; // evtl muss die klappe erst abgesenkt werden, um anschließend geöffnet werden zu können

    last_status_pin = current_status_pin;
    
    return true;
}

bool MotorSteuerung::senken() {
    // steuert den motor an, falls der Status noch nicht "UNTEN" ist
    // @return true, solange der Motor angesteuert wird

    if(status == UNTEN)
        return false;

    // lesen des status pins
    int current_status_pin = digitalRead(MOTOR_STATUS_PIN);

    if((last_status_pin == 1) && (current_status_pin == 0)) {
        // wenn der status pin von high nach low wechselt, ist die klappe unten
        status = UNTEN;
        digitalWrite(MOTOR_ENABLE_PIN, 0); // motor anhalten
        last_status_pin = current_status_pin;
        return false;
    }

    // bewegen der klappe
    digitalWrite(MOTOR_ENABLE_PIN, 1);
    delay(500);

    // status aktualisieren
    if(current_status_pin == 0)
        status = HEBEN; // evtl muss die klappe erst gehoben werden, um anschließend gesenkt werden zu können
    else
        status = SENKEN; 

    last_status_pin = current_status_pin;
    
    return true;
}