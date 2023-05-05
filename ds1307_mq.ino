#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot

const int chipSelect = 10;
File sd_file;
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
  Wire.begin();

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // see if the card is present and can be initialized:
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}


void loop() {
  // read the analog in value:
  DateTime now = rtc.now();
  sensorValue = analogRead(analogInPin);

  //Read Date, Time and Sensor
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.println(sensorValue);

  //Write data on SD card
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(String(now.year()) + String("/") + String(now.month()) + 
    String(now.day()) + String("/") + String(now.hour()) + String("/") + String(now.minute()) + String("/") 
    + String(now.second()) + String("/"));
    dataFile.println(String("||") + String(sensorValue) + String(";") + String(sensorValue + 100));
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("test.txt");
  }
  delay(5000);
}
