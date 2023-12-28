#ifndef CAPTEURS_H
#define CAPTEURS_H

#include <SPI.h>
#include <Wire.h>
#include "Variables.h"
#include "Timing.h"
#include "MH-Z19B-CO2-Sensor-Serial.h"
#include "PietteTech_DHT.h"
#include "math.h"

class Capteurs
{
    // Définition de la classe "capteurs" qui permet de gérer la communication avec les capteurs, le relevé
    // de leurs mesures par des méthodes et de stocker leurs valeurs dans le structure "Données" précedemment définie

public:
    Capteurs();

    Donnees donnees; // Structure "données" pour stocker les résultats des mesures des capteurs
    Timing timingCapteurs;

    /* déclaration des variables et des méthodes publiques */
    bool begin(); // initialisation des capteurs

    /* gestion des mesures */
    float getTemperature(); // Mesurer et obtenir la température d'air
    float getHumidity();    // Mesurer et obtenir l'humidité relative dans l'air
    int getCo2();           // Mesurer et obtenir le taux de co2 dans l'air
    bool getPresence();     // Mesurer et obtenir la présence dans la pièce
    void newPresence();
    int counterNbPresence(); // Compteur de présence dans la pièce
    void RAZNbPresence();    // Remise à zéro du compteur de nombre de fronts montant du détecteur de présence

    /* Prise de mesures */
    bool updateSensors(); // Lancer toutes les méthodes précédentes en même temps

    /* Process */
    // Fonction qui permet de tracer les créneaux de présences et d'abscences
    bool processPresence();
    void evaluateAirQuality();

    // Fonctions pour gérer les données de qualité de l'air
    void handleInvalidData();
    void handleValidData();
    void handleQAIChangeAndPublish(int newIndiceQAI, int r, int g, int b, const char *newState);

    // Fonction pour obtenir une représentation textuelle de l'état de la qualité de l'air
    const char *getStateString(int indiceQAI);

    // Fonction pour définir la couleur RGB des LED en façade
    void setRGB(int r, int g, int b);

    /* private fonctions*/
private:
    /*--------- En remplacement de la fonction delay ------------ */
    void waitingLoop(unsigned int);
};

/**
 * Arrondit un nombre flottant au centième près
 * @param float number - nombre à arrondir
 * @return float - nombre arrondi au centième près
 */
float roundToHundredth(float);

#endif
