#ifndef ACTIONNEURS_H
#define ACTIONNEURS_H

// Cette bibliothèque contient la classe affichage qui permet de gérer tous
// les affichages de sur l'écran OLED

#include "oled-wing-adafruit.h"
#include "Variables.h"
#include "Capteurs.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// définition de la classe affichage
class Actionneurs
{
    Donnees donnees; // Structure "données" pour stocker les résultats des mesures des capteurs

    // déclaration des variables et des méthodes
public:
    void begin();

    // Ecran OLED
    void standby();
    void displayTemp(float);
    void displayHr(float);
    void displayCo2(int);

    // LED
    void redLight(int);
    void greenLight(int);
    void blueLight(int);
    void rgbLight(int, int, int);
    void blinkLED(int, int);
    void fadingLed(int, int, int);

    // Process
    void processLED();
    void processMotor();

private:
};

#endif