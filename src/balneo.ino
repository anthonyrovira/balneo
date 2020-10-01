/*
 * Project Balneo
 * Description:
 * Author: Rovira Anthony
 * Date: 20/03/2020
 */

#include "pins.h"
#include "capteurs.h"
#include "variables.h"

//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 32 // OLED display height, in pixels

SYSTEM_MODE(MANUAL);    // Connexion automatique au particle cloud.
SYSTEM_THREAD(ENABLED); // Multi thread entre la gestion de la connexion et le programme principal

Capteurs capteurs; // --------- création de la variable structure qui héberge les données des capteurs et leurs méthodes associées.
Ihm ihm;           // --------- création de la variable Ihm qui héberge la structure d'affichage et ses méthodes associées.

// int motorState = 0;
int presence = LOW;
// bool pirValue = false;
byte stateIndicator;
byte lastState = 10;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

void setup()
{
  Particle.variable("temp", t);
  Particle.variable("humidity", h);
  Particle.variable("CO2", co2);
  Particle.variable("presence", presence);

  capteurs.begin();

  blinkLedTest();
  display.setup();
  display.clearDisplay();
  display.display();

  dht.begin();

  delay(100);

  t = getTemperature();
  h = getHumidity();
  co2 = getCo2();

  delay(1000);
  previousMillis = millis();
}

void loop()
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= TEMPO_MAJ_30SEC)
  {
    /* -----||| GETTING DATA FROM SENSORS |||----- */
    t = capteurs.getTemperature();
    h = getHumidity();
    co2 = getCo2();

    if (co2 < 0 || t < 0 || h < 0)
    {
      g_value = LOW;
      r_value, b_value = HIGH;
      stateIndicator = 0;
      if (stateIndicator != lastState)
      {
        Particle.publish("newState", "dataError", PRIVATE);
      }
    }
    else
    {
      if ((h > 85) || (co2 > 2000))
      {
        analogWrite(MOTOR_PIN, MAX_SPEED);
        stateIndicator = 4;
        if (stateIndicator != lastState)
        {
          Particle.publish("newState", "red", PRIVATE);
        }
        // Serial.println(F("Vitesse MAX"));
        r_value = HIGH;
        g_value = LOW;
        b_value = LOW;
      }
      else if (((h > 75) && (h <= 85)) || ((co2 > 1400) && (co2 <= 2000)))
      {
        analogWrite(MOTOR_PIN, INTER2_SPEED);
        stateIndicator = 3;
        if (stateIndicator != lastState)
        {
          Particle.publish("newState", "blue", PRIVATE);
        }
        // Serial.println(F("Vitesse 2"));
        r_value = LOW;
        g_value = LOW;
        b_value = HIGH;
      }
      else if (((h > 65) && (h <= 75)) || ((co2 > 700) && (co2 <= 1400)))
      {
        analogWrite(MOTOR_PIN, INTER1_SPEED);
        stateIndicator = 2;
        if (stateIndicator != lastState)
        {
          Particle.publish("newState", "lightBlue", PRIVATE);
        }
        // Serial.println(F("Vitesse 1"));
        b_value = HIGH;
        r_value = LOW;
        g_value = HIGH;
      }
      else if ((h <= 65) && (co2 <= 700))
      {
        analogWrite(MOTOR_PIN, MIN_SPEED);
        stateIndicator = 1;
        if (stateIndicator != lastState)
        {
          Particle.publish("newState", "green", PRIVATE);
        }
        // Serial.println(F("Vitesse MIN"));
        g_value = HIGH;
        r_value = LOW;
        b_value = LOW;
      }
    }
    lastState = stateIndicator;
    previousMillis = currentMillis;
  }
  // pirValue = digitalRead(PIR_PIN);
  if (digitalRead(PIR_PIN) == HIGH)
  {
    digitalWrite(REDPIN, r_value);
    digitalWrite(GREENPIN, g_value);
    digitalWrite(BLUEPIN, b_value);

    if (presence == LOW)
    {
      presence = HIGH;
      Particle.publish("Motion-Detection", "presence", PRIVATE);
    }
    displayTemp((float)t);
    displayHr((float)h);
    displayCo2((int)co2);
  }
  else
  {
    display.clearDisplay();
    display.display();

    if (presence == HIGH)
    {
      presence = LOW;
      Particle.publish("Motion-Detection", "absence", PRIVATE);
    }
    fadingLed(HIGH, HIGH, HIGH);
    fadingLed(HIGH, HIGH, LOW);
    fadingLed(HIGH, LOW, HIGH);
  }
}
/*
double arrondi(float data)
{
  return (double)((int)(data * pow(10, 2) + .5)) / pow(10, 2);
}

double getTemperature()
{
  // Get Temperature
  float readings = dht.readTemperature();
  double result = arrondi(readings);
  if (isnan(result))
  {
    return -1;
  }
  else
  {
    return result;
  }
}*/

