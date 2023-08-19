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
        MANUELL,
        STOP, // die klappe hängt gerade irgendwo, genauer status erst wieder bekannt wenn schalter betätigt
    };

    static Status status;
    static int last_status_pin;

  public:
    // Funktionen zum Steuern des Motors

    static void init(); // konfiguriert die gpio pins
    static void update(); // sollte mehrmals pro sekunde aufgerufen werden

    static Status getStatus();
    
    static void heben(); // steuert den motor an, falls der Status noch nicht "OBEN" ist
    static void senken();// steuert den motor an, falls der Status noch nicht "UNTEN" ist
    static void stop();
    static void manuell(); // startet den motor manuell

};

#endif // MOTOR_STEUERUNG_H