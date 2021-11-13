#include "Capteurs.h"

PietteTech_DHT dht(DHT22_PIN, DHT22);
MHZ19BCO2SensorSerial<USARTSerial> mhz19b(Serial1);

Capteurs::Capteurs()
{
    //Initialisation des variables
}

// Initier la communication avec tous les capteurs
bool Capteurs::begin()
{
    bool state = 1;
    Particle.publish("setup", "Sensor initialization", PRIVATE);

    pinMode(DHT22_PIN, INPUT);
    pinMode(PIR_PIN, INPUT_PULLDOWN);
    //attachInterrupt(PIR_PIN, newPresence, RISING);

    waitingLoop(100);

    dht.begin();
    return state;
}

void Capteurs::waitingLoop(unsigned int timeInMs)
{
    unsigned long previousTime = millis();
    bool waiting = false;
    while (millis() <= previousTime + timeInMs)
    {
        waiting = true;
    }
    waiting = false;
}

float Capteurs::arrondi(float number)
{
    return (int)(number * pow(10, 2) + .5) / pow(10, 2);
}

float Capteurs::getTemperature()
{
    donnees.temperature = arrondi(dht.readTemperature());

    // On contrôle la cohérence des données
    if (isnan(donnees.temperature) || donnees.temperature <= 0)
    {
        return -1;
    }
    else
    {
        return donnees.temperature;
    }
}

float Capteurs::getHumidity()
{
    donnees.humidity = arrondi(dht.readHumidity());

    // On contrôle la cohérence des données
    if (isnan(donnees.humidity) || donnees.humidity <= 0)
    {
        return -1;
    }
    else
    {
        return donnees.humidity;
    }
}

int Capteurs::getCo2()
{
    donnees.co2 = mhz19b.Read();
    // On contrôle la cohérence des données
    if (isnan((float)donnees.co2) || donnees.co2 <= 0)
    {
        return -1;
    }
    else
    {
        return donnees.co2;
    }
}

void Capteurs::newPresence()
{
    donnees.presence = HIGH;
}

bool Capteurs::getPresence()
{
    donnees.presence = digitalRead(PIR_PIN);
    return donnees.presence;
}

int Capteurs::counterNbPresence()
{
    return donnees.nbPresence++;
}

// Remise à zéro du compteur de nombre de fronts montants du détecteur de présence
void Capteurs::RAZNbPresence()
{
    Particle.publish("info", "24 hrs", PRIVATE);
    donnees.nbPresence = 0;
}

// Mise à jour de tous les capteurs
bool Capteurs::MAJCapteurs()
{
    bool isOk = true;
    getTemperature();
    getHumidity();
    getCo2();

    if (donnees.temperature == -1 || donnees.humidity == -1 || donnees.co2 == -1)
    {
        /* Fontion à ajouter dans le futur:
        Envoi d'un Publish pour cibler plus précisement de quel(s) capteur(s) provient l'erreur */
        isOk = false;
    }

    getPresence() ? donnees.presence = HIGH : donnees.presence = LOW;

    return isOk;
}

// Fonction qui permet de tracer les créneaux de présences et d'abscences / Renvoi true si présence il y a
bool Capteurs::processPresence()
{
    if (donnees.presence == HIGH)
    {
        if (donnees.lastPresence != donnees.presence && donnees.lastPresence != -1)
        {
            donnees.lastPresence = donnees.presence;
            donnees.nbPresence++;
            Particle.publish("Motion-Detection", "presence", PRIVATE);
            timingCapteurs.debutPresence = millis();
        }
        return true;
    }
    else
    {
        if (donnees.lastPresence != donnees.presence)
        {
            donnees.lastPresence = donnees.presence;
            Particle.publish("Motion-Detection", "absence", PRIVATE);
            timingCapteurs.finPresence = millis();
            timingCapteurs.dureePresence = ((timingCapteurs.finPresence - timingCapteurs.debutPresence) / 1000) / 60;
        }
        return false;
    }
}

