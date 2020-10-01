#ifndef CAPTEURS_H
#define CAPTEUR_H

#include <SPI.h>
#include <Wire.h>
#include "variables.h"
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
    int getPresence();      //Mesurer et obtenir la présence dans la pièce
    void RAZNbPresence();   // Remise à zéro du compteur de nombre de fronts montant du détecteur de présence

    bool MAJCapteurs(); // Lancer toutes les méthodes précédentes en même temps
    // Actionneurs

    // LED SDP
    void redLight(int);
    void greenLight(int);
    void blueLight(int);
    void rgbLight(int, int, int);
    void blinkLED(int, int);
    void fadingLed(int, int, int);

    // SSR
    void calcul_compensations(); // Calcul des corrections à appliquer sur la mesure de température de sonde
    void compute_PID();          // Méthode pour mettre à jour les régulateurs PID
    void gerer_SSR_PID();        // Méthode pour mettre à jour les actionneurs SSR en fonction des résultats du PID
    void switch_SSR();
    void allumer_SSR_1(); // Méthode pour allumer le SSR1 et mettre à jour l'indes de conso d'énergie
    // void allumer_SSR_2();   // Méthode pour allumer le SSR2 et mettre à jour l'indes de conso d'énergie
    void eteindre_SSR_1();
    // void eteindre_SSR_2();
    void MAJ_index_energie_SSR_1(); // Méthode pour mettre à jour l'index d'énergie du SSR1 sans changer sons état
    // void MAJ_index_energie_SSR_2(); // Méthode pour mettre à jour l'index d'énergie du SSR2 sans changer sons état

    //------------------CONTROLE DE TOLERANCE----------------------
    double arrondi(float);
    bool Est_Strictement_Inferieur(float v1, float v2, float Tolerance);
    bool Est_Strictement_Superieur(float v1, float v2, float Tolerance);
    bool Est_Inferieur_Ou_Egal(float v1, float v2, float Tolerance);
    bool Est_Superieur_Ou_Egal(float v1, float v2, float Tolerance);
    bool Est_Egal(float v1, float v2, float Tolerance);
    int deltaTempAir(float v1, float v2, float Tolerance);

    //--------- Affichage pour utilisateur ------------ //
    void Get_Info_Chauffe();

    // private fonctions
private:
};

#endif
