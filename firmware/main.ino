/* PAS 4:
      -Implementarea MOSFET IRF520
      -Adaugarea becului care se incalzeste si lumineaza
      cat timp e sub temperatura ceruta si apoi se stinge.
      -Fixarea la butoane: sa nu trebuiasca sa ti apasat
      pentru functionare, ci apesi butonul si se trece
      pe on, apesi din nou si se opreste.
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

bool sistemPornit = false;
bool cicluFinalizat = false;
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
  
  if (digitalRead(btnUp) == LOW) 
  {
    GettoTemp++;
    cicluFinalizat = false;
    delay(150); 
  }
  if (digitalRead(btnDown) == LOW) 
  {
    GettoTemp--;
    cicluFinalizat = false;
    delay(150);
  }

  int citireStart = digitalRead(btnStart);
  if (citireStart == LOW && ultimulStatusStart == HIGH) 
  {
    sistemPornit = !sistemPornit;
    cicluFinalizat = false; 
    delay(200);
  }
  ultimulStatusStart = citireStart;

  int val = analogRead(SenzCald);
  float tempR = (val * (5000.0 / 1024.0)) / 10.0;

  if (sistemPornit) 
  {
    if (tempR >= GettoTemp) 
    {
      digitalWrite(pinMOSFET, LOW);
      sistemPornit = false;
      cicluFinalizat = true;
    } 
    else 
    {
      digitalWrite(pinMOSFET, HIGH);
    }
  } 
  else 
  {
    digitalWrite(pinMOSFET, LOW);
  }

  lcd.setCursor(0, 0);
  lcd.print("Set: ");
  lcd.print(GettoTemp);
  lcd.print((char)223);
  lcd.print("C  ");

  lcd.setCursor(11, 0);
  if (sistemPornit) 
  {
    lcd.print("ON  ");
  } 
  else if (cicluFinalizat) 
  {
    lcd.print("DONE");
  } 
  else 
  {
    lcd.print("STOP ");
  }

  lcd.setCursor(0, 1);
  lcd.print("Real: ");
  lcd.print(tempR, 1);
  lcd.print((char)223);
  lcd.print("C    ");

  delay(60);
}