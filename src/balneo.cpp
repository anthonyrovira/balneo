/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "d:/antho/Documents/Programmation/Particle/balneo/src/balneo.ino"
/*
 * Project Balneo
 * Description:
 * Author: Rovira Anthony
 * Date: 20/03/2020
 */

#include "application.h"
#include "Capteurs.h"
#include "Actionneurs.h"

//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 32 // OLED display height, in pixels

void setup();
void loop();
int cloud_reset(String command);
int state_QAI(String command);
bool particleConnect();
bool particleProcess();
#line 15 "d:/antho/Documents/Programmation/Particle/balneo/src/balneo.ino"
SYSTEM_MODE(MANUAL);    // Connexion automatique au particle cloud.
SYSTEM_THREAD(ENABLED); // Multi thread entre la gestion de la connexion et le programme principal

Capteurs capteurs;       // --------- création de la variable structure qui héberge les données des capteurs et leurs méthodes associées.
Actionneurs actionneurs; // --------- création de la variable Actionneurs qui héberge la structure d'affichage et ses méthodes associées.
Timing timing;           // --------- Création de la variable timer pour gérer les variables temporelles

// --------- Initialisation de la machine d'état du capteur --------
enum Etat
{
  IDLE = 0,         // Attente de la prochaine action à mener
  INIT = 1,         // Initialisation du système
  ACQUISITION = 2,  // Acquisition des données capteurs
  PROCESS = 3,      // Analyse et prise de décisions
  COMMANDE = 4,     // Commande de la led rgb et du moteur
  PUBLISH = 5,      // Publication des données vers l'exterieur
  OLED_DISPLAY = 6, // Gestion de l'affichage sur l'écran OLED
  RECONNECT = 7,    // Gestion de la reconnexion en cas de perte de connexion
  SYSTEM_RESET = 10 // Gestion de la rénitialisation du système
};

Etat etat = INIT;

// --------- Variables globales ----------
// int motorState = 0;
double _humidity = -1.0;
double _temperature = -1.0;
int _co2 = -1;
int _presence = -1;
int _nbPresence = -1;
int _dureePresence = -1;
int _dureeChgtQAI = -1;
// bool pirValue = false;
byte stateIndicator;
byte lastState = 10;

void setup()
{
  /* Fonctions Particle cloud */
  Particle.function("Reset", cloud_reset);
  Particle.function("QAI", state_QAI);

  /* Variables Particle cloud */
  Particle.variable("temperature", _temperature);
  Particle.variable("humidity", _humidity);
  Particle.variable("co2", _co2);
  Particle.variable("presence", _presence);
  Particle.variable("NbPresence", _nbPresence);
  Particle.variable("dureePresence", _dureePresence);
  Particle.variable("dureeChgtQAI", _dureeChgtQAI);

  particleConnect();

  Particle.publish("info", "Balneo version " + String(VERSION), PRIVATE);

  capteurs.begin();        // initialisation des capteurs
  capteurs.donnees.init(); // initialisation des donnees des capteurs

  actionneurs.begin();                     // initialisation des actionneurs
  actionneurs.blinkLED(5, TEMPO_MAJ_1SEC); // Clignotement de LED pour vérifier que les actionneurs fonctionnent

  Particle.publish("info", "Setup completed", PRIVATE);
}

