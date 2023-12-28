#ifndef TIMING_H
#define TIMING_H

#include "Particle.h"
#include "application.h"
#include "Variables.h"

/********************************************************************
 * Temporisations
 ********************************************************************/
#define TEMPO_MAJ_20mSEC 20    // Temporisation de 20 millisecondes
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
 *  Définition de la structure "Timer" pour les gestions des différentes temporisations
 ********************************************************************/
class Timing
{
public:
    unsigned long derniereMAJ_3SEC;     // Instant de la dernière mise à jour de la vitesse du tachymètre
    unsigned long derniereMAJ_5SEC;     // Instant de la dernière mise à jour des capteurs à évolution rapide (son, dust, vitesse air)
    unsigned long derniereMAJ_15SEC;    // Instant du dernier point affiché
    unsigned long derniereMAJ_30SEC;    // Instant de la dernière mise à jour de la boucle de 30 secondes
    unsigned long derniereMAJ_1MIN;     // Instant de la dernière mise à jour de la boucle de 1 minutes
    unsigned long derniereMAJ_5MIN;     // Instant de la dernière mise à jour de la boucle de 5 minutes
    unsigned long derniereMAJ_1H;       // Instant de la dernière mise à jour de la boucle de 1h
    unsigned long derniereMAJ_24H;      // Instant de la dernière mise à jour de la boucle de 24h
    unsigned long dernierRedemarrage;   // Instant du dernier démarrage du capteur --> pour faire un redémarrage tous les jours
    unsigned long lastChgtQAI;          // Instant du dernier changement d'état de la qualité d'air
    unsigned long durationChgtQAI = -1; // Durée du dernier créneau d'état de la qualité d'air
    unsigned long debutPresence;        // Instant de la détection de présence
    unsigned long finPresence;          // Instant du début de l'absence
    unsigned long dureePresence = -1;   // Durée du dernier créneau de présence
    unsigned long displayTimeout;       // Timeout de l'affichage des données sur l'écran
    unsigned long timeoutConnexion;

    Timing()
    {
        derniereMAJ_3SEC = derniereMAJ_5SEC = derniereMAJ_15SEC = derniereMAJ_30SEC = derniereMAJ_1MIN =
            derniereMAJ_5MIN = derniereMAJ_1H = derniereMAJ_24H = dernierRedemarrage =
                debutPresence = finPresence = displayTimeout = millis();

        lastChgtQAI = 0;
        dureePresence = 0;
        timeoutConnexion = 0;
    }

    void waitingLoop(unsigned int timeInMs);

private:
};
typedef struct Timing Timing;
#endif