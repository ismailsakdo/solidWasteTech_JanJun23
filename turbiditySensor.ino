#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h> // library take from here: http://www.rinkydinkelectronics.com/library.php?id=73;
//download the http://www.rinkydinkelectronics.com/download.php?f=DS3231.zip

const int chipSelect = 10;
File sd_file;
DS3231  rtc(SDA, SCL);

//Variable
int sensorPin = A1;
float volt;
float ntu;

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

  pinMode(sensorPin, INPUT);
}

void loop() {

  //Water sensor
  volt = 0;
  for (int i = 0; i < 800; i++)
  {
    volt += ((float)analogRead(sensorPin) / 1023) * 5;
  }
  volt = volt / 800;
  volt = round_to_dp(volt, 2);
  if (volt < 2.5) {
    ntu = 3000;
  } else {
    ntu = -1120.4 * square(volt) + 5742.3 * volt - 4353.8;
  }
  Serial.print("Volt: ");
  Serial.print(volt);
  Serial.print("NTU: ");
  Serial.println(ntu);
  delay(10);

  //Read Date, Time and Sensor
  Serial.print("Date:  ");
  Serial.print(rtc.getDateStr());
  Serial.print(", Time:  ");
  Serial.print(rtc.getTimeStr());
  Serial.print(", NNTU:      ");
  Serial.println(ntu);
  //Serial.print("CO (PPM):       ");
  //Serial.println(co);
  Serial.println("--------------------------------------------------------");


  //Write data on SD card
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(String(rtc.getDateStr()) + String("/") + String(rtc.getTimeStr()));
    dataFile.println(String("||") + String(volt) + String(";") + String(ntu));
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(5000);
}

float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
