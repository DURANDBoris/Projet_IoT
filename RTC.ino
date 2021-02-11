/**
 * Exemple de code de lecture et d'ajustement de l'heure avec un module RTC DS1307.
 * Compatible Arduino 0023 et Arduino 1.x (et supérieur).
 */

/* Dépendances */
#include <Wire.h>
#include "DS1307.h"


/* Rétro-compatibilité avec Arduino 1.x et antérieur */
#if ARDUINO >= 100
#define Wire_write(x) Wire.write(x)
#define Wire_read() Wire.read()
#else
#define Wire_write(x) Wire.send(x)
#define Wire_read() Wire.receive()
#endif


/** Fonction de conversion BCD -> decimal */
byte bcd_to_decimal(byte bcd) {
  return (bcd / 16 * 10) + (bcd % 16); 
}

/** Fonction de conversion decimal -> BCD */
byte decimal_to_bcd(byte decimal) {
  return (decimal / 10 * 16) + (decimal % 10);
}


/** 
 * Fonction récupérant l'heure et la date courante à partir du module RTC.
 * Place les valeurs lues dans la structure passée en argument (par pointeur).
 * N.B. Retourne 1 si le module RTC est arrêté (plus de batterie, horloge arrêtée manuellement, etc.), 0 le reste du temps.
 */
byte read_current_datetime(DateTime_t *datetime) {
  
  /* Début de la transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire_write((byte) 0); // Lecture mémoire à l'adresse 0x00
  Wire.endTransmission(); // Fin de la transaction I2C
 
  /* Lit 7 octets depuis la mémoire du module RTC */
  Wire.requestFrom(DS1307_ADDRESS, (byte) 7);
  byte raw_seconds = Wire_read();
  datetime->seconds = bcd_to_decimal(raw_seconds);
  datetime->minutes = bcd_to_decimal(Wire_read());
  byte raw_hours = Wire_read();
  if (raw_hours & 64) { // Format 12h
    datetime->hours = bcd_to_decimal(raw_hours & 31);
    datetime->is_pm = raw_hours & 32;
  } else { // Format 24h
    datetime->hours = bcd_to_decimal(raw_hours & 63);
    datetime->is_pm = 0;
  }
  datetime->day_of_week = bcd_to_decimal(Wire_read());
  datetime->days = bcd_to_decimal(Wire_read());
  datetime->months = bcd_to_decimal(Wire_read());
  datetime->year = bcd_to_decimal(Wire_read());
  
  /* Si le bit 7 des secondes == 1 : le module RTC est arrêté */
  return raw_seconds & 128;
}


/** 
 * Fonction ajustant l'heure et la date courante du module RTC à partir des informations fournies.
 * N.B. Redémarre l'horloge du module RTC si nécessaire.
 */
void adjust_current_datetime(DateTime_t *datetime) {
  
  /* Début de la transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire_write((byte) 0); // Ecriture mémoire à l'adresse 0x00
  Wire_write(decimal_to_bcd(datetime->seconds) & 127); // CH = 0
  Wire_write(decimal_to_bcd(datetime->minutes));
  Wire_write(decimal_to_bcd(datetime->hours) & 63); // Mode 24h
  Wire_write(decimal_to_bcd(datetime->day_of_week));
  Wire_write(decimal_to_bcd(datetime->days));
  Wire_write(decimal_to_bcd(datetime->months));
  Wire_write(decimal_to_bcd(datetime->year));
  Wire.endTransmission(); // Fin de transaction I2C
}


/** Fonction setup() */
void setup() {
  
  /* Initialise le port série */
  Serial.begin(115200);
  
  /* Initialise le port I2C */
  Wire.begin();
  
  /* Vérifie si le module RTC est initialisé */
  DateTime_t now;
  if (read_current_datetime(&now)) {
    Serial.println(F("L'horloge du module RTC n'est pas active !"));
    
    // Reconfiguration avec une date et heure en dure (pour l'exemple)
    now.seconds = 0;
    now.minutes = 0;
    now.hours = 12; // 12h 0min 0sec
    now.is_pm = 0; 
    now.day_of_week = 4;
    now.days = 1; 
    now.months = 12;
    now.year = 16; // 1 dec 2016
    adjust_current_datetime(&now);
  }
}


/** Fonction loop() */
void loop() {
  
  /* Lit la date et heure courante */
  DateTime_t now;
  if (read_current_datetime(&now)) {
    Serial.println(F("L'horloge du module RTC n'est pas active !"));
  }
  
  /* Affiche la date et heure courante */
  Serial.print(F("Date : "));
  Serial.print(now.days);
  Serial.print(F("/"));
  Serial.print(now.months);
  Serial.print(F("/"));
  Serial.print(now.year + 2000);
  Serial.print(F("  Heure : "));
  Serial.print(now.hours);
  Serial.print(F(":"));
  Serial.print(now.minutes);
  Serial.print(F(":"));
  Serial.println(now.seconds);
  
  /* Rafraichissement une fois par seconde */ 
  delay(1000); 
}