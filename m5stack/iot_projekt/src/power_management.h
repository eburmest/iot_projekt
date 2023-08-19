#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include "cstdint"

class PowerManager {

  protected:

    static bool _schlaf_enable;
    static uint64_t _schlaf_start; // zeitpunkt (=millis()), ab dem der esp32 in den deep sleep versetzt werden soll
    static uint32_t _schlaf_dauer; // zeit in millisekunden, default sind 5 minuten

  public:

    // legt fest, wie lange der Schlaf dauern soll (millisekunden)
    // und in wie vielen millisekunden der Schlaf starten soll
    static void init(uint32_t schlaf_dauer, uint32_t schlaf_start);

    // sobald der timer den wert 0 erreicht, wird der esp32 in den Schlaf modus versetzt
    // nach der vorgebenen anzahl an millisekunden sollte er wieder aufwachen
    // sollte mehrmals pro sekunde aufgerufen werden
    // gibt true zurueck, nachdem der M5 Stack wieder aufgewacht ist
    static bool update(); 

    // schaltet den schlaf timer an / aus
    static void enableSchlafTimer();
    static void disableSchlafTimer();
    static bool istSchlafEnabled();

    // legt fest, wie lange der Schlaf dauern soll
    static void setSchlafDauer(uint32_t milli_sekunden);

    // resettet den Schlaf timer
    // legt fest, nach wie vielen millisekunden seit dem letzten reset
    // der Schlaf modus aktiviert wird
    static void resetSchlafTimer(uint32_t milli_sekunden);

    // gibt die verbleibende anzahl an millisekunden bis zum nächsten Schlaf zurück
    static uint32_t getMillisekundenBisSchlaf();

};

#endif // POWER_MANAGEMENT_H