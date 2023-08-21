#ifndef LICHT_SENSOR_H
#define LICHT_SENSOR_H

class LichtSensor {
  
  public:

    // Zeitpunkte (millis()), zu denen der Lichtsensor zuletzt dunkel bzw hell berichtet hat
    static unsigned long last_hell_millis;
    static unsigned long last_dunkel_millis;

    // konfiguriert die GPIO Pins um den Lichtsensor auslesen zu können
    static void init();

    // sollte mehrmals pro Sekunde aufgerufen werden
    // liest den digitalen Eingang des Lichtsensors
    // und speichert den aktuellen Zeitpunkt (Rueckgabe der Funktion millis())
    // entsprechend, ob es gerade hell oder dunkel ist, in der dazugehörigen Variable
    static void update(); 

    // sollte nach dem Aufwachen aufgerufen werden
    // da der Lichtsensor seit dem Einschlafen nicht mehr ausgelesen wurde
    // und sowohl der Zeitpunkt an dem es zuletzt hell bzw. dunkel war
    // weit in der Vergangenheit liegen
    static void reset();

    // liest den analogen Eingang des Lichtsensors
    static int getLichtLevel();

    // liest den digitalen Eingang des Lichtsensors
    static bool istDunkel();

    // gibt true zurueck, falls es seit der angegebenen Anzahl an Millisekunden hell bzw dunkel ist
    static bool istDauerhaftDunkel(unsigned long milli_sekunden);
    static bool istDauerhaftHell(unsigned long milli_sekunden);

};

#endif // LICHT_SENSOR_H