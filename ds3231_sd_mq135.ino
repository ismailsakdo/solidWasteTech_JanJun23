#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>


const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot

const int chipSelect = 10;
File sd_file;
DS3231  rtc(SDA, SCL);


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
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
  rtc.begin(); // Initialize the rtc object
}


void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  //Read Date, Time and Sensor
  Serial.print("Date:  ");
  Serial.print(rtc.getDateStr());
  Serial.print(", Time:  ");
  Serial.print(rtc.getTimeStr());
  Serial.print(", Sensor = ");
  Serial.println(sensorValue);

  //Write data on SD card
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(String(rtc.getDateStr()) + String("/") + String(rtc.getTimeStr()));
    dataFile.println(String("||") + String(sensorValue) + String(";") + String(sensorValue + 100));
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(5000);
}
