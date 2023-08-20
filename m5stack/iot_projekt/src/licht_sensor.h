#ifndef LICHT_SENSOR_H
#define LICHT_SENSOR_H

class LichtSensor {
  
  public:

    // zeitpunkte (millis), zu denen der Lichtsensor zuletzt dunkel bzw hell berichtet hat
    static unsigned long last_hell_millis;
    static unsigned long last_dunkel_millis;

    static void init();
    static void update(); // sollte mehrmals pro sekunde aufgerufen werden
    static void reset(); // sollte nach dem aufwachen aufgerufen werden

    static int getLichtLevel();

    static bool istDunkel();

    /// @return true, falls es seit millis Millisekunden hell bzw dunkel ist
    static bool istDauerhaftDunkel(unsigned long milli_sekunden);
    static bool istDauerhaftHell(unsigned long milli_sekunden);

};

#endif // LICHT_SENSOR_H