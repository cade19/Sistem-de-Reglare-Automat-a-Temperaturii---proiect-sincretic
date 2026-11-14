/*Pas 5:
    Implementari stari (BEFORE,HEAT,KEEP,COOL,FINI) pentru controlul becului
    prin MOFSET IRF520, temporizare cu millis(), si reglaj cu oprireClipocit
    pentru stabilizarea temperaturii
*/
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int SenzCald = A0;

const int btnUp     = 6;
const int btnDown   = 7;
const int btnSelect = 8;
const int btnStart  = 9;

const int pinMOSFET = 10; 

int GettoTemp = 40;
int KeepSec = 60;     
const float oprireClipocit = 0.8; //prevenirea clipociri becului la atingerea pragului

enum StareSistem {
  STARE_BEFORE,
  STARE_HEAT,
  STARE_KEEP,
  STARE_COOL,
  STARE_FINI
};

StareSistem stareCurenta = STARE_BEFORE;

unsigned long StarttoKeep = 0;
unsigned long RemainingtoKeep = 0;
bool ultimulStatusStart = HIGH;

void setup() 
{
  lcd.begin(16, 2);
  
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);
  pinMode(btnStart, INPUT_PULLUP);

  pinMode(pinMOSFET, OUTPUT);
  digitalWrite(pinMOSFET, LOW);

  lcd.clear();
}

void loop() 
{
  int val = analogRead(SenzCald);
  float tempR = (val * (5000.0 / 1024.0)) / 10.0;

  int citireStart = digitalRead(btnStart);
  if (citireStart == LOW && ultimulStatusStart == HIGH) 
  {
    if (stareCurenta == STARE_BEFORE || stareCurenta == STARE_FINI) 
    {
      stareCurenta = STARE_HEAT; 
    } 
    else 
    {
      stareCurenta = STARE_BEFORE; 
      digitalWrite(pinMOSFET, LOW);
    }
    delay(200); 
  }
  ultimulStatusStart = citireStart;

  if (stareCurenta == STARE_BEFORE) 
  {
    if (digitalRead(btnUp) == LOW) 
    {
      GettoTemp++;
      delay(150);
    }
    if (digitalRead(btnDown) == LOW) 
    {
      GettoTemp--;
      delay(150);
    }
  }

  switch (stareCurenta) 
  {
    case STARE_BEFORE:
      digitalWrite(pinMOSFET, LOW);
      break;

    case STARE_HEAT:
      digitalWrite(pinMOSFET, HIGH);
      if (tempR >= GettoTemp) 
      {
        stareCurenta = STARE_KEEP;
        StarttoKeep = millis(); 
      }
      break;

    case STARE_KEEP:
      {
        unsigned long secundeScurse = (millis() - StarttoKeep) / 1000;
        if (secundeScurse >= KeepSec) 
        {
          stareCurenta = STARE_COOL;
          digitalWrite(pinMOSFET, LOW);
        } 
        else 
        {
          RemainingtoKeep = KeepSec - secundeScurse;
          if (tempR < (GettoTemp - oprireClipocit)) 
          {
            digitalWrite(pinMOSFET, HIGH);
          } 
          else if (tempR >= GettoTemp) 
          {
            digitalWrite(pinMOSFET, LOW);
          }
        }
      }
      break;

    case STARE_COOL:
      digitalWrite(pinMOSFET, LOW);
      if (tempR <= 30.0) 
      {
        stareCurenta = STARE_FINI;
      }
      break;

    case STARE_FINI:
      digitalWrite(pinMOSFET, LOW);
      break;
  }

  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(GettoTemp);
  lcd.print((char)223);
  lcd.print("C ");

  lcd.setCursor(8, 0);
  switch (stareCurenta) 
  {
    case STARE_BEFORE: lcd.print("[BE4 ]  "); break;
    case STARE_HEAT: lcd.print("[HEAT]  "); break;
    case STARE_KEEP: 
      lcd.print("[KEEP");
      if (RemainingtoKeep < 10) lcd.print(" ");
      lcd.print(RemainingtoKeep);
      lcd.print("s]");
      break;
    case STARE_COOL: lcd.print("[COOL]  "); break;
    case STARE_FINI: lcd.print("[FINI]  "); break;
  }

  lcd.setCursor(0, 1);
  lcd.print("Real: ");
  lcd.print(tempR, 1);
  lcd.print((char)223);
  lcd.print("C    ");

  delay(60);
}