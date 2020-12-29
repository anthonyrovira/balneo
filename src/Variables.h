#ifndef VARIABLES_H
#define VARIABLES_H

#include "application.h"
#include "Pins.h"
#include "Timing.h"

//  Définition de la version
#define VERSION "2.1"

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
#define MIN_SPEED 200

/********************************************************************
 * Variables définissant l'état des LED
********************************************************************/
#define ON 255
#define OFF 0

/********************************************************************
 * Variables globales
********************************************************************/
//size_t tailleEEPROM; // Returns the total number of bytes available in the emulated EEPROM

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
    //IndiceQAI indiceQAI;

public:
    // mesures
    float humidity;             // Mesure d'humidité relative en %
    float temperature;          // Mesure de température en °C
    int co2;                    // Mesure de CO² en ppm
    volatile int presence;      // Présence d'une personne dans la pièce (0 ou 1)
    volatile int lastPresence;  // Indicateur de comparaison pour savoir s'il s'agit réellement d'une nouvelle présence
    volatile int indiceQAI;     // Indice numérique de la qualité d'air
    volatile int lastIndiceQAI; // Indicateur de comparaison pour la qualité d'air
    volatile int nbPresence;    // Intensité des mouvement dans la pièce (entier >0)
    int r_capteurs;             // Couleur rouge de la led
    int g_capteurs;             // Couleur verte de la led
    int b_capteurs;             // Couleur bleue de la led
    bool etat_LED_rouge;        // Etat de la LED rouge
    bool etat_LED_verte;        // Etat de la LED verte
    bool etat_LED_bleue;        // Etat de la LED bleue
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
        indiceQAI = -1;
        lastIndiceQAI = -1;
        nbPresence = 0; // Intensité des mouvement dans la pièce (entier >0)
        r_capteurs = OFF;
        g_capteurs = OFF;
        b_capteurs = OFF;
        etat_LED_rouge = false; // Etat de la LED rouge
        etat_LED_verte = false; // Etat de la LED verte
        etat_LED_bleue = false; // Etat de la LED bleue
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
        indiceQAI = -1;
        lastIndiceQAI = -1;
        nbPresence = 0; // Intensité des mouvement dans la pièce (entier >0)
        r_capteurs = OFF;
        g_capteurs = OFF;
        b_capteurs = OFF;
        etat_LED_rouge = false; // Etat de la LED rouge
        etat_LED_verte = false; // Etat de la LED verte
        etat_LED_bleue = false; // Etat de la LED bleue
        etat_connexion = false; // Etat de la connexion du Particle

        statut = 0; // Etat des capteurs (0 = OK ou 1 = Erreur)
    }
};
typedef struct Donnees Donnees;

#endif
