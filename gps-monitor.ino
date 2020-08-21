#include <LiquidCrystal.h>
#include <TinyGPS++.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
TinyGPSPlus tinyGPS;

const int BUTTON_MODE = 6;
const int BUTTON_OPEN = HIGH;
const int BUTTON_CLOSED = LOW;
int mode = 1;

void setup() {
  pinMode(BUTTON_MODE, INPUT);
  digitalWrite(BUTTON_MODE, HIGH);     // turn on the built in pull-up resistor
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  DisplayTitle();
}

void loop() {
  CheckForSatLock();
  loadData(1000);
  
  switch (mode) {
  case 1:
    displayLatLon();
    break;
  case 2:
    displayAltitude();
    break;
  default:
    break;
  }
  checkButton();
}

void printvals()
{
  Serial.println("---BEGIN---");
  Serial.println(tinyGPS.location.lat(), 6); // Latitude in degrees (double)
Serial.println(tinyGPS.location.lng(), 6); // Longitude in degrees (double)
Serial.print(tinyGPS.location.rawLat().negative ? "-" : "+");
Serial.println(tinyGPS.location.rawLat().deg); // Raw latitude in whole degrees
Serial.println(tinyGPS.location.rawLat().billionths);// ... and billionths (u16/u32)
Serial.print(tinyGPS.location.rawLng().negative ? "-" : "+");
Serial.println(tinyGPS.location.rawLng().deg); // Raw longitude in whole degrees
Serial.println(tinyGPS.location.rawLng().billionths);// ... and billionths (u16/u32)
Serial.println(tinyGPS.date.value()); // Raw date in DDMMYY format (u32)
Serial.println(tinyGPS.date.year()); // Year (2000+) (u16)
Serial.println(tinyGPS.date.month()); // Month (1-12) (u8)
Serial.println(tinyGPS.date.day()); // Day (1-31) (u8)
Serial.println(tinyGPS.time.value()); // Raw time in HHMMSSCC format (u32)
Serial.println(tinyGPS.time.hour()); // Hour (0-23) (u8)
Serial.println(tinyGPS.time.minute()); // Minute (0-59) (u8)
Serial.println(tinyGPS.time.second()); // Second (0-59) (u8)
Serial.println(tinyGPS.time.centisecond()); // 100ths of a second (0-99) (u8)
Serial.println(tinyGPS.speed.value()); // Raw speed in 100ths of a knot (i32)
Serial.println(tinyGPS.speed.knots()); // Speed in knots (double)
Serial.println(tinyGPS.speed.mph()); // Speed in miles per hour (double)
Serial.println(tinyGPS.speed.mps()); // Speed in meters per second (double)
Serial.println(tinyGPS.speed.kmph()); // Speed in kilometers per hour (double)
Serial.println(tinyGPS.course.value()); // Raw course in 100ths of a degree (i32)
Serial.println(tinyGPS.course.deg()); // Course in degrees (double)
Serial.println(tinyGPS.altitude.value()); // Raw altitude in centimeters (i32)
Serial.println(tinyGPS.altitude.meters()); // Altitude in meters (double)
Serial.println(tinyGPS.altitude.miles()); // Altitude in miles (double)
Serial.println(tinyGPS.altitude.kilometers()); // Altitude in kilometers (double)
Serial.println(tinyGPS.altitude.feet()); // Altitude in feet (double)
Serial.println(tinyGPS.satellites.value()); // Number of satellites in use (u32)
Serial.println(tinyGPS.hdop.value()); // Horizontal Dim. of Precision (100ths-i32)
Serial.println("---END---");
}
void checkButton()
{
  if (digitalRead(BUTTON_MODE) == BUTTON_CLOSED)
  {
    mode++;
    if (mode == 3) {
      mode = 1;
    }
    lcd.clear();
  }
}


void CheckForSatLock()
{
  if (tinyGPS.location.lat() == 0.0)
  {
    DisplaySatLock();
    while (tinyGPS.location.lat() == 0.0) {
      loadData(500);
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

void displayAltitude() {
  lcd.setCursor(0,0);
  lcd.print("Altitude:");
  lcd.setCursor(10,0);
  lcd.print(tinyGPS.altitude.feet());

  lcd.setCursor(0,1);
  lcd.print("Num Sats:");
  lcd.setCursor(10,1);
  lcd.print(tinyGPS.satellites.value());
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

 