/*double getHumidity()
{
  // Get Humidity
  float readings = dht.readHumidity();
  double result = arrondi(readings);
  if (isnan(result))
  {
    return -1;
  }
  else
  {
    return result;
  }
}

int getCo2()
{
  //Get CO2
  int readings = mhz19b.Read();
  return readings;
}

void blinkLedTest()
{
  int ledState = LOW;
  int coutdown = 20;
  int prevMillis = 0;
  while (coutdown != 0)
  {
    int curMillis = millis();
    if (curMillis - prevMillis >= 500)
    {
      prevMillis = curMillis;
      if (ledState == LOW)
      {
        ledState = HIGH;
      }
      else
      {
        ledState = LOW;
      }
      digitalWrite(REDPIN, ledState);
      digitalWrite(GREENPIN, ledState);
      digitalWrite(BLUEPIN, ledState);
      coutdown--;
    }
  }
}
*/
void displayTemp(float temp)
{
  int currentTime = 0;
  int prevTime;
  int tensecs = 10000;
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Temperature : "));
  display.setTextSize(3);
  display.print(temp);
  display.println(F(" C"));
  display.display();
  prevTime = millis();
  while (currentTime - prevTime < tensecs)
  {
    currentTime = millis();
  }
}

void displayHr(float hr)
{
  int currentTime = 0;
  int prevTime;
  int tensecs = 10000;
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Humidite : "));
  display.setTextSize(3);
  display.print(hr);

  display.drawLine(display.width() - 20, display.height(), display.width(),
                   display.height() - 20, WHITE);

  display.drawCircle(display.width() - 16, display.height() - 16, 3, WHITE);

  display.drawCircle(display.width() - 4, display.height() - 4, 3, WHITE);
  display.display();
  prevTime = millis();
  while (currentTime - prevTime < tensecs)
  {
    currentTime = millis();
  }
}

void displayCo2(int ppm)
{
  int currentTime = 0;
  int prevTime;
  int tensecs = 10000;
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("CO2 : "));
  display.setTextSize(3);
  display.print(ppm);
  display.println(F("ppm"));
  display.display();
  prevTime = millis();
  while (currentTime - prevTime < tensecs)
  {
    currentTime = millis();
  }
}
/*
void fadingLed(int Led1, int Led2, int Led3)
{
  int i;
  int i1;
  int i2;
  int i3;
  int currentTime = 0;
  int prevTime;
  int twentymillis = 20;

  for (i = 0; i < 255; i += 5)
  {
    if (Led1 == HIGH)
    {
      i1 = i;
      analogWrite(REDPIN, i1);
    }
    else
    {
      digitalWrite(REDPIN, LOW);
    }
    if (Led2 == HIGH)
    {
      digitalWrite(BLUEPIN, HIGH);
    }
    else
    {
      digitalWrite(BLUEPIN, LOW);
    }
    if (Led3 == HIGH)
    {
      digitalWrite(GREENPIN, HIGH);
    }
    else
    {
      digitalWrite(GREENPIN, LOW);
    }
    prevTime = millis();
    while (currentTime - prevTime < twentymillis)
    {
      currentTime = millis();
    }
  }
  currentTime = 0;
  for (i = 255; i > 0; i -= 5)
  {
    if (Led1 == HIGH)
    {
      i1 = i;
      analogWrite(REDPIN, i1);
    }
    else
    {
      digitalWrite(REDPIN, LOW);
    }
    if (Led2 == HIGH)
    {
      digitalWrite(BLUEPIN, HIGH);
    }
    else
    {
      digitalWrite(BLUEPIN, LOW);
    }
    if (Led3 == HIGH)
    {
      digitalWrite(GREENPIN, HIGH);
    }
    else
    {
      digitalWrite(GREENPIN, LOW);
    }
    prevTime = millis();
    while (currentTime - prevTime < twentymillis)
    {
      currentTime = millis();
    }
  }
}
*/