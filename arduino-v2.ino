/*
#   DURAND Boris
#
#   Fichier du programme pour l'arduino
#
#   Le programme permettra la l'affichage des données sur un écran tous en enregistrant 
#   les données sur une sd et les transmettra à une passerelle (raspberry)
*/

//Bibliotéque incluse
#include <SparkFun_HIH4030.h>
#include <Wire.h>
#include "rgb_lcd.h"

//Définition : on défini la partie b avec la partie a exemple(#define a b;)
#define HIH_4030_OUT A0
#define HIH_4030_TENSION 5
#define TMP36_OUT A1
#define BOUTTON_ECRAN_MENU 8

//Déclaration du capteur HIH_4030
HIH4030 sensorSpecs(HIH_4030_OUT, HIH_4030_TENSION);

//Déclaration de l'écran LCD
rgb_lcd lcd;

// RGB de l'écran
const int           colorR = 255;
const int           colorG = 0;
const int           colorB = 0;

//Choix du menu écran
int choix_ecran_menu = 1;
//Données Relevées
float               Temperature;
float               Humidite;
unsigned long long  Time_unix;

//Initialisation de l'arduino
void setup()
{
    lcd.begin(16,2);
    pinMode(BOUTTON_ECRAN_MENU, INPUT);
    digitalRead(BOUTTON_ECRAN_MENU);
    delay(1000);
}

//fonction d'affichage des données sur le LCD
void afficher_temperature(float temperature)
{
    /*lcd.setCursor(0,0);
    lcd.print(time_unix);*/
    lcd.setRGB(colorR, colorG, colorB);
    lcd.setCursor(0,0);
    lcd.print("Temperature : ");
    lcd.setCursor(0,1);
    lcd.print(temperature);
    lcd.print(" C");
}

void afficher_humidite(HIH4030 sensor, float temperature)
{
    lcd.setRGB(colorR, 255, colorB);
    lcd.setCursor(0,0);
    lcd.print("Humidite :       ");
    lcd.setCursor(0,1);
    lcd.print(sensor.getTrueRH(temperature));
    lcd.print(" %");
}

void afficher_date()
{
    lcd.setRGB(colorR, colorG, 255);
    lcd.setCursor(0,0);
    lcd.print("Hello world !");
    lcd.setCursor(0,1);
    lcd.print("                ");
}

//Boucle de la fonction à boucle infini
void loop()
{
    Temperature = (analogRead(TMP36_OUT)* (5000 / 1024.0)-500)/10;
    if (digitalRead(BOUTTON_ECRAN_MENU)==HIGH)//test si bouton appuyé
    {
      if(choix_ecran_menu > 3)
      {
        choix_ecran_menu = 0;
      }
      choix_ecran_menu++;
    }
    switch(choix_ecran_menu)
    {
      case 1 :
        afficher_temperature(Temperature);
        break;
      case 2 :
        afficher_humidite(sensorSpecs, Temperature);
        break;
      case 3 :
        afficher_date();
        break;
    }
    delay(1000);
}