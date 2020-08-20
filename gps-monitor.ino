#include <LiquidCrystal.h>
#include <TinyGPS++.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
TinyGPSPlus tinyGPS;

void setup() {
  lcd.begin(16, 2);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  DisplayTitle();
}

void loop() {
  CheckForSatLock();
  loadData(1000);
  displayLatLon();
  
}

void CheckForSatLock()
{
  if (tinyGPS.location.lat() == 0.0)
  {
    DisplaySatLock();
    while (tinyGPS.location.lat() == 0.0) {
      loadData(1000);
    }
    lcd.clear();
  }
}

void displayLatLon() {
  lcd.setCursor(0,0);
  lcd.print("LAT:");
  lcd.setCursor(6,0);
  lcd.print(tinyGPS.location.lat(), 6);
  
  lcd.setCursor(0,1);
  lcd.print("LON:");
  lcd.setCursor(5,1);
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
  lcd.setCursor(2, 0);
  lcd.print("GPS Monitor");
  lcd.setCursor(0, 1);
  lcd.print("(c) Erik Orange");
  delay(1000);
  lcd.clear();
}

void DisplaySatLock()
{
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Acquiring");
  lcd.setCursor(1,1);
  lcd.print("Satellite Lock");
}

void Blink(int time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(time);
  digitalWrite(LED_BUILTIN, LOW);
}

 