void loop()
{
  switch (etat)
  {
  case IDLE:
    if (!capteurs.donnees.etat_connexion)
    {
      etat = RECONNECT;
    }

    if (millis() - timing.derniereMAJ_5SEC >= TEMPO_MAJ_5SEC)
    {
      capteurs.donnees.etat_connexion = particleProcess();
      timing.derniereMAJ_5SEC = millis();
    }

    if (millis() - timing.derniereMAJ_30SEC >= TEMPO_MAJ_30SEC)
    {
      etat = ACQUISITION;
      timing.derniereMAJ_30SEC = millis();
    }

    if (millis() - timing.derniereMAJ_24H >= TEMPO_MAJ_24H)
    {
      capteurs.RAZNbPresence();
      timing.derniereMAJ_24H = millis();
    }

    if (capteurs.processPresence())
    {
      actionneurs.processLED(capteurs.r_capteurs, capteurs.g_capteurs, capteurs.b_capteurs);
      actionneurs.displayTemp(capteurs.donnees.temperature);
      actionneurs.displayHr(capteurs.donnees.humidity);
      actionneurs.displayCo2(capteurs.donnees.co2);
    }
    else
    {
      actionneurs.standby();
      actionneurs.fadingLed(HIGH, HIGH, HIGH);
      actionneurs.fadingLed(HIGH, HIGH, LOW);
      actionneurs.fadingLed(HIGH, LOW, HIGH);
    }

    break;

  //**************************************************
  case INIT:
    if (!capteurs.donnees.etat_connexion)
    {
      etat = RECONNECT;
    }
    timing.init();
    capteurs.MAJCapteurs();
    actionneurs.blinkLED(5, 300);

    Particle.publish("info", "Init completed", PRIVATE);
    etat = IDLE;
    break;

  //**************************************************
  case ACQUISITION:
    if (!capteurs.MAJCapteurs())
    {
      Particle.publish("error", "Invalid data from sensors", PRIVATE);
    }
    etat = PROCESS;
    break;

  //**************************************************
  case PROCESS:
    capteurs.evaluateAirQuality();
    capteurs.processPresence();
    Particle.publish("info", "news data available", PRIVATE);

    etat = COMMANDE;
    break;

  //**************************************************
  case COMMANDE:
    actionneurs.processMotor(capteurs.donnees.indiceQAI);

    etat = PUBLISH;
    break;

  //**************************************************
  case PUBLISH:
    _humidity = (double)capteurs.donnees.humidity;
    _temperature = (double)capteurs.donnees.temperature;
    _co2 = capteurs.donnees.co2;
    _presence = capteurs.donnees.presence;
    _nbPresence = capteurs.donnees.nbPresence;
    _dureePresence = (int)capteurs.timingCapteurs.dureePresence;
    _dureeChgtQAI = (int)capteurs.timingCapteurs.dureeChgtQAI;
    //Particle.publish("info", "news data available", PRIVATE);

    etat = IDLE;
    break;

  //**************************************************
  case OLED_DISPLAY:
    /* code */
    break;

  //**************************************************
  case RECONNECT:

    capteurs.donnees.etat_connexion = particleConnect();
    //capteurs.donnees.etat_connexion ? Particle.publish("reconnect", "connection is back", PRIVATE) : Particle.publish("reconnect", "reconnection failed", PRIVATE);

    etat = IDLE;
    break;

  //**************************************************
  case SYSTEM_RESET:
    Particle.publish("reset", "Reset processing...", PRIVATE);
    timing.waitingLoop(2000);
    System.reset();
    break;

  //**************************************************
  default:
    etat = INIT;
    break;
  }
}

//  Reset depuis le cloud
int cloud_reset(String command)
{ // Fonction de callback de la Particle.function() reset : reset du système.
  // look for the matching argument "reset" <-- max of 64 characters long
  if (command.toLowerCase() == "reset" || command == "1" || command.toLowerCase() == "ok")
  {
    etat = SYSTEM_RESET;
  }
}

// Pour obtenir l'indice de QAI actuel
int state_QAI(String command)
{
  if (command == "" || command == "1" || command.toLowerCase() == "ok")
  {
    return (int)capteurs.donnees.indiceQAI;
  }
}

// Procédure de connexion au cloud Particle
bool particleConnect()
{
  if (!Particle.connected())
  {
    do
    {
      Particle.connect();
      timing.timeoutConnexion = millis();
    } while (!Particle.connected() || (millis() - timing.timeoutConnexion) >= TEMPO_MAJ_15SEC);
  }
  if (Particle.connected())
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool particleProcess()
{
  if (capteurs.donnees.etat_connexion)
  {
    Particle.process();
    return true;
  }
  else
  {
    return false;
  }
}