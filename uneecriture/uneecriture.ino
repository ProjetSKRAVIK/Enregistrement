#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

File file;
int sensorValue;
const byte SDCARD_CS_PIN = 10; // TODO remplace with the pin of your SDShield


void setup() {
  Serial.begin(9600);
//For SD card
 if(!SD.begin(SDCARD_CS_PIN)) {
 Serial.println(F("Initialisation failed !"));
 return;
 }
 Serial.println(F("Initialisation OK"));
//end SDCard

//For Clock
 #ifdef AVR
 Wire.begin();
 #else
 Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
 #endif
 rtc.begin();

 if (! rtc.isrunning()) {
 Serial.println("RTC failed!");
 rtc.adjust(DateTime(__DATE__, __TIME__));
 }
//End Clock

}

void loop() {

  sensorValue = analogRead(A0);//Replace with the pin of the sensor
  DateTime now = rtc.now();

//Naming the file with the current date (
 char datafile[13]; //table for the name 
 int day=now.day();
 int month = now.month();
 int year= now.year(); 
 sprintf(datafile,"%02d%02d%04d.csv",day,month,year); // %d pour un int 
 datafile[13]='\0';//à mettre pour fermer convenablement le fichier
//end naming phase

//opening file
 file = SD.open(datafile, FILE_WRITE);
 
 //Test pour écriture
 if(file) {
 Serial.println(F("Ecriture en cours"));
 //Ecriture
 write_current_date(file);
 file.print(";");
 file.println(sensorValue);
 file.close();
 }
 //closing file
 
 delay(1000);//delay between measures. Adapt in order to have 1 measure every second  
}

//Function to write in the selected file the date
void write_current_date(File file) {
 DateTime now = rtc.now();
 
 file.print(now.year(), DEC);
 file.print('/');
 file.print(now.month(), DEC);
 file.print('/');
 file.print(now.day(), DEC);
 file.print('_');
 file.print(now.hour(), DEC);
 file.print(':');
 file.print(now.minute(), DEC);
 file.print(':');
 file.print(now.second(), DEC);
}
