#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h> // library take from here: http://www.rinkydinkelectronics.com/library.php?id=73;
//download the http://www.rinkydinkelectronics.com/download.php?f=DS3231.zip

//intialize pin
#define PinMoisture (A0)  //Analog input 2 of your arduino

// Constant for dry sensor
const int DryValue = 1022;
// Constant for wet sensor
const int WetValue = 619;

const int chipSelect = 10;
File sd_file;
DS3231  rtc(SDA, SCL);

// Variables for soil moisture
int soilMoistureValue;
int soilMoisturePercent;

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

  //Hidupkan Sensor
  pinMode(PinMoisture, INPUT);
}


void loop() {

  // Get soil mositure value
  soilMoistureValue = analogRead(PinMoisture);

  // Print to serial monitor
  Serial.print(soilMoistureValue);
  Serial.print(" - ");

  // Determine soil moisture percentage value
  soilMoisturePercent = map(soilMoistureValue, DryValue, WetValue, 0, 100);

  // Keep values between 0 and 100
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

  //Read Date, Time and Sensor
  Serial.print("Date:  ");
  Serial.print(rtc.getDateStr());
  Serial.print(", Time:  ");
  Serial.print(rtc.getTimeStr());
  Serial.print(", Moisture Status: ");
  Serial.println(soilMoistureValue);
  Serial.print(", Moisture Percent: ");
  Serial.println(soilMoisturePercent);
  Serial.println("--------------------------------------------------------");


  //Write data on SD card
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(String(rtc.getDateStr()) + String("/") + String(rtc.getTimeStr()));
    dataFile.println(String("||") + String(soilMoistureValue) + String(";") + String(soilMoisturePercent));
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(5000);
}
