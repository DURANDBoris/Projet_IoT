/*
#
#   DURAND Boris
#
#   Fichier du programme pour l'arduino (système embarqué du projet)
#
#   Le programme a pour objectif de permettre de relever la température ainsi
#   que l'humidité en fonction de la température prélevé. C'est données seront
#   afficher sur un écran LCD de façon cyclique toute les 5 secondes. Les données
#   relevé seront enregistré sur une carte SD toute les heures en format CSV et JSON.
#   De la façon suivante : DATE-HEURE, TEMPERATURE, HUMIDITE.
#   Il envera aussi ces relevé toutes les heures à une passerelle par l'intérmédiaire
#   d'une communication radio appelé LoRa Wan.
#   Une synchronisation NTP est à prévoir de façon récurante mais pas encore défini.
#   
*/

 

//Bibliotéque incluse
  #include <SparkFun_HIH4030.h>
  #include <Wire.h>
  #include <DS3231.h>
  #include "rgb_lcd.h"

 

//Définition : on défini la partie b avec la partie a exemple(#define a b;)
  #define HIH_4030_OUT A0
  #define HIH_4030_TENSION 5
  #define TMP36_OUT A1

 

//Déclaration du capteur HIH_4030
  HIH4030 sensorSpecs(HIH_4030_OUT, HIH_4030_TENSION);

 

//Horloge
  DS3231 clock;
  RTCDateTime dt;

 

//Déclaration de l'écran LCD
  rgb_lcd lcd;

 

// RGB de l'écran
  const int           colorR = 255;
  const int           colorG = 192;
  const int           colorB = 0;

 

//Choix du menu écran
int choix_ecran_menu = 0;

 

//Données Relevées
  float               Temperature;
  float               Humidite;
  unsigned long long  Time_unix;

 

//Initialisation de l'arduino
  void setup()
  {
      lcd.begin(16,2);
      clock.begin();
      // Manual (YYYY, MM, DD, HH, II, SS);
      // clock.setDateTime(2021, 02, 18, 14, 30, 00);
      // Send sketch compiling time to Arduino
      clock.setDateTime(__DATE__, __TIME__);
      delay(1000);
  }

 

//fonction d'affichage de la température
  void afficher_temperature(float temperature)
  {
    //Choix de la couleur de fond
      lcd.setRGB(colorR, colorG, colorB);
    //Affichage de la première ligne au premier caractère
      lcd.setCursor(0,0);
      lcd.print("Temperature : ");
      lcd.setCursor(0,1);
    //Affichage à la seconde ligne au premier caractère
      lcd.print(temperature);
      lcd.print(" C            ");
  }

 

//fonction d'affichage de l'humiditée
  void afficher_humidite(HIH4030 sensor, float temperature)
  {
      lcd.setRGB(colorR, colorG, colorB);
      lcd.setCursor(0,0);
      lcd.print("Humidite :       ");
      lcd.setCursor(0,1);
      lcd.print(sensor.getTrueRH(temperature));
      lcd.print(" %            ");
  }

 

//fonction d'affichage de la date
  void afficher_date()
  {
      dt = clock.getDateTime();
      lcd.setRGB(colorR, colorG, colorB);
      lcd.setCursor(0,0);
      lcd.print("Date :          ");
      lcd.setCursor(0,1);
      lcd.print(dt.year);lcd.print("-");lcd.print(dt.month);lcd.print("-");lcd.print(dt.day);
      lcd.print(" ");lcd.print(dt.hour);lcd.print(":");lcd.print(dt.minute);
  }

 

//Boucle de la fonction à boucle infini
  void loop()
  {
      Temperature = (analogRead(TMP36_OUT)* (5000 / 1024.0)-500)/10;
  
      //affichage lcd menu automatique
        choix_ecran_menu++;
        
        if(choix_ecran_menu > 3)
        {
          choix_ecran_menu = 0;
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
    //un délai de 5 secondes à été mis
      delay(5000);
  }