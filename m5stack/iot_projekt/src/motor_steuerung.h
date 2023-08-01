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
        STOP, // die klappe hängt gerade irgendwo, genauer status erst wieder bekannt wenn schalter betätigt
    };

    static Status status;
    static int last_status_pin;

  public:
    // Funktionen zum Steuern des Motors

    static void init(); // konfiguriert die gpio pins
    static void update(); // sollte mehrmals pro sekunde aufgerufen werden

    static Status getStatus();
    
    // die Funktionen sollten mehrmals pro sekunde aufgerufen werden, damit der Motor rechtzeitig stoppen kann
    // @return true, solange der Motor angesteuert wird
    static void heben(); // steuert den motor an, falls der Status noch nicht "OBEN" ist
    static void senken();// steuert den motor an, falls der Status noch nicht "UNTEN" ist
    static void stop();

};

#endif // MOTOR_STEUERUNG_H