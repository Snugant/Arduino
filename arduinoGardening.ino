#include <DFRobot_DS1307.h>
#include <SD.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int prevMins = -1;
int prevHours = -1;
const int CSPin = 10;
const int dir1PinA = 7;
const int dir2PinA = 6;
const int dir3PinA = 5;
const int dir4PinA = 4;
const int enA = 3;
const int DHTPIN = 2;
const int moistPin = A3;
int count = 100;
int moistureRead() {

  digitalWrite(moistPin, HIGH);
  delay(10);
  int reading = map(analogRead(moistPin), 0, 800, 0, 100);
  delay(10);
  digitalWrite(moistPin, LOW);
  return reading;
}
byte dat[5];

float value;
float moisture;

#define DHTTYPE DHT11

LiquidCrystal_I2C lcd1(0x27, 16, 2);

DHT dht(DHTPIN, DHTTYPE);

File myFile;

byte read_data() {
  byte i = 0;
  byte result = 0;

  for (i = 0; i < 8; i++) {
    while (digitalRead(DHTPIN) == LOW)
      ;
    delayMicroseconds(30);
    if (digitalRead(DHTPIN) == HIGH)
      result |= (1 << (8 - i));
    while (digitalRead(DHTPIN) == HIGH)
      ;
  }
  return result;
}

DFRobot_DS1307 DS1307;

void setup() {

  moisture = analogRead(moistPin);
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();

  dht.begin();
  lcd1.init();
  lcd1.backlight();

  pinMode(13, INPUT);
  pinMode(8, INPUT);
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(dir3PinA, OUTPUT);
  pinMode(dir4PinA, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(DHTPIN, OUTPUT);
  pinMode(CSPin, OUTPUT);
  pinMode(moistPin, INPUT);


  Serial.begin(9600);

  while (!(DS1307.begin())) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  Serial.print("Initializing SD card...");
  if (!SD.begin(CSPin)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("Data.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Writing to test.txt...");


    Serial.println("done.");
  } else {
    Serial.println("error opening Data.txt");
  }
}

void loop() {

    if (digitalRead(13) == HIGH) {
    digitalWrite(dir4PinA, HIGH); 
    delay(5000);
  }  else if (moisture >= 200) {
    digitalWrite(dir4PinA, HIGH);
    delay(5000);
  } else if (temp >= 30) {
    digitalWrite(dir2PinA, HIGH);
    delay(5000);
  } else {
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir4PinA, LOW);
    return;
  }
  

  uint16_t getTimeBuff[7] = { 0 };
  DS1307.getTime(getTimeBuff);
  char outputarr[128];
  sprintf(outputarr, "time: %d/%d/%d-%d %d:%d:%d\r\n",
          getTimeBuff[6],
          getTimeBuff[5],
          getTimeBuff[4],
          getTimeBuff[3],
          getTimeBuff[2],
          getTimeBuff[1],
          getTimeBuff[0]);

  int mins = getTimeBuff[1];

  analogWrite(enA, count);

  lcd1.setCursor(0, 0);
  lcd1.print("Moist = ");
  lcd1.print(moisture);

  lcd1.setCursor(0, 1);
  lcd1.print("Temp = ");
  lcd1.print((float)temp, 1);
  lcd1.print("C");

  if (mins != prevMins && (mins == 0 || mins == 15)) {
    prevMins = mins;
  }

  int hours = getTimeBuff[2];

  if (hours != prevHours) {
    prevHours = hours;

    myFile = SD.open("DATA.txt", FILE_WRITE);
    Serial.println("Writing data");
    myFile.print(outputarr);
    myFile.print(moisture);
    myFile.print(", ");
    myFile.print((float)temp, 1);
    myFile.print(", ");
    myFile.println((float)humid, 1);
    myFile.close();
    Serial.println("Closed");
  }
}
