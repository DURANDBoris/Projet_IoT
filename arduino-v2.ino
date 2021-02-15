/*
#   DURAND Boris
#
#   Fichier du programme pour l'arduino
*/

//Bibliotéque incluse
#include <SparkFun_HIH4030.h>
#include <Wire.h>
#include "rgb_lcd.h"

//Définition
#define HIH_4030_OUT A0;
#define HIH_4030_TENSION 5;
#define TMP36_OUT A1;

//Déclaration du capteur HIH_4030
HIH_4030 sensorSpecs(HIH_4030_OUT, HIH_4030_TENSION);

//Couleur de l'écran LCD
rgb_lcd lcd;

struct RGB_LCD
{
    const int RED = 255;
    const int GREEN = 0;
    const int BLUE = 0;
};

//Structure des données
struct SE_DONNEE
{
    float Temperature;
    float Humidite;
    unsigned long long Time_unix;
};


void setup()
{
    lcd.begin(20,4);
    lcd.setRGB(RGB_LCD::RED, RGB_LCD::GREEN, RGB_LCD::BLUE);
    delay(1000);
}

void printData(HIH_4030 sensor, float temperature, unsigned long long time_unix)
{
    lcd.setCursor(0,0);
    lcd.print(time_unix);
    lcd.setCursor(0,1);
    lcd.print("Température : ");
    lcd.print(temperature);
    lcd.print(" °C")
    lcd.setCursor(0,2);
    lcd.print("Humidité : ");
    lcd.print(sensor getSensorRH());
    lcd.print(" %");
}

void loop()
{
    printData(sensorSpecs, SE_DONNEE::Temperature, SE_DONNEE::Time_linux);
    delay(100);
}