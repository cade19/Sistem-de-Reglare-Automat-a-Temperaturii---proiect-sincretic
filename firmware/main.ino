/*Pas 6:
    Inlocuirea comportamentului de alternare de pe ON pe OFF la reglaj,
    in apropierea pragului, printr-un semnal PWM(0-255).
    Intensitatea becului scade treptat la intrarea in range-ul de 
    [prag dorit-4,prag dorit] pentru eliminarea depasirii dupa atingerea
    pragului, iar temperatura este mediata pe 5 esantioane(5 citiri consecutive
    la interval de 2ms si facerea mediei aritmetice) pentru stabilitate
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
const float bandaProp = 4.0;
const int pwmMinimKeep = 35;

int comandaPWM = 0;

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
  analogWrite(pinMOSFET, 0);

  lcd.clear();
}

void loop() 
{
  long sumaRaw = 0;
  for (int i = 0; i < 5; i++) {
    sumaRaw += analogRead(SenzCald);
    delay(2);
  }
  float valMedie = sumaRaw / 5.0;
  float tempR = (valMedie * (5000.0 / 1024.0)) / 10.0;

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
      analogWrite(pinMOSFET, 0);
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

  float eroare = GettoTemp - tempR;

  switch (stareCurenta) 
  {
    case STARE_BEFORE:
      comandaPWM = 0;
      break;

    case STARE_HEAT:
      if (tempR >= GettoTemp) 
      {
        stareCurenta = STARE_KEEP;
        StarttoKeep = millis();
        comandaPWM = pwmMinimKeep;
      } 
      else if (eroare > bandaProp) 
      {
        comandaPWM = 255;
      } 
      else 
      {
        comandaPWM = map((long)(eroare * 10), 0, (long)(bandaProp * 10), pwmMinimKeep, 255);
        comandaPWM = constrain(comandaPWM, 0, 255);
      }
      break;

    case STARE_KEEP:
      {
        unsigned long secundeScurse = (millis() - StarttoKeep) / 1000;
        if (secundeScurse >= KeepSec) 
        {
          stareCurenta = STARE_COOL;
          comandaPWM = 0;
        } 
        else 
        {
          RemainingtoKeep = KeepSec - secundeScurse;
          
          if (eroare > 0) {
            comandaPWM = map((long)(eroare * 10), 0, (long)(bandaProp * 10), pwmMinimKeep, 220);
          } else {
            comandaPWM = 0;
          }
          comandaPWM = constrain(comandaPWM, 0, 255);
        }
      }
      break;

    case STARE_COOL:
      comandaPWM = 0;
      if (tempR <= 30.0) 
      {
        stareCurenta = STARE_FINI;
      }
      break;

    case STARE_FINI:
      comandaPWM = 0;
      break;
  }

  analogWrite(pinMOSFET, comandaPWM);

  int procentPutere = map(comandaPWM, 0, 255, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(GettoTemp);
  lcd.print((char)223);
  lcd.print("C ");

  lcd.setCursor(8, 0);
  switch (stareCurenta) 
  {
    case STARE_BEFORE: lcd.print("[BE4 ]  "); break;
    case STARE_HEAT: 
      lcd.print("[H ");
      if (procentPutere < 100) lcd.print(" ");
      lcd.print(procentPutere);
      lcd.print("%]");
      break;
    case STARE_KEEP: 
      lcd.print("[K ");
      if (RemainingtoKeep < 10) lcd.print(" ");
      lcd.print(RemainingtoKeep);
      lcd.print("s]");
      break;
    case STARE_COOL: lcd.print("[COOL]  "); break;
    case STARE_FINI: lcd.print("[FINI]  "); break;
  }

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(tempR, 1);
  lcd.print((char)223);
  lcd.print("C ");

  lcd.setCursor(10, 1);
  lcd.print("P:");
  if (procentPutere < 10) lcd.print("  ");
  else if (procentPutere < 100) lcd.print(" ");
  lcd.print(procentPutere);
  lcd.print("%");

  delay(60);
}