#ifndef MOTOR_STEUERUNG_H
#define MOTOR_STEUERUNG_H


class MotorSteuerung {

  public:
    // status des motors

    enum Status {
        HEBEN,
        SENKEN,
        OBEN,
        UNTEN,
        MANUELL, // der Motor wird dauerhaft angesteuert
        STOP, // die klappe hängt gerade irgendwo, genauer status erst wieder bekannt wenn schalter betätigt
    };

    static Status status;
    static int last_status_pin; // letzter Zustand des Motor-Status-Pins

  public:
    // Funktionen zum Steuern des Motors

    static void init(); // konfiguriert die gpio pins zur steuerung des Motors

    // sollte mehrmals pro sekunde aufgerufen werden
    // falls der Status HEBEN oder SENKEN sein sollte,
    // wird der Motor automatische angehalten sobald das 
    // Erreichen der Endposition erkannt wird und eine kurze 
    // Verzögerungszeit zum Freigeben des Schalters verstrichen ist
    static void update(); 

    static Status getStatus();
    
    static void heben(); // steuert den Motor an, falls der Status noch nicht "OBEN" ist
    static void senken();// steuert den Motor an, falls der Status noch nicht "UNTEN" ist
    static void stop(); // stopt den Motor manuell
    static void manuell(); // startet den Motor manuell

};

#endif // MOTOR_STEUERUNG_H