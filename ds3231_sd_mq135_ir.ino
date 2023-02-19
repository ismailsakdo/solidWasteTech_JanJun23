#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h> // library take from here: http://www.rinkydinkelectronics.com/library.php?id=73;
//download the http://www.rinkydinkelectronics.com/download.php?f=DS3231.zip
#include <MQUnifiedsensor.h>

//intialize pin
#define Board ("Arduino NANO")
#define Pin135 (A2)  //Analog input 2 of your arduino
//if relevant
//#define Pin7 (35)    //Analog input 3 of your arduino

const int chipSelect = 10;
File sd_file;
DS3231  rtc(SDA, SCL);
int IR_SENSOR_PIN = 9;

//setup of sensor MQ135
#define RatioMQ135CleanAir (3.6)  //RS / R0 = 10 ppm

//if relevant to be use - uncomment
//#define RatioMQ7CleanAir (27.5)   //RS / R0 = 27.5 ppm

#define ADC_Bit_Resolution (10)   // 10 bit ADC
#define Voltage_Resolution (5)    // Volt resolution to calc the voltage
#define Type ("Arduino Uno")     //Board used

//Initialize MQ
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin135, Type);
//if relevant to be use - uncomment
//MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin7, Type);

//Variable
float co2;

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
  MQ135.init();
  MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ135.setR0(9.03);

  /*
   * if include the MQ7 uncomment accordingly this notation
  MQ7.init();
  MQ7.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ7.setR0(5.90);
  */

  pinMode(IR_SENSOR_PIN, INPUT);
}


void loop() {
  //MQ Sensor
  MQ135.update();
  //MQ7.update();

  MQ135.setA(102.2);
  MQ135.setB(-2.473);  //NH4
  float nh4 = MQ135.readSensor();
  /*
  MQ7.setA(99.042);
  MQ7.setB(-1.518);  //CO
  float co = MQ7.readSensor();
  */

  //IR sensor
  int IR_sensor_value = digitalRead(IR_SENSOR_PIN);
  String IR_status = (IR_SENSOR_PIN == HIGH) ? "Detected" : "Not detected";

  //Read Date, Time and Sensor
  Serial.print("Date:  ");
  Serial.print(rtc.getDateStr());
  Serial.print(", Time:  ");
  Serial.print(rtc.getTimeStr());
  Serial.print(", NH4 (PPM):      ");
  Serial.println(nh4);
  //Serial.print("CO (PPM):       ");
  //Serial.println(co);
  Serial.print("IR Sensor Status: ");
  Serial.println(IR_sensor_value);
  Serial.println("--------------------------------------------------------");


  //Write data on SD card
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    //dataFile.print(String(myRTC.hours)+String(":")+String(myRTC.minutes)+String(":"));
    //dataFile.print(String(myRTC.dayofmonth)+String("/")+String(myRTC.month)+String("/")+String(myRTC.year));
    dataFile.print(String(rtc.getDateStr()) + String("/") + String(rtc.getTimeStr()));
    dataFile.println(String("||") + String(nh4) + String(";") + IR_sensor_value);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(1000);
}
