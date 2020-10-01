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

    delay(50);

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

int Capteurs::getPresence()
{
    donnees.presence = digitalRead(PIR_PIN);
    return donnees.presence;
}

// Remise à zéro du compteur de nombre de fronts montant du détecteur de présence
void Capteurs::RAZNbPresence()
{
    donnees.nbPresence = 0;
}

void Capteurs::redLight(int r_value)
{
    digitalWrite(REDPIN, r_value);

    if (r_value != 0)
    {
        donnees.etat_LED = 1;
    }
    else
        donnees.etat_LED = 0;
}

void Capteurs::greenLight(int g_value)
{
    digitalWrite(GREENPIN, g_value);

    if (g_value != 0)
    {
        donnees.etat_LED = 1;
    }
    else
        donnees.etat_LED = 0;
}

void Capteurs::blueLight(int b_value)
{
    digitalWrite(BLUEPIN, b_value);

    if (b_value != 0)
    {
        donnees.etat_LED = 1;
    }
    else
        donnees.etat_LED = 0;
}

void Capteurs::rgbLight(int r_value, int g_value, int b_value)
{
    digitalWrite(REDPIN, r_value);
    digitalWrite(GREENPIN, g_value);
    digitalWrite(BLUEPIN, b_value);

    if (r_value != 0 && g_value != 0 && b_value != 0)
    {
        donnees.etat_LED = 1;
    }
    else
        donnees.etat_LED = 0;
}

void Capteurs::blinkLED(int nb, int loopTime)
{
    int ledState = LOW;
    unsigned long previousMillis = 0;
    unsigned long currentMillis = 0;
    for (int i = 0; i < nb * 2; i++)
    {
        currentMillis = millis();
        if (currentMillis - previousMillis >= loopTime)
        {
            previousMillis = currentMillis;
            donnees.etat_LED = !donnees.etat_LED;
            ledState = !ledState;
            rgbLight(ledState, ledState, ledState);
        }
    }
}

void Capteurs::fadingLed(int redLed, int greenLed, int blueLed)
{
    int i;
    int ledValue;
    int currentTime = 0;
    int prevTime;
    int twentymillis = 20;

    for (i = 0; i < 255; i += 5)
    {
        if (redLed == HIGH)
        {
            ledValue = i;
            redLight(ledValue);
        }
        else
            redLight(LOW);
        if (greenLed == HIGH)
        {
            blueLight(HIGH);
        }
        else
            blueLight(LOW);

        if (blueLed == HIGH)
        {
            greenLight(HIGH);
        }
        else
            greenLight(LOW);

        prevTime = millis();
        while (currentTime - prevTime < twentymillis)
        {
            currentTime = millis();
        }
    }
    currentTime = 0;
    for (i = 255; i > 0; i -= 5)
    {
        if (redLed == HIGH)
        {
            ledValue = i;
            redLight(ledValue);
        }
        else
            redLight(LOW);

        if (greenLed == HIGH)
        {
            blueLight(HIGH);
        }
        else
            blueLight(LOW);
        if (blueLed == HIGH)
        {
            greenLight(HIGH);
        }
        else
            greenLight(LOW);
        prevTime = millis();
        while (currentTime - prevTime < twentymillis)
        {
            currentTime = millis();
        }
    }
}