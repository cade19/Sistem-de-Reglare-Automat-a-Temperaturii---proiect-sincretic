/* PAS 3:
    Adaugarea celor 4 butoane, a posibilitatii de a 
    modifica temperatura de atins si a functionalitatii
    butonului de start - cat il tin apasat merge,
    altfel se opreste.
*/

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int SenzCald = A0;

const int btnUp     = 6;
const int btnDown   = 7;
const int btnSelect = 8;
const int btnStart  = 9;

int GettoTemp = 40; 

void setup() 
{
  lcd.begin(16, 2);
  
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);
  pinMode(btnStart, INPUT_PULLUP);

  lcd.clear();
}

void loop() 
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

  
  int val = analogRead(SenzCald);
  float tempR = (val* (5000.0 / 1024.0)) / 10.0;

  lcd.setCursor(0, 0);
  lcd.print("Set: ");
  lcd.print(GettoTemp);
  lcd.print((char)223);
  lcd.print("C  ");

  if (digitalRead(btnStart) == LOW) 
  {
    lcd.print("GO  ");
  } else 
  {
    lcd.print("STOP");
  }

  lcd.setCursor(0, 1);
  lcd.print("Real: ");
  lcd.print(tempR, 1);
  lcd.print((char)223);
  lcd.print("C   ");

  delay(50);
}