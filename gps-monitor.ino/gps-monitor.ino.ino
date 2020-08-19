#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  lcd.begin(16, 2);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  DisplayTitle();
  DisplayLabels();
}

void loop() {
  int s;
  do
  {
    if (Serial1.available() > 0) {
      s = Serial1.read();
      lcd.setCursor(10,1);
      lcd.print(s);
    }
  } while (true);
}

void DisplayTitle()
{
  lcd.setCursor(6, 0);
  lcd.print("G P S");
  lcd.setCursor(0, 1);
  lcd.print("(c) Erik Orange");
  delay(3000);
  lcd.clear();
}

void DisplayLabels()
{
  lcd.setCursor(0,0);
  lcd.print("LAT:");
  lcd.setCursor(0,1);
  lcd.print("LON:");
}

void ClearLCDLine(int line)
{
  lcd.setCursor(0, line);
  lcd.print("                    ");
  return;
}

void Blink(int time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(time);
  digitalWrite(LED_BUILTIN, LOW);
}

 
