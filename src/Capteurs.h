#ifndef CAPTEURS_H
#define CAPTEURS_H

#include <SPI.h>
#include <Wire.h>
#include "Variables.h"
#include "Timing.h"
#include "Actionneurs.h"
#include "MH-Z19B-CO2-Sensor-Serial.h"
#include "PietteTech_DHT.h"
#include "math.h"

class Capteurs
{
    // Définition de la classe "capteurs" qui permet de gérer la communication avec les capteurs, le relevé
    //de leurs mesures par des méthodes et de stocker leurs valeurs dans le structure "Données" précedemment définie

public:
    Capteurs();

    Donnees donnees; // Structure "données" pour stocker les résultats des mesures des capteurs
    Timing timingCapteurs;

    /* déclaration des variables et des méthodes publiques */
    bool begin(); // initialisation des capteurs

    /* gestion des mesures */
    float getTemperature(); //Mesurer et obtenir la température d'air
    float getHumidity();    //Mesurer et obtenir l'humidité relative dans l'air
    int getCo2();           //Mesurer et obtenir le taux de co2 dans l'air
    bool getPresence();     //Mesurer et obtenir la présence dans la pièce
    void newPresence();
    int counterNbPresence(); //Compteur de présence dans la pièce
    void RAZNbPresence();    // Remise à zéro du compteur de nombre de fronts montant du détecteur de présence
    int r_capteurs;          // Couleur rouge de la led
    int g_capteurs;          // Couleur verte de la led
    int b_capteurs;          // Couleur bleue de la led

    /* Prise de mesures */
    bool MAJCapteurs(); // Lancer toutes les méthodes précédentes en même temps

    /* Process */
    // Fonction qui permet de tracer les créneaux de présences et d'abscences
    bool processPresence();
    void evaluateAirQuality();

    /*------------------CONTROLE DE TOLERANCE----------------------*/
    float arrondi(float);

    /*--------- Affichage pour utilisateur ------------ */
    void Get_Info_Chauffe();

    /* private fonctions*/
private:
    /*--------- En remplacement de la fonction delay ------------ */
    void waitingLoop(unsigned int);
};

#endif
