#ifndef VARIABLES_H
#define VARIABLES_H

#include "Particle.h"
#include "application.h"
#include "Pins.h"

//  Définition de la version
#define VERSION "2.0"

/********************************************************************
 * Variables liées à la board
********************************************************************/
#define VOLTAGE 3.3

/********************************************************************
 * Réservation des Adresses EEPROM
********************************************************************/
#define EEPROM_SIZE 64 // Taille en octets utilisés de l'EEPROM
#define ADRESSE_DEBUT_EEPROM 0

#define CO2_MIN_EEPROM 10       // 4 octets
#define CO2_MAX_EEPROM 14       // 4 octets
#define HR_MIN_EEPROM 18        // 4 octets
#define HR_MAX_EEPROM 22        // 4 octets
#define TEMP_MIN_EEPROM 26      // 4 octets
#define TEMP_MAX_EEPROM 30      // 4 octets
#define LAST_PRESENCE_EEPROM 34 // 4 octets

/********************************************************************
 * Variables servant à l'indice de qualité d'air
********************************************************************/
// CO2
#define CO2_ROUGE 2000
#define CO2_BLEU_DARK 1400
#define CO2_BLEU_LIGHT 700
#define CO2_VERT 400
// Humidité
#define HR_ROUGE 85
#define HR_BLEU_LIGHT 75
#define HR_VERT 65
#define HR_BLEU_DARK 50

/********************************************************************
 * Variables définissant les vitesses du moteur
********************************************************************/
#define MAX_SPEED 214
#define INTER2_SPEED 170
#define INTER1_SPEED 95
#define MIN_SPEED 20

/********************************************************************
 * Temporisations
********************************************************************/
#define TEMPO_MAJ_1SEC 1000    // Temporisation de 1 seconde
#define TEMPO_MAJ_3SEC 3000    // Temporisation de 3 secondes
#define TEMPO_MAJ_5SEC 5000    // Temporisation de 5 secondes
#define TEMPO_MAJ_10SEC 10000  // Temporisation de 10 secondes
#define TEMPO_MAJ_15SEC 15000  // Temporisation de 15 secondes
#define TEMPO_MAJ_30SEC 30000  // Temporisation de 30 secondes
#define TEMPO_MAJ_1MIN 60000   // Temporisation de 1 minute
#define TEMPO_MAJ_5MIN 300000  // Temporisation de 5 minutes
#define TEMPO_MAJ_1H 3600000   // Temporisation de 1h
#define TEMPO_MAJ_24H 86400000 // Temporisation de 24h

/********************************************************************
 * Variables globales
********************************************************************/
size_t tailleEEPROM; // Returns the total number of bytes available in the emulated EEPROM

/********************************************************************
 * Indices relatif aux données de qualité d'air
********************************************************************/
enum IndiceHR
{
    Air_tres_humide = 1,
    Air_humide = 2,
    Air_sain = 3,
    Air_sec = 4,
    Air_tres_sec = 5
};

enum IndiceCO2
{
    CO2_rouge = 1,
    CO2_orange = 2,
    CO2_bleu = 3,
    CO2_vert = 4
};

enum IndiceQAI
{
    QAI_error = 0,
    QAI_vert = 1,
    QAI_bleuClair = 2,
    QAI_bleuFonce = 3,
    QAI_rouge = 4

};

/********************************************************************
 *  Définition de la structure "Donnees" qui regroupe toutes les données 
 *  mesurées par les capteurs afin de les partager avec les autres fonctions du programme.
********************************************************************/
struct Donnees
{
    IndiceHR indiceHR;
    IndiceCO2 indiceCo2;
    IndiceQAI indiceQAI;

public:
    // mesures
    float humidity;             // Mesure d'humidité relative en %
    float temperature;          // Mesure de température en °C
    int co2;                    // Mesure de CO² en ppm
    volatile int presence;      // Présence d'une personne dans la pièce (0 ou 1)
    volatile int lastPresence;  //Indicateur de comparaison pour savoir s'il s'agit réellement d'une nouvelle présence
    volatile int lastIndiceQAI; //Indicateur de comparaison pour la qaulité d'air
    volatile int nbPresence;    // Intensité des mouvement dans la pièce (entier >0)
    int r_value;                // Couleur rouge de la led
    int g_value;                // Couleur verte de la led
    int b_value;                // Couleur bleue de la led
    bool etat_LED;              // Etat de la LED
    bool etat_connexion;        // Etat de la connexion du Particle

