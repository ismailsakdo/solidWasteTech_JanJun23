#include <Wire.h>
#include <virtuabotixRTC.h> //ArduinoRTClibrary (Special for RTC1302 using it) - Set the Time Using RTC by Makuna Library
#include <SPI.h>
#include <SD.h>

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot

const int chipSelect = 10;
File sd_file;

//VCC, GND, MOSI, D10, SCK, MISO ===> Match accordingly from left to right
virtuabotixRTC myRTC(5, 4, 2); //CLK, DAT, RST (RTC1302)


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Wire.begin();

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}


void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  Serial.print("sensor = ");
  Serial.println(sensorValue);
  myRTC.updateTime();
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(String(myRTC.hours)+String(":")+String(myRTC.minutes)+String(":"));
    dataFile.print(String(myRTC.dayofmonth)+String("/")+String(myRTC.month)+String("/")+String(myRTC.year));
    dataFile.println(String("||")+String(sensorValue)+String(";")+String(sensorValue+100));
    dataFile.close();
    }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(5000);
}
