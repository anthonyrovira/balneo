/*
 * Project Balneo-v2
 * Description:
 * Author: Rovira Industries
 * Date: 20/03/2020
 */

#include <Adafruit_Sensor.h>
#include <MH-Z19B-CO2-Sensor-Serial.h>
//#include <DHT.h>
#include <PietteTech_DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <oled-wing-adafruit.h>

#define redpin 2
#define bluepin 4
#define greenpin 5
#define DHT22_PIN 6
#define PIR_PIN 7
#define MOTOR_PIN 3
#define MAX_SPEED 214
#define MIN_SPEED 20
#define INTER1_SPEED 95
#define INTER2_SPEED 170
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 32 // OLED display height, in pixels

SYSTEM_THREAD(ENABLED);

OledWingAdafruit display;

PietteTech_DHT dht(DHT22_PIN, DHT22);

MHZ19BCO2SensorSerial<USARTSerial> mhz19b(Serial1);

double h = 0.0;
double t = 0.0;
int co2 = 0;
int motorState = 0;
int pirState = LOW;
bool pirValue = false;
byte stateIndicator;
byte lastState = 10;
int r_value = HIGH;
int g_value = HIGH;
int b_value = HIGH;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned int interval = 30000;

void setup() {
  // Serial.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(DHT22_PIN, INPUT);
  pinMode(PIR_PIN, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIR_PIN), state, CHANGE);

  blinkLedTest();

  display.setup();
  display.clearDisplay();
  display.display();

  dht.begin();

  Particle.variable("temp", t);
  Particle.variable("humidity", h);
  Particle.variable("CO2", co2);
  Particle.variable("presence", pirState);

  delay(100);

  t = getTemperature();
  h = getHumidity();
  co2 = getCo2();

  delay(1000);
  previousMillis = millis();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    /* -----||| GETTING DATA FROM SENSORS |||----- */
    t = getTemperature();
    h = getHumidity();
    co2 = getCo2();

    if (co2 < 0 || t < 0 || h < 0) {
      g_value = LOW;
      r_value, b_value = HIGH;
      stateIndicator = 0;
      if (stateIndicator != lastState) {
        Particle.publish("newState", "dataError", PRIVATE);
      }
    } else {
      if ((h <= 65) && (co2 <= 700)) {
        analogWrite(MOTOR_PIN, MIN_SPEED);
        stateIndicator = 1;
        if (stateIndicator != lastState) {
          Particle.publish("newState", "green", PRIVATE);
        }
        // Serial.println(F("Vitesse MIN"));
        g_value = HIGH;
        r_value = LOW;
        b_value = LOW;
      }
      if (((h > 65) && (h <= 75)) || ((co2 > 700) && (co2 <= 1400))) {
        analogWrite(MOTOR_PIN, INTER1_SPEED);
        stateIndicator = 2;
        if (stateIndicator != lastState) {
          Particle.publish("newState", "lightBlue", PRIVATE);
        }
        // Serial.println(F("Vitesse 1"));
        b_value = HIGH;
        r_value = LOW;
        g_value = HIGH;
      }
      if (((h > 75) && (h <= 85)) || ((co2 > 1400) && (co2 <= 2000))) {
        analogWrite(MOTOR_PIN, INTER2_SPEED);
        stateIndicator = 3;
        if (stateIndicator != lastState) {
          Particle.publish("newState", "blue", PRIVATE);
        }
        // Serial.println(F("Vitesse 2"));
        r_value = LOW;
        g_value = LOW;
        b_value = HIGH;
      }
      if ((h > 85) || (co2 > 2000)) {
        analogWrite(MOTOR_PIN, MAX_SPEED);
        stateIndicator = 4;
        if (stateIndicator != lastState) {
          Particle.publish("newState", "red", PRIVATE);
        }
        // Serial.println(F("Vitesse MAX"));
        r_value = HIGH;
        g_value = LOW;
        b_value = LOW;
      }
    }
    lastState = stateIndicator;
    previousMillis = currentMillis;
    // attachInterrupt(digitalPinToInterrupt(PIR_PIN), state, CHANGE);
  }
  // pirValue = digitalRead(PIR_PIN);
  if (digitalRead(PIR_PIN) == HIGH) {
    digitalWrite(redpin, r_value);
    digitalWrite(greenpin, g_value);
    digitalWrite(bluepin, b_value);

    if (pirState == LOW) {
      pirState = HIGH;
      Particle.publish("Motion-Detection", "presence", PRIVATE);
    }
    displayTemp((float)t);
    displayHr((float)h);
    displayCo2((int)co2);
  } else {
    display.clearDisplay();
    display.display();

    if (pirState == HIGH) {
      pirState = LOW;
      Particle.publish("Motion-Detection", "absence", PRIVATE);
    }
    fadingLed(HIGH, HIGH, HIGH);
    fadingLed(HIGH, HIGH, LOW);
    fadingLed(HIGH, LOW, HIGH);
  }
}

