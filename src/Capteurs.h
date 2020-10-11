#ifndef CAPTEURS_H
#define CAPTEUR_H

#include <SPI.h>
#include <Wire.h>
#include "Variables.h"
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

    // déclaration des variables et des méthodes publiques
    bool begin(); // initialisation des capteurs

    // gestion des mesures
    float getTemperature(); //Mesurer et obtenir la température d'air
    float getHumidity();    //Mesurer et obtenir l'humidité relative dans l'air
    int getCo2();           //Mesurer et obtenir le taux de co2 dans l'air
    bool getPresence();     //Mesurer et obtenir la présence dans la pièce
    void newPresence();
    int counterNbPresence(); //Compteur de présence dans la pièce
    void RAZNbPresence();    // Remise à zéro du compteur de nombre de fronts montant du détecteur de présence

    // Prise de mesures
    bool MAJCapteurs(); // Lancer toutes les méthodes précédentes en même temps

    // Process
    void evaluateAirQuality();

    //------------------CONTROLE DE TOLERANCE----------------------
    double arrondi(float);

    //--------- Affichage pour utilisateur ------------ //
    void Get_Info_Chauffe();

    // private fonctions
private:
};

#endif
