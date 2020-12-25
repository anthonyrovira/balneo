#include "Actionneurs.h"

OledWingAdafruit display;

enum Etat_MENU
{
    MENU_temp = 0, // Affichage des données de température
    MENU_hr = 10,  // Affichage des données d'humidité relative
    MENU_co2 = 20  // Affichage des données de co2
};
Etat_MENU etat_MENU = MENU_temp;

Actionneurs::Actionneurs()
{
}

void Actionneurs::begin()
{
    /*Particle.publish("IHM", "Actionneurs init", PRIVATE);*/

    // Initialisation de la led RGB
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);

    // Initialisation de l'écran
    display.setup();
    display.clearDisplay();
    display.display();
}

void Actionneurs::waitingLoop(unsigned int timeInMs)
{
    unsigned long previousTime = millis();
    bool waiting = false;
    while (millis() <= previousTime + timeInMs)
    {
        waiting = true;
    }
    waiting = false;
}

void Actionneurs::standby()
{
    //Particle.publish("IHM", "Switched-off", PRIVATE);

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

    waitingLoop(TEMPO_MAJ_10SEC);
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

    waitingLoop(TEMPO_MAJ_10SEC);
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

    waitingLoop(TEMPO_MAJ_10SEC);
}

void Actionneurs::redLight(int r_value)
{
    digitalWrite(REDPIN, r_value);

    if (r_value != 0)
    {
        dataActionneurs.etat_LED = true;
    }
    else
        dataActionneurs.etat_LED = false;
}

void Actionneurs::greenLight(int g_value)
{
    digitalWrite(GREENPIN, g_value);

    if (g_value != 0)
    {
        dataActionneurs.etat_LED = true;
    }
    else
        dataActionneurs.etat_LED = false;
}

void Actionneurs::blueLight(int b_value)
{
    digitalWrite(BLUEPIN, b_value);

    if (b_value != 0)
    {
        dataActionneurs.etat_LED = true;
    }
    else
        dataActionneurs.etat_LED = false;
}

void Actionneurs::rgbLight(int r_value, int g_value, int b_value)
{
    digitalWrite(REDPIN, r_value);
    digitalWrite(GREENPIN, g_value);
    digitalWrite(BLUEPIN, b_value);

    if (r_value != 0 && g_value != 0 && b_value != 0)
    {
        dataActionneurs.etat_LED = true;
    }
    else
        dataActionneurs.etat_LED = false;
}

void Actionneurs::blinkLED(int nb, int loopTime)
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
            dataActionneurs.etat_LED = !dataActionneurs.etat_LED;
            ledState = !ledState;
            rgbLight(ledState, ledState, ledState);
        }
    }
}

void Actionneurs::fadingLed(int redLed, int greenLed, int blueLed)
{
    int i;
    int prevTime;

    for (i = 0; i < 255; i += 5)
    {
        if (redLed == HIGH)
        {
            redLight(i);
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

        waitingLoop(TEMPO_MAJ_20mSEC);
    }
    for (i = 255; i > 0; i -= 5)
    {
        if (redLed == HIGH)
        {
            redLight(i);
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

        waitingLoop(TEMPO_MAJ_20mSEC);
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
}