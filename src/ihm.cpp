#include "ihm.h"

OledWingAdafruit display;

enum Etat_MENU
{
    MENU_temp = 0, // Affichage des données de température
    MENU_hr = 10,  // Affichage des données d'humidité relative
    MENU_co2 = 20  // Affichage des données de co2
};
Etat_MENU etat_MENU = MENU_temp;

void Ihm::begin()
{
    Particle.publish("setup", "Ihm initialization", PRIVATE);
    // Initialisation de la LED RGB
    pinMode(REDPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);

    // Initialisation de l'écran
    display.setup();
    display.clearDisplay();
    display.display();
}