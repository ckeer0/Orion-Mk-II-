//Importications
//Orion Mark II Code 
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <MPU6050_tockn.h>


MPU6050 mpu6050(Wire);

//Declaring Variables
int red = 16;
int blue = 14;
int green = 15;

Adafruit_BMP280 bmp;


const int chipSelect = 10   ;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  digitalWrite(blue, LOW);
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  Serial.begin(9600);
  Wire.begin();

  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);


  Serial.println(F("BMP280 test"));




  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500);
  delay(500);

}

void logger(int IA) {
  // make a string for assembling the data to log:
  String dataString = "";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  mpu6050.update();

  dataString = dataString + (String)(bmp.readAltitude(1013.25) - IA);
  dataString += " ";
  dataString = dataString + (String)mpu6050.getAccX();
  dataString += " ";
  dataString = dataString + (String)mpu6050.getAccY();
  dataString += " ";
  dataString = dataString + (String)mpu6050.getAccZ();
  dataString += " ";
  dataString = dataString + (String)mpu6050.getAngleX();
  dataString += " ";
  dataString = dataString + (String)mpu6050.getAngleY();
  dataString += " ";
  dataString = dataString + (String)mpu6050.getAngleZ();
  
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void loop() {
  //Stages
  int initialSetUp = 0;
  int resting = 1;
  int motorIgnition = 2;
  int motorBurnOut = 3;
  int coast = 4;
  int apogee = 5;
  int mainChute = 6;
  int landing = 7;
  int findMe = 8;
  int stageMarker = 0;
  int initialAlt = 0;

  while (stageMarker == initialSetUp) {
    if (!bmp.begin()) {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
      while (1);
    }
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
    }
    initialAlt = bmp.readAltitude(1013.25);
    stageMarker = resting;
  }
  while (stageMarker == resting) {
    logger(initialAlt);
    delay(50);
  }
  while (stageMarker == motorIgnition) {
    stageMarker = motorBurnOut;
  }
  while (stageMarker == motorBurnOut) {
    stageMarker = coast;
  }
  while (stageMarker == coast) {
    stageMarker = apogee;
  }
  while (stageMarker == apogee) {
    stageMarker = mainChute;
  }
  while (stageMarker == mainChute) {
    stageMarker = landing;
  }
  while (stageMarker == landing) {
    stageMarker = findMe;
  }
  while (stageMarker == findMe) {
    stageMarker = 0;
  }
  digitalWrite(blue, HIGH);
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
  while (1) {
    Serial.println("Sucessful");
  }

}
