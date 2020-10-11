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

    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(DHT22_PIN, INPUT);
    pinMode(PIR_PIN, INPUT_PULLDOWN);
    attachInterrupt(PIR_PIN, newPresence, RISING);

    delay(100);

    dht.begin();
    return state;
}

double arrondi(float number)
{
    return (double)((int)(number * pow(10, 2) + .5)) / pow(10, 2);
}

float Capteurs::getTemperature()
{
    donnees.temperature = arrondi(dht.readTemperature());

    // On contrôle la cohérence des données
    if (isnan(donnees.temperature) || donnees.temperature < 0)
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
    if (isnan(donnees.humidity) || donnees.humidity < 0)
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
    if (isnan((float)donnees.co2) || donnees.co2 < 0)
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
    capteurs.donnees.presence = HIGH;
}

bool Capteurs::getPresence()
{
    donnees.presence = digitalRead(PIR_PIN);
    /*
    if (donnees.presence != donnees.lastPresence)
    {
        if (donnees.presence == HIGH)
        {
            dernierePresence = millis();
            counterNbPresence();
        }
    }
    donneees.lastPresence = donnees.presence;
    */
    return donnees.presence;
}

int Capteurs::counterNbPresence()
{
    return donnees.nbPresence++;
}

// Remise à zéro du compteur de nombre de fronts montant du détecteur de présence
void Capteurs::RAZNbPresence()
{
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
        isOk = false;
    }

    if (getPresence())
    {
        capteurs.donnees.presence = HIGH;
    }
    else
        capteurs.donnees.presence = LOW;

    return isOk;
}

void Capteurs::evaluateAirQuality()
{
    if (donnees.temperature <= 0 || donnees.humidity <= 0 || donnees.co2 <= 0)
    {
        donnees.indiceQAI = QAI_error;
        if (lastIndiceQAI != donnees.indiceQAI)
        {
            Particle.publish("error", "Invalid data from sensors", PRIVATE);
            dernierChgtQAI = millis();
        }
        donnees.r_value = HIGH;
        donnees.g_value = LOW;
        donnees.b_value = HIGH;
    }
    else
    {
        if ((donnees.humidity > 85) || (donnees.co2 > 2000))
        {
            donnees.indiceQAI = QAI_rouge;
            if (lastIndiceQAI != donnees.indiceQAI)
            {
                Particle.publish("newState", "red", PRIVATE);
                dernierChgtQAI = millis();
            }
            donnees.r_value = HIGH;
            donnees.g_value = LOW;
            donnees.b_value = LOW;
        }
        else if (((h > 65) && (h <= 75)) || ((co2 > 700) && (co2 <= 1400)))
        {
            donnees.indiceQAI = QAI_bleuFonce;
            if (lastIndiceQAI != donnees.indiceQAI)
            {
                Particle.publish("newState", "blue", PRIVATE);
                dernierChgtQAI = millis();
            }
            donnees.r_value = LOW;
            donnees.g_value = LOW;
            donnees.b_value = HIGH;
        }
        else if (((h > 65) && (h <= 75)) || ((co2 > 700) && (co2 <= 1400)))
        {
            donnees.indiceQAI = QAI_bleuClair;
            if (lastIndiceQAI != donnees.indiceQAI)
            {
                Particle.publish("newState", "lightBlue", PRIVATE);
                dernierChgtQAI = millis();
            }
            donnees.r_value = LOW;
            donnees.g_value = HIGH;
            donnees.b_value = HIGH;
        }
        else if ((h <= 65) && (co2 <= 700))
        {
            donnees.indiceQAI = QAI_vert;
            if (lastIndiceQAI != donnees.indiceQAI)
            {
                Particle.publish("newState", "green", PRIVATE);
                dernierChgtQAI = millis();
            }
            donnees.r_value = LOW;
            donnees.g_value = HIGH;
            donnees.b_value = LOW;
        }
    }
    lastIndiceQAI = donnees.indiceQAI;
}
