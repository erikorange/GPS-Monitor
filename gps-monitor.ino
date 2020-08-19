#include <LiquidCrystal.h>
#include <TinyGPS++.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
TinyGPSPlus tinyGPS;

void setup() {
  lcd.begin(16, 2);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  DisplayTitle();
  DisplayLabels();
}

void loop() {
  displayLatLon();
  loadData(1000);
}

void displayLatLon() {
  lcd.setCursor(5,0);
  lcd.print(tinyGPS.location.lat(), 6);

  lcd.setCursor(4,1);
  lcd.print(tinyGPS.location.lng(), 6);
}


void loadData(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      tinyGPS.encode(Serial1.read());
  } while (millis() - start < ms);
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

void ClearLine(int line)
{
  lcd.setCursor(4, line);
  lcd.print("            ");
  return;
}

void Blink(int time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(time);
  digitalWrite(LED_BUILTIN, LOW);
}

 
