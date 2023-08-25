const int dir1PinA = 7;
const int dir2PinA = 6;
const int dir3PinA = 5;
const int dir4PinA = 4;
const int enA = 3;
int count = 100;

void setup() {
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(dir3PinA, OUTPUT);
  pinMode(dir4PinA, OUTPUT);
  pinMode(enA, OUTPUT);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  digitalWrite(dir3PinA, LOW);
  digitalWrite(dir4PinA, HIGH);
  Serial.begin(9600);
}
void loop() {
  analogWrite(enA, count);
  Serial.println(count);

}
