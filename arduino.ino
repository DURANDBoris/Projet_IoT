/*
#   DURAND Boris
#
#   Fichier du programme pour l'arduino
*/

#include LiquideCristal_I2C lcd(0x27, 16, 2);// définit le type d'écran lcd pour ma part j'utilise un 16 x 2

const int PIN_CAPTEUR_TEMPERATURE =10;
const int PIN_CAPTEUR_HUMIDITE = 11;

void setup() 
{
    Serial.begin(9600);
    lcd.begin(); 
    lcd.backlight();
}

void loop() 
{
    lcd.clear(); 
    lcd.setCursor(0,0); // positionne le curseur à la colonne 1 et à la ligne 2 
    lcd.print("Hello World ! "); 
    delay(1000);
}