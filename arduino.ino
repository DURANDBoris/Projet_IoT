/*
#   DURAND Boris
#
#   Fichier du programme pour l'arduino
*/


// Library Inclusions
#include <SparkFun_HIH4030.h>
#include <Wire.h>
#include "rgb_lcd.h"

//color LCD
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// Example Variables
int tempAddress = 0x48;             // I2C: TMP102 Slave Address with ADD0 Grounded
float temp = 0;                     // Temperature Value either Static or Sensor Reading

// Are You Using a Temperature Sensor? 1 = YES / 0 = NO
int tempSensor = 0;

// Analog IO Pin Connected to OUT
#define HIH4030_OUT A0

// Supply Voltage - Typically 5 V
#define HIH4030_SUPPLY 5

// Library Variables
HIH4030 sensorSpecs(HIH4030_OUT, HIH4030_SUPPLY);

void setup(void){
  
  Serial.begin(9600);               // Serial Port
  Wire.begin();                     // I2C: Utilized by TMP102 Temperature Sensor
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  delay(1000);
}

/* Serial Output of Temperature C, Sensor Voltage V, SensorRH %, and TrueRH % */
void printData(HIH4030 sensor, float temperature){
  lcd.setCursor(0, 0);
  lcd.print("Tempera = ");
  lcd.print(temperature);
  lcd.print(" C ("); Serial.print((temperature * 9/5) + 32); Serial.println(" F)");
  Serial.print("Sensor Voltage = ");
  Serial.print(sensor.vout());
  Serial.println(" V");
  lcd.setCursor(0, 1);
  lcd.print("Humidity = ");
  lcd.print(sensor.getSensorRH());
  lcd.println(" %");/*
  lcd.print("Humidity = ");
  lcd.print(sensor.getTrueRH(temperature));
  lcd.print(" %");*/
  
}

void loop(void){
  
  /*  IF tempSensor = 1 Utilizing a Temperature Sensor             */
  /*  IF tempSensor = 0 Utilizing a Static Value for Temperature   */
  if (tempSensor == 1) {
    temp = getTemperature();        // Get Temperature Sensor Reading
  } else if (tempSensor == 0) {
    temp = 25;                      // Static Temperature Value in C
                                    // Set by User to Desired Temperature                            
  } else {
    while (tempSensor != 1 || tempSensor != 0){
      Serial.println("ERROR: tempSensor Value Out of Range");
    }
  }
  // (note: line 1 is the second row, since counting begins with 0):
  
  printData(sensorSpecs, temp);     // Print Sensor Readings
  Serial.println("");               // Return Space Between Readings
  delay(1000);                       // Slow Down Serial Output
  
}

/* Only Called IF Temperature Sensor is Being Used */
float getTemperature(){

  /*  IF Temperature Sensor is different from the TMP102 and not   */      
  /*    utilizing I2C via the Wire.h library, replace the code     */ 
  /*    within the getTemperature() function.                      */ 
  
  Wire.requestFrom(tempAddress,2);  // Wire.requestFrom(address, # of bytes to request)

  byte MSB = Wire.read();           // Receive byte as MSB or left-most bit
  byte LSB = Wire.read();           // Receive byte as LSB or right-most bit

  /*  TMP102 Temperature Register 12-bit, read-only, two's compliment for negative */
  /*  << is bit shift left ; >> is bit shift right ; | is bitwise OR               */
  /*  Syntax:   variable << number_of_bits                                         */
  /*            variable >> number_of_bits                                         */
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

  float celsius = TemperatureSum*0.0625;  //One LSB equals 0.0625 C
  return celsius;
  
}