void Capteurs::evaluateAirQuality()
{
    if (donnees.temperature <= 0 || donnees.humidity <= 0 || donnees.co2 <= 0)
    {
        donnees.indiceQAI = QAI_error;
        if (donnees.lastIndiceQAI != donnees.indiceQAI)
        {
            donnees.lastIndiceQAI = donnees.indiceQAI;
            Particle.publish("error", "Invalid data from sensors", PRIVATE);
            timingCapteurs.dureeChgtQAI = ((millis() - timingCapteurs.dernierChgtQAI) / 1000) / 60;
            timingCapteurs.dernierChgtQAI = millis();
        }
        donnees.r_capteurs = HIGH;
        donnees.g_capteurs = LOW;
        donnees.b_capteurs = HIGH;
    }
    else
    {
        if ((donnees.humidity > 85) || (donnees.co2 > 2000))
        {
            donnees.indiceQAI = QAI_rouge;
            if (donnees.lastIndiceQAI != donnees.indiceQAI)
            {
                donnees.lastIndiceQAI = donnees.indiceQAI;
                Particle.publish("newState", "red", PRIVATE);
                timingCapteurs.dureeChgtQAI = ((millis() - timingCapteurs.dernierChgtQAI) / 1000) / 60;
                timingCapteurs.dernierChgtQAI = millis();
            }
            donnees.r_capteurs = HIGH;
            donnees.g_capteurs = LOW;
            donnees.b_capteurs = LOW;
        }
        else if (((donnees.humidity > 75) && (donnees.humidity <= 85)) || ((donnees.co2 > 1400) && (donnees.co2 <= 2000)))
        {
            donnees.indiceQAI = QAI_bleuFonce;
            if (donnees.lastIndiceQAI != donnees.indiceQAI)
            {
                donnees.lastIndiceQAI = donnees.indiceQAI;
                Particle.publish("newState", "blue", PRIVATE);
                timingCapteurs.dureeChgtQAI = ((millis() - timingCapteurs.dernierChgtQAI) / 1000) / 60;
                timingCapteurs.dernierChgtQAI = millis();
            }
            donnees.r_capteurs = LOW;
            donnees.g_capteurs = LOW;
            donnees.b_capteurs = HIGH;
        }
        else if (((donnees.humidity > 65) && (donnees.humidity <= 75)) || ((donnees.co2 > 700) && (donnees.co2 <= 1400)))
        {
            donnees.indiceQAI = QAI_bleuClair;
            if (donnees.lastIndiceQAI != donnees.indiceQAI)
            {
                donnees.lastIndiceQAI = donnees.indiceQAI;
                Particle.publish("newState", "lightBlue", PRIVATE);
                timingCapteurs.dureeChgtQAI = ((millis() - timingCapteurs.dernierChgtQAI) / 1000) / 60;
                timingCapteurs.dernierChgtQAI = millis();
            }
            donnees.r_capteurs = LOW;
            donnees.g_capteurs = HIGH;
            donnees.b_capteurs = HIGH;
        }
        else if ((donnees.humidity <= 65) && (donnees.co2 <= 700))
        {
            donnees.indiceQAI = QAI_vert;
            if (donnees.lastIndiceQAI != donnees.indiceQAI)
            {
                donnees.lastIndiceQAI = donnees.indiceQAI;
                Particle.publish("newState", "green", PRIVATE);
                timingCapteurs.dureeChgtQAI = ((millis() - timingCapteurs.dernierChgtQAI) / 1000) / 60;
                timingCapteurs.dernierChgtQAI = millis();
            }
            donnees.r_capteurs = LOW;
            donnees.g_capteurs = HIGH;
            donnees.b_capteurs = LOW;
        }
    }
    donnees.lastIndiceQAI = donnees.indiceQAI;
}
