#include <SD.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// data order:
// Moisture, Humidity, Temperature, Time


#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

unsigned long currentMillis = 0;
const int CSPin = 10;
const int msV = 9;
const int dir1PinA = 7;
const int dir2PinA = 6;
const int dir3PinA = 5;
const int dir4PinA = 4;
const int enA = 3;
const int DHTPIN = 2;
const int moistPin = A3;
bool something = false;
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

LiquidCrystal_I2C lcd1(0x3F, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);

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



void setup() {
  dht.begin();
  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();

  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(dir3PinA, OUTPUT);
  pinMode(dir4PinA, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(DHTPIN, OUTPUT);
  pinMode(CSPin, OUTPUT);
  pinMode(moistPin, INPUT);


  Serial.begin(9600);
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
 if(digitalRead(8) == HIGH && lcdOn == 0){
    lcdOn = 1;
    delay(1000);
  }else if (digitalRead(8) == HIGH && lcdOn == 1){
    lcdOn = 0;
    delay(1000);
  }
saver();
if (something == true){
    digitalWrite(msV, HIGH);
    delay(10);
    moisture = map(analogRead(moistPin), 0, 800, 0, 100);
    delay(10);
    digitalWrite(msV, LOW);
}

  float humid = dht.readHumidity();
  float temp = dht.readTemperature();
  analogWrite(enA, count);

  lcd1.setCursor(0, 0);
  lcd1.print("Hum = ");
  lcd1.print((float)humid, 1);
  lcd1.print("%");

  lcd1.setCursor(0, 1);
  lcd1.print("Temp = ");
  lcd1.print((float)temp, 1);
  lcd1.print("C");

//________________________________

  lcd2.setCursor(0, 0);
  lcd2.print("Moist = ");
  lcd2.print(moisture);

  lcd2.setCursor(0, 1);
  lcd2.print("Light = ");

  if (moisture <= 30)
  {
    myFile.print((float)moisture, 1);
    myFile.print(humid);
    myFile.print((float)temp, 1);
    myFile.println();
    time(millis() / 1000);  
    myFile.close();
    digitalWrite(dir3PinA, LOW);
    digitalWrite(dir4PinA, HIGH);
    delay(5000);
  }
  else if (temp >= 30){
    myFile.print((float)moisture, 1);
    myFile.print(humid);
    myFile.print((float)temp, 1);
    time(millis() / 1000);  
    myFile.close();
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
void time(long val){  
 myFile.println();
 int days = elapsedDays(val);
 int hours = numberOfHours(val);
 int minutes = numberOfMinutes(val);
 int seconds = numberOfSeconds(val);
  myFile.print(days,DEC);  
  printDigits(hours);  
  printDigits(minutes);
  printDigits(seconds);
  myFile.println();  
}

void printDigits(byte digits){
  // utility function for digital clock display: prints colon and leading 0
  myFile.print(":");
  if(digits < 10)
  myFile.print('0');
  myFile.print(digits,DEC);   
}

void saver(){
  something = false;
  delay(5000);
  something = true;
  delay(100);
}
