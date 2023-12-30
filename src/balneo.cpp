/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "c:/Users/antho/OneDrive/Documents/Programmation/Particle/balneo/src/balneo.ino"
/*
 * Project Balneo
 * Author: Rovira Anthony
 * Date: 20/03/2020
 */

#include "application.h"
#include "Capteurs.h"
#include "Actionneurs.h"

void setup();
void loop();
int cloudReset(String command);
int stateQAI(String command);
int redLightToggle(String command);
int greenLightToggle(String command);
int blueLightToggle(String command);
bool particleConnect();
bool particleProcess();
#line 11 "c:/Users/antho/OneDrive/Documents/Programmation/Particle/balneo/src/balneo.ino"
SYSTEM_MODE(MANUAL);    // Connexion automatique au particle cloud.
SYSTEM_THREAD(ENABLED); // Multi thread entre la gestion de la connexion et le programme principal

Capteurs capteurs;       // --------- création de la variable structure qui héberge les données des capteurs et leurs méthodes associées.
Actionneurs actionneurs; // --------- création de la variable Actionneurs qui héberge la structure d'affichage et ses méthodes associées.
Timing timing;           // --------- Création de la variable timer pour gérer les variables temporelles

// --------- Initialisation de la machine d'état du capteur --------
enum Etat
{
  IDLE,         // Attente de la prochaine action à mener
  INIT,         // Initialisation du système
  ACQUISITION,  // Acquisition des données capteurs
  PROCESS,      // Analyse et prise de décisions
  COMMANDE,     // Commande de la led rgb et du moteur
  PUBLISH,      // Publication des données vers l'exterieur
  OLED_DISPLAY, // Gestion de l'affichage sur l'écran OLED
  RECONNECT,    // Gestion de la reconnexion en cas de perte de connexion
  SYSTEM_RESET  // Gestion de la rénitialisation du système
};

Etat etat = INIT;

// --------- Variables globales ----------
double _humidity = -1.0;
double _temperature = -1.0;
int _co2 = -1;
int _presence = -1;
int _nbPresence = -1;
int _dureePresence = -1;
int _durationChgtQAI = -1;
byte stateIndicator;
byte lastState = 10;

void setup()
{
  /* Fonctions Particle cloud */
  Particle.function("Reset", cloudReset);
  Particle.function("QAI", stateQAI);
  Particle.function("RedLight", redLightToggle);
  Particle.function("GreenLight", greenLightToggle);
  Particle.function("BlueLight", blueLightToggle);

  /* Variables Particle cloud */
  Particle.variable("temperature", _temperature);
  Particle.variable("humidity", _humidity);
  Particle.variable("co2", _co2);
  Particle.variable("presence", _presence);
  Particle.variable("NbPresence", _nbPresence);
  Particle.variable("dureePresence", _dureePresence);
  Particle.variable("durationChgtQAI", _durationChgtQAI);

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
      actionneurs.processLED(capteurs.donnees.r_capteurs, capteurs.donnees.g_capteurs, capteurs.donnees.b_capteurs);
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
    capteurs.updateSensors();
    actionneurs.blinkLED(5, 300);

    Particle.publish("info", "Init completed", PRIVATE);
    etat = IDLE;
    break;

  //**************************************************
  case ACQUISITION:
    if (!capteurs.updateSensors())
    {
      Particle.publish("error", "Invalid data from sensors", PRIVATE);
    }
    etat = PROCESS;
    break;

  //**************************************************
  case PROCESS:
    capteurs.evaluateAirQuality();
    capteurs.processPresence();
    // Particle.publish("info", "news data available", PRIVATE);

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
    _durationChgtQAI = (int)capteurs.timingCapteurs.durationChgtQAI;

    etat = IDLE;
    break;

  //**************************************************
  case RECONNECT:

    capteurs.donnees.etat_connexion = particleConnect();
    // capteurs.donnees.etat_connexion ? Particle.publish("reconnect", "connection is back", PRIVATE) : Particle.publish("reconnect", "reconnection failed", PRIVATE);

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
int cloudReset(String command)
{ // Fonction de callback de la Particle.function() reset : reset du système.
  // look for the matching argument "reset" <-- max of 64 characters long
  if (command.equalsIgnoreCase("reset") || command == "1" || command.equalsIgnoreCase("ok"))
  {
    System.reset();
    return 1;
  }
  return -1;
}

// Pour obtenir l'indice de QAI actuel
int stateQAI(String command)
{
  if (command == "" || command == "1" || command.equalsIgnoreCase("ok"))
  {
    return capteurs.donnees.indiceQAI;
  }
  return -1;
}

// Allumer/Eteindre la led Rouge
int redLightToggle(String command)
{
  if (command == "" || command == "1" || command.equalsIgnoreCase("ok"))
  {
    if (actionneurs.stateRedLight())
    {
      actionneurs.redLight(LOW);
      return 0;
    }
    else
    {
      actionneurs.redLight(HIGH);
      return 1;
    }
  }
  return -1;
}

// Allumer/Eteindre la led verte
int greenLightToggle(String command)
{
  if (command == "" || command == "1" || command.equalsIgnoreCase("ok"))
  {
    if (actionneurs.stateGreenLight())
    {
      actionneurs.greenLight(LOW);
      return 0;
    }
    else
    {
      actionneurs.greenLight(HIGH);
      return 1;
    }
  }
  return -1;
}

// Allumer/Eteindre la led bleue
int blueLightToggle(String command)
{
  if (command == "" || command == "1" || command.equalsIgnoreCase("ok"))
  {
    if (actionneurs.stateBlueLight())
    {
      actionneurs.blueLight(LOW);
      return 0;
    }
    else
    {
      actionneurs.blueLight(HIGH);
      return 1;
    }
  }
  return -1;
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
  return Particle.connected();
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