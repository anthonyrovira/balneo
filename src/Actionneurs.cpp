#include "Actionneurs.h"
#include "Timing.h"

OledWingAdafruit display;

enum Etat_MENU
{
    MENU_temp = 0, // Affichage des données de température
    MENU_hr = 10,  // Affichage des données d'humidité relative
    MENU_co2 = 20  // Affichage des données de CO2
};
Etat_MENU etat_MENU = MENU_temp;

Actionneurs::Actionneurs()
{
}

void Actionneurs::begin()
{
    /*Particle.publish("IHM", "Actionneurs init", PRIVATE);*/

    // Initialisation de la led RGB
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);

    // Initialisation de l'écran
    display.setup();
    display.clearDisplay();
    display.display();
}

void Actionneurs::standby()
{
    // Particle.publish("IHM", "Switched-off", PRIVATE);

    // Effacement de l'affichage
    display.clearDisplay();
    display.display();
}

void Actionneurs::displayTemp(float temperature)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(F("Temperature : "));
    display.setTextSize(3);
    display.print(temperature);
    display.println(F(" C"));
    display.display();

    timingActionneurs.waitingLoop(TEMPO_MAJ_10SEC);
}

void Actionneurs::displayHr(float humidity)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(F("Humidite : "));
    display.setTextSize(3);
    display.print(humidity);
    // Display percent icon
    display.drawLine(display.width() - 20, display.height(), display.width(),
                     display.height() - 20, WHITE);

    display.drawCircle(display.width() - 16, display.height() - 16, 3, WHITE);

    display.drawCircle(display.width() - 4, display.height() - 4, 3, WHITE);
    display.display();

    timingActionneurs.waitingLoop(TEMPO_MAJ_10SEC);
}

void Actionneurs::displayCo2(int co2)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(F("CO2 : "));
    display.setTextSize(3);
    display.print(co2);
    display.println(F("ppm"));
    display.display();

    timingActionneurs.waitingLoop(TEMPO_MAJ_10SEC);
}

void Actionneurs::redLight(int r_value)
{
    analogWrite(REDPIN, r_value);
    dataActionneurs.etat_LED_rouge = r_value != 0;
}

void Actionneurs::greenLight(int g_value)
{
    g_value >= 1 ? g_value = HIGH : g_value = LOW;

    digitalWrite(GREENPIN, g_value);
    dataActionneurs.etat_LED_verte = g_value != 0;
}

void Actionneurs::blueLight(int b_value)
{
    b_value >= 1 ? b_value = HIGH : b_value = LOW;

    digitalWrite(BLUEPIN, b_value);
    dataActionneurs.etat_LED_bleue = b_value != 0;
}

bool Actionneurs::stateRedLight()
{
    return dataActionneurs.etat_LED_rouge;
}

bool Actionneurs::stateGreenLight()
{
    return dataActionneurs.etat_LED_verte;
}

bool Actionneurs::stateBlueLight()
{
    return dataActionneurs.etat_LED_bleue;
}

void Actionneurs::rgbLight(int r_value, int g_value, int b_value)
{
    analogWrite(REDPIN, r_value);
    digitalWrite(GREENPIN, g_value);
    digitalWrite(BLUEPIN, b_value);

    r_value != 0 ? dataActionneurs.etat_LED_rouge = true : dataActionneurs.etat_LED_rouge = false;
    g_value != 0 ? dataActionneurs.etat_LED_verte = true : dataActionneurs.etat_LED_verte = false;
    b_value != 0 ? dataActionneurs.etat_LED_bleue = true : dataActionneurs.etat_LED_bleue = false;
}

void Actionneurs::blinkLED(int nb, int loopTime)
{
    int ledState = LOW;
    for (int i = 0; i < nb * 2; i++)
    {
        timingActionneurs.waitingLoop(loopTime);
        ledState = !ledState;
        rgbLight(ledState, ledState, ledState);
    }
}

void Actionneurs::fadingLed(int redLed, int greenLed, int blueLed)
{
    int i;

    for (i = 0; i < 255; i += 5)
    {
        redLight(i);
        greenLed == HIGH ? greenLight(HIGH) : greenLight(LOW);
        blueLed == HIGH ? blueLight(HIGH) : blueLight(LOW);

        timingActionneurs.waitingLoop(TEMPO_MAJ_20mSEC);
    }
    for (i = 255; i > 0; i -= 5)
    {
        redLight(i);
        greenLed == HIGH ? greenLight(HIGH) : greenLight(LOW);
        blueLed == HIGH ? blueLight(HIGH) : blueLight(LOW);

        timingActionneurs.waitingLoop(TEMPO_MAJ_20mSEC);
    }
}

void Actionneurs::processLED(int r_value, int g_value, int b_value)
{
    rgbLight(r_value, g_value, b_value);
}

void Actionneurs::processMotor(int indexQAI)
{
    if (indexQAI == (int)QAI_rouge)
    {
        analogWrite(MOTOR_PIN, MAX_SPEED);
    }
    else if (indexQAI == (int)QAI_bleuFonce)
    {
        analogWrite(MOTOR_PIN, INTER2_SPEED);
    }
    else if (indexQAI == (int)QAI_bleuClair)
    {
        analogWrite(MOTOR_PIN, INTER1_SPEED);
    }
    else if (indexQAI == (int)QAI_vert)
    {
        analogWrite(MOTOR_PIN, MIN_SPEED);
    }
    else
    {
        digitalWrite(MOTOR_PIN, LOW);
    }
}