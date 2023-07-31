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
        UNBEKANNT, // die klappe hängt gerade irgendwo, status erst wieder bekannt wenn schalter betätigt
    };

    static Status status;
    static int last_status_pin;

  public:
    // Funktionen zum Steuern des Motors

    static void init(); // konfiguriert die gpio pins

    static Status getStatus();
    
    // die Funktionen sollten mehrmals pro sekunde aufgerufen werden, damit der Motor rechtzeitig stoppen kann
    // @return true, solange der Motor angesteuert wird
    static bool heben(); // steuert den motor an, falls der Status noch nicht "OBEN" ist
    static bool senken();// steuert den motor an, falls der Status noch nicht "UNTEN" ist

};

#endif // MOTOR_STEUERUNG_H