double getTemperature() {
  // Get Temperature
  float readings1 = dht.readTemperature();
  // float readings1 = dht.getCelsius();
  if (isnan(readings1)) {
    return -1;
  } else {
    return (double)readings1;
  }
}

double getHumidity() {
  // Get Humidity
  float readings2 = dht.readHumidity();
  if (isnan(readings2)) {
    return -1;
  } else {
    return (double)readings2;
  }
}

int getCo2() {
  int readings = mhz19b.Read();
  return readings;
}
/*
void state() {
  // -----||| PRESENCE DETECTION |||-----
  if (pirValue == true) {
    if (pirState == LOW) {
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH) {
      pirState = LOW;
    }
  }
  //detachInterrupt(digitalPinToInterrupt(PIR_PIN));
}
*/

void blinkLedTest() {
  int ledState = LOW;
  int coutdown = 20;
  int prevMillis = 0;
  while (coutdown != 0) {
    int curMillis = millis();
    if (curMillis - prevMillis >= 500) {
      prevMillis = curMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(redpin, ledState);
      digitalWrite(greenpin, ledState);
      digitalWrite(bluepin, ledState);
      coutdown--;
    }
  }
}

void displayTemp(float temp) {
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
  while (currentTime - prevTime < tensecs) {
    currentTime = millis();
  }
}

void displayHr(float hr) {
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
  while (currentTime - prevTime < tensecs) {
    currentTime = millis();
  }
}

void displayCo2(int ppm) {
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
  while (currentTime - prevTime < tensecs) {
    currentTime = millis();
  }
}

void fadingLed(int Led1, int Led2, int Led3) {
  int i;
  int i1;
  int i2;
  int i3;
  int currentTime = 0;
  int prevTime;
  int twentymillis = 20;

  for (i = 0; i < 255; i += 5) {
    if (Led1 == HIGH) {
      i1 = i;
      analogWrite(redpin, i1);
    } else {
      digitalWrite(redpin, LOW);
    }
    if (Led2 == HIGH) {
      digitalWrite(bluepin, HIGH);
    } else {
      digitalWrite(bluepin, LOW);
    }
    if (Led3 == HIGH) {
      digitalWrite(greenpin, HIGH);
    } else {
      digitalWrite(greenpin, LOW);
    }
    prevTime = millis();
    while (currentTime - prevTime < twentymillis) {
      currentTime = millis();
    }
  }
  currentTime = 0;
  for (i = 255; i > 0; i -= 5) {
    if (Led1 == HIGH) {
      i1 = i;
      analogWrite(redpin, i1);
    } else {
      digitalWrite(redpin, LOW);
    }
    if (Led2 == HIGH) {
      digitalWrite(bluepin, HIGH);
    } else {
      digitalWrite(bluepin, LOW);
    }
    if (Led3 == HIGH) {
      digitalWrite(greenpin, HIGH);
    } else {
      digitalWrite(greenpin, LOW);
    }
    prevTime = millis();
    while (currentTime - prevTime < twentymillis) {
      currentTime = millis();
    }
  }
}
