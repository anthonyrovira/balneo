#ifndef PINS_H
#define PINS_H

/* ----- LED RGB -----------
Référence : KY-016
Lien internet : https://arduinomodules.info/ky-016-rgb-full-color-led-module/
------------------------*/
#define redpin 2
#define bluepin 4
#define greenpin 5

/* ----- ECRAN OLED -----------
Référence : MakerHawk OLED Module d'affichage, SPI I2C IIC 128X64 LCD
Lien internet : https://www.amazon.fr/gp/product/B0777HHQDT/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1
------------------------*/

/* ----- Capteur de température et humidité -----------
Référence : DHT22
Lien internet : https://create.arduino.cc/projecthub/mafzal/temperature-monitoring-with-dht22-arduino-15b013
-----------------------------------------------------*/
#define DHT22_PIN 6

/* ----- Capteur de CO2 -----------
Référence : MHZ-19
Lien internet : https://projetsdiy.fr/projet-micropython-publier-taux-co2-capteur-mhz19-domoticz-2/
---------------------------------*/

/* ----- Capteur PIR (présence) -----------
Référence : HC-SR501
Lien internet : https://www.robot-maker.com/shop/blog/27_Connecter-capteur-Infrarouge-HCSR501.html
-----------------------------------------*/
#define PIR_PIN 7

#endif
