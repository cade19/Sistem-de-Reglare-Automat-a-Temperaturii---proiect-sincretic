#include <LiquidCrystal.h>

const int rs=12, en=11, d4=5, d5=4, d6=3, d7=2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
const int SenzCald= A0;
void setup() 
{
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sistem Termic");
  lcd.setCursor(0,1);
  lcd.print("Init Senz ...");
  delay(1000);
  lcd.clear;
}

void loop() 
{
  int val= analogRead(SenzCald);
  float tensiune= val*(5000.0/1024.0);
  float temp= tensiune/10.0;
  lcd.setCursor(0,0);
  lcd.print("Proba Senz");
  lcd.SetCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(temp,1);
  lcd.print((char)233);
  lcd.print("C   ");
  delay(500);
}
