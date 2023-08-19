#include "power_management.h"
#include "M5Stack.h"

bool PowerManager::_schlaf_enable = false;
uint64_t PowerManager::_schlaf_start = -1; // zeitpunkt (=millis()), ab dem der esp32 in den Schlaf versetzt werden soll
uint32_t PowerManager::_schlaf_dauer; // zeit in millisekunden

void PowerManager::init(uint32_t schlaf_dauer, uint32_t schlaf_start) {
    // legt fest, wie lange der Schlaf dauern soll (millisekunden)
    // und in wie vielen millisekunden der Schlaf starten soll

    M5.Power.setWakeupButton(BUTTON_B_PIN);

    setSchlafDauer(schlaf_dauer);
    resetSchlafTimer(schlaf_start);
}

bool PowerManager::update() {
    // sobald der timer den wert 0 erreicht, wird der esp32 in den schlaf modus versetzt
    // nach der vorgebenen anzahl an millisekunden sollte er wieder aufwachen
    // sollte mehrmals pro sekunde aufgerufen werden
    // gibt true zurueck, nachdem der M5 Stack wieder aufgewacht ist

    if(_schlaf_enable && (millis() > _schlaf_start)) {

        Serial.println("Schlaf Modus aktiviert!");
        delay(50);

        // die funktion deepSleep nimmt als parameter mikrosekunden, nicht millisekunden
        M5.Power.lightSleep(_schlaf_dauer * 1000);

        return true;
    }

    return false;
}

void PowerManager::enableSchlafTimer() {

    _schlaf_enable = true;
}

void PowerManager::disableSchlafTimer() {

    _schlaf_enable = false;
}

bool PowerManager::istSchlafEnabled() {

    return _schlaf_enable;
}

void PowerManager::setSchlafDauer(uint32_t milli_sekunden) {
    // legt fest, wie lange der Schlaf dauern soll

    _schlaf_dauer = milli_sekunden;
}

void PowerManager::resetSchlafTimer(uint32_t milli_sekunden) {
    // resettet den schlaf timer
    // legt fest, nach wie vielen millisekunden seit dem letzten reset
    // der schlaf modus aktiviert wird

    _schlaf_start = millis() + milli_sekunden;
}

uint32_t PowerManager::getMillisekundenBisSchlaf() {
    // gibt die verbleibende anzahl an millisekunden bis zum nächsten Schlaf zurück

    return _schlaf_start - millis();
}