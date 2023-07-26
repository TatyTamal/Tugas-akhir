#include <Servo.h>
#include <LiquidCrystal_I2C.h>

const int trigPin = 2;               
const int echoPin = 3;              
const int servoPin = 9;              
const int lcdColumns = 16;           
const int lcdRows = 2;               
const int lcdAddress = 0x27;         
const int distanceThreshold = 30;    
const int servoAngle = 90;           
const unsigned long thankYouTime = 5000;  
const int backlightBrightness = 255;  

Servo servo;                          
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);  
long duration, distance;               
bool isObjectDetected = false;        
unsigned long thankYouTimer = 0;       

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
  servo.write(0);                     
  lcd.begin(lcdColumns, lcdRows);
  lcd.setBacklight(backlightBrightness);
  lcd.setCursor(0, 0);
  lcd.print("Buanglah sampah");
  lcd.setCursor(0, 1);
  lcd.print("pada tempatnya");
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.035 / 2;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < distanceThreshold) {
    if (!isObjectDetected) {
      openLid();
      isObjectDetected = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Terimakasih sudah");
      lcd.setCursor(0, 1);
      lcd.print("membuang sampah");
      thankYouTimer = millis();
    }
  } else {
    if (isObjectDetected && (millis() - thankYouTimer) >= thankYouTime) {
      closeLid();
      isObjectDetected = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Buanglah sampah");
      lcd.setCursor(0, 1);
      lcd.print("pada tempatnya");
    }
  }

  delay(1000);
}

void openLid() {
  servo.write(servoAngle);  
}

void closeLid() {
  servo.write(0); 
  lcd.setBacklight(backlightBrightness);  
}
