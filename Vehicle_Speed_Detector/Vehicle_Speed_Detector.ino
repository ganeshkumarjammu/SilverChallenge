/*****************************************************
  Project Title: Vehicle Speed Detector
  Organisation: Soham Academy Of Human Excellence
  Website: https://ria.sohamacademy.org/
****************************************************/

#include "LiquidCrystal_I2C.h"
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16, 2);
// A4 to LCD_I2C SDA
// A5 to LCD_I2c SCL
const float distance = 10.0;
const int maxSpeed = 60 ;
int timer1;
int timer2;

float Time;

int flag1 = 0;
int flag2 = 0;

float speed;

int ir_s1 = A0; //A0 to IR_Sensor OUT 
int ir_s2 = A1; //A1 to IR_Sensor OUT

int buzzer = 13;//D13 to Buzzer "+"Ve 

void setup() {
  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print(" WELCOME To ");
  lcd.setCursor(0, 1);
  lcd.print("  ROBOTICS");
  delay(1000);
  lcd.clear();
}

void loop() {
  if (digitalRead (ir_s1) == LOW && flag1 == 0) {
    timer1 = millis();
    flag1 = 1;
  }
  if (digitalRead (ir_s2) == LOW && flag2 == 0) {
    timer2 = millis();
    flag2 = 1;
  }
  if (flag1 == 1 && flag2 == 1) {
    if (timer1 > timer2) {
      Time = timer1 - timer2;
    }
    else if (timer2 > timer1) {
      Time = timer2 - timer1;
   }
    Time = (Time / 1000) + 0.25 ; //convert millisecond to second
    speed = (distance / Time); //v=d/t
    speed = speed * 3600; //multiply by seconds per hr
    speed = speed / 1000; //division by meters per Km
  }
  if (speed == 0) {
    lcd.setCursor(0, 1);
    if (flag1 == 0 && flag2 == 0) {
      lcd.print("No car  detected");
    }
    else {
      lcd.print("Searching...    ");
    }
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed:");
    lcd.print(speed, 1);
    lcd.print("Km/Hr  ");
    lcd.setCursor(0, 1);
    if (speed > maxSpeed) {
      lcd.print("  Over Speeding  ");
      digitalWrite(buzzer, HIGH);
    }
    else {
      lcd.print("  Normal Speed   ");
    }
    delay(2000);
    digitalWrite(buzzer, LOW);
    speed = 0;
    flag1 = 0;
    flag2 = 0;
  }
}
