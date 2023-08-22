#include "motor_steuerung.h"
#include "M5Stack.h"

// Port A nutzen um motor anzusteuern (funktioniert nicht)
//#define MOTOR_ENABLE_PIN 21 // Port A, gelbes kabel
//#define MOTOR_STATUS_PIN 22 // Port A, weißes kabel

// Port B nutzen um motor anzusteuern (funktioniert)
//#define MOTOR_ENABLE_PIN 26 // Port B, gelbes kabel
//#define MOTOR_STATUS_PIN 36 // Port B, weißes kabel

// Port C nutzen um motor anzusteuern (funktioniert, aber kabelfarben vertauscht)
#define MOTOR_ENABLE_PIN 16 // Port C, weißes kabel
#define MOTOR_STATUS_PIN 17 // Port C, gelbes kabel

// globaler (static) Status der Klappe
MotorSteuerung::Status MotorSteuerung::status;
int MotorSteuerung::last_status_pin; // zustand des status pins beim letzten mal lesen

void MotorSteuerung::init() {
    // konfiguriert die gpio pins

    pinMode(MOTOR_ENABLE_PIN, OUTPUT);
    pinMode(MOTOR_STATUS_PIN, INPUT);

    status = STOP;
    last_status_pin = digitalRead(MOTOR_STATUS_PIN);

}

void MotorSteuerung::update() {
    // sollte mehrmals pro sekunde aufgerufen werden

    // lesen des status pins
    int current_status_pin = digitalRead(MOTOR_STATUS_PIN);

    // wenn der status pin von low nach high wechselt, ist die klappe oben
    if((status == HEBEN) && (last_status_pin == 0) && (current_status_pin == 1)) {
        status = OBEN;
        delay(300); // den motor etwas weiter laufen lassen, damit der schalter freigegeben wird
        digitalWrite(MOTOR_ENABLE_PIN, 0); // motor anhalten
    }

    // wenn der status pin von low nach high wechselt, ist die klappe oben
    if((status == SENKEN) && (last_status_pin == 1) && (current_status_pin == 0)) {
        status = UNTEN;
        delay(300); // den motor etwas weiter laufen lassen, damit der schalter freigegeben wird
        digitalWrite(MOTOR_ENABLE_PIN, 0); // motor anhalten
    }

    last_status_pin = current_status_pin;

} 

MotorSteuerung::Status MotorSteuerung::getStatus() {

    return status;
}

String MotorSteuerung::getStatusString() {

    switch(MotorSteuerung::getStatus()) {
      case MotorSteuerung::Status::OBEN:
        return "OBEN";
      case MotorSteuerung::Status::UNTEN:
        return "UNTEN";
      case MotorSteuerung::Status::HEBEN:
        return "HEBEN";
      case MotorSteuerung::Status::SENKEN:
        return "SENKEN";    
      case MotorSteuerung::Status::STOP:
        return "STOP";
      case MotorSteuerung::Status::MANUELL:
        return "MANUELL";
      default:
        return "ERROR";
    }

}

void MotorSteuerung::heben() {
    // steuert den motor an, falls der Status noch nicht "OBEN" ist

    if(status == OBEN)
        return;

    // motor ansteuern
    digitalWrite(MOTOR_ENABLE_PIN, 1);
    status = HEBEN;

}

void MotorSteuerung::senken() {
    // steuert den motor an, falls der Status noch nicht "UNTEN" ist
    // @return true, solange der Motor angesteuert wird

    if(status == UNTEN)
        return;

    // motor ansteuern
    digitalWrite(MOTOR_ENABLE_PIN, 1);
    status = SENKEN;

}

void MotorSteuerung::stop() {

    if(status == STOP)
        return;

    // motor anhalten
    digitalWrite(MOTOR_ENABLE_PIN, 0);
    status = STOP;

}

void MotorSteuerung::manuell() {
    // startet den motor manuell

    if(status == MANUELL)
        return;

    // motor starten
    digitalWrite(MOTOR_ENABLE_PIN, 1);
    status = MANUELL;
} 