    int statut; // Etat des capteurs (0 = OK ou 1 = Erreur)

    // Actionneurs

    void init()
    {

        // Capteurs
        humidity = -1.0;    // Mesure d'humidité relative en %
        temperature = -1.0; // Mesure de température en °C
        co2 = -1;           // Mesure de CO² en ppm
        presence = -1;      // Présence d'une personne dans la pièce (0 ou 1)
        lastPresence = -1;
        lastIndiceQAI = -1;
        nbPresence = -1;        // Intensité des mouvement dans la pièce (entier >0)
        r_value = HIGH;         // Couleur rouge de la led
        g_value = HIGH;         // Couleur verte de la led
        b_value = HIGH;         // Couleur bleue de la led
        etat_LED = false;       // Etat de la LED
        etat_connexion = false; // Etat de la connexion du Particle

        statut = -1; // Etat des capteurs (0 = OK ou 1 = Erreur)

        // Actionneurs
    }

    void RAZ()
    {
        // mesures
        humidity = 0.0;    // Mesure d'humidité relative en %
        temperature = 0.0; // Mesure de température en °C
        co2 = 0;           // Mesure de CO² en ppm
        presence = LOW;    // Présence d'une personne dans la pièce (0 ou 1)
        lastPresence = LOW;
        lastIndiceQAI = -1;
        nbPresence = 0;         // Intensité des mouvement dans la pièce (entier >0)
        r_value = LOW;          // Couleur rouge de la led
        g_value = LOW;          // Couleur verte de la led
        b_value = LOW;          // Couleur bleue de la led
        etat_LED = false;       // Etat de la LED
        etat_connexion = false; // Etat de la connexion du Particle

        statut = 0; // Etat des capteurs (0 = OK ou 1 = Erreur)
    }
};
typedef struct Donnees Donnees;

struct Timer
{
public:
    unsigned long derniereMAJ_3SEC;   // Instant de la dernière mise à jour de la vitesse du tachymètre
    unsigned long derniereMAJ_5SEC;   // Instant de la dernière mise à jour des capteurs à évolution rapide (son, dust, vitesse air)
    unsigned long derniereMAJ_15SEC;  // Instant du dernier point affiché
    unsigned long derniereMAJ_30SEC;  // Instant de la dernière mise à jour de la boucle de 30 secondes
    unsigned long derniereMAJ_1MIN;   // Instant de la dernière mise à jour de la boucle de 1 minutes
    unsigned long derniereMAJ_5MIN;   // Instant de la dernière mise à jour de la boucle de 5 minutes
    unsigned long derniereMAJ_1H;     // Instant de la dernière mise à jour de la boucle de 1h
    unsigned long dernierRedemarrage; // Instant du dernier démarrage du capteur --> pour faire un redémarrage tous les jours
    // unsigned long dernierVerificationEtatWifi;		 // Instant du dernier démarrage du capteur --> pour faire un redémarrage tous les jours
    unsigned long dernierFrontSDP;  // Instant du dernier moment ou quelqu'un a été vu dans la pièce
    unsigned long dernierChgtQAI;   // Instant du dernier changement d'état de la qualité d'air
    unsigned long dernierePresence; // Instant du dernier moment ou quelqu'un a été vu dans la pièce
    unsigned long displayTimeout;
    unsigned long timeoutConnexion;

    void init()
    {
        tailleEEPROM = EEPROM.length();
        // Récupération de la dernière présence détectée dans l'EEPROM
        EEPROM.get(LAST_PRESENCE_EEPROM, dernierFrontSDP);
        if (dernierFrontSDP == 0)
        {
            dernierFrontSDP = millis();
        }
        derniereMAJ_3SEC = millis();
        derniereMAJ_5SEC = millis();
        derniereMAJ_15SEC = millis();
        derniereMAJ_30SEC = millis();
        derniereMAJ_1MIN = millis();
        derniereMAJ_5MIN = millis();
        derniereMAJ_1H = millis();
        dernierRedemarrage = millis();
        dernierChgtQAI = millis();
        // dernierVerificationEtatWifi = millis();
        dernierePresence = millis();
        displayTimeout = millis();
        timeoutConnexion = 0;
    }

    void waitingLoop(unsigned int timeInMs)
    {
        unsigned long previousTime = millis();
        bool waiting = false;
        while (millis() - previousTime >= timeInMs)
        {
            waiting = true;
        }
        waiting = false;
    }

private:
};
typedef struct Timer Timer;

#endif
