#include <DFRobot_DS1307.h>
#include <SD.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
const int CSPin = 10;
const int dir1PinA = 7;
const int dir2PinA = 6;
const int dir3PinA = 5;
const int dir4PinA = 4;
const int enA = 3;
const int DHTPIN = 2;
const int moistPin = A3;
int count = 100;
int lcdOn = 0;
int moistureRead(){
    // Turn on our power for the moisture sensor
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
 
byte read_data()
{
  byte i = 0;
  byte result = 0;
 
  for (i = 0; i < 8; i++) {
    while (digitalRead(DHTPIN) == LOW);
    delayMicroseconds(30);
    if (digitalRead(DHTPIN) == HIGH)
      result |= (1 << (8 - i));
    while (digitalRead(DHTPIN) == HIGH);
  }
  return result;
}
 
DFRobot_DS1307 DS1307;
 
void setup() {
  dht.begin();
  lcd1.init();
  lcd1.backlight();
  
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
 
  while( !(DS1307.begin()) ){
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
 
  DS1307.setTypeTime(DS1307.eYR, 2023);
  Serial.print("get type time: ");
  Serial.println(DS1307.getTypeTime(DS1307.eYR));
  DS1307.stop();
  DS1307.start();
  DS1307.setSqwPinMode(DS1307.eSquareWave_1Hz);
 
 
  uint16_t setTimeBuff[7] = {5, 1, 7, 6, 9, 9, 2030};
  DS1307.setTime(setTimeBuff);
 
 
}
 
void loop() {
  moisture = analogRead(moistPin);
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();
  analogWrite(enA, count);
  
  lcd1.setCursor(0, 0);
  lcd1.print("Moist = ");
  lcd1.print(moisture);
 
  lcd1.setCursor(0, 1);
  lcd1.print("Temp = ");
  lcd1.print((float)temp, 1);
  lcd1.print("C");
/* 
  if(digitalRead(8) == HIGH && lcdOn == 0){
    lcdOn = 1;
    delay(3000);
  }else if (digitalRead(8) == HIGH && lcdOn == 1){
    lcdOn = 0;
    delay(3000);
  }

  if(DS1307.eSquareWave_1Hz == DS1307.getSqwPinMode()){
    Serial.print("SQW/OUT pin: 1Hz | ");
  }
  uint16_t getTimeBuff[7] = {0};
  DS1307.getTime(getTimeBuff);
  char outputarr[128];
  sprintf(outputarr, "time: %d/%d/%d-%d %d:%d:%d\r\n",
            getTimeBuff[6],
            getTimeBuff[5],
            getTimeBuff[4],
            getTimeBuff[3],
            getTimeBuff[2],
            getTimeBuff[1],
            getTimeBuff[0]
            );
  Serial.print(outputarr);
 */
 delay(1000);
 
//________________________________
 
 
  if (moisture >= 200)
  {
    digitalWrite(dir3PinA, LOW);
    digitalWrite(dir4PinA, HIGH);
    delay(5000);
  }
  else if (temp >= 30){
    digitalWrite(dir1PinA, LOW);
    digitalWrite(dir2PinA, HIGH);
    delay(5000);
    }else{
    digitalWrite(dir1PinA, LOW);
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir3PinA, LOW);
    digitalWrite(dir4PinA, LOW);            
      return;
    }
  }
 
 
