#include <LiquidCrystal.h>
#include <TinyGPS++.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
TinyGPSPlus tinyGPS;

const int BUTTON_MODE = 6;
const int BUTTON_OPEN = HIGH;
const int BUTTON_CLOSED = LOW;
const int UPDATE_INTERVAL = 100;

int mode = 1;

byte degsym[8] = {
  B01000,
  B10100,
  B01000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

void setup() {
  pinMode(BUTTON_MODE, INPUT);
  digitalWrite(BUTTON_MODE, HIGH);     // turn on the built in pull-up resistor
  lcd.begin(16, 2);
  lcd.createChar(0, degsym);
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  DisplayTitle();
}

void loop() {
  CheckForSatLock();
  loadData(UPDATE_INTERVAL);

  switch (mode) {
    case 1:
      displayLatLonDecimal();
      break;
    case 2:
      displayLatLonDegrees();
      break;
    case 3:
      displayStats();
      break;
    default:
      break;
  }
  checkButton();
}


void checkButton()
{
  if (digitalRead(BUTTON_MODE) == BUTTON_CLOSED)
  {
    mode++;
    if (mode == 4) {
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
      lcd.setCursor(5,1);
      lcd.print("  ");
      lcd.setCursor(5,1);
      lcd.print(tinyGPS.satellites.value());
      loadData(UPDATE_INTERVAL);
    }
    lcd.clear();
  }
}

void displayLatLonDecimal() {
  lcd.setCursor(0, 0);
  lcd.print("LAT:");
  lcd.setCursor(6, 0);
  lcd.print(tinyGPS.location.lat(), 6);

  lcd.setCursor(0, 1);
  lcd.print("LON:");
  lcd.setCursor(5, 1);
  lcd.print(tinyGPS.location.lng(), 6);
}

void calcDegMinSec(double posDecimal, double* posDeg, double* posMin, double* posSec) {
  double intPart, fractPart;

  fractPart = modf(posDecimal, &intPart);
  *posDeg = intPart;
  fractPart = modf(fractPart * 60.0, &intPart);
  *posMin = intPart;
  fractPart = modf(fractPart * 600.0, &intPart);  // want 3 digits for seconds
  *posSec = intPart;
}

void displayLatLonDegrees() {
  double latDeg, latMin, latSec;
  double lonDeg, lonMin, lonSec;
  char buf[12];

  calcDegMinSec(tinyGPS.location.lat(), &latDeg, &latMin, &latSec);
  calcDegMinSec(tinyGPS.location.lng(), &lonDeg, &lonMin, &lonSec);
  Serial.println(lonDeg);

  dtostrf(latDeg, 2, 0, buf);
  lcd.setCursor(0, 0);
  lcd.print("N");
  lcd.setCursor(3, 0);
  lcd.print(buf);
  lcd.setCursor(5, 0);
  lcd.write(int(0));
  dtostrf(latMin, 2, 0, buf);
  lcd.setCursor(6, 0);
  lcd.print(buf);
  lcd.setCursor(8, 0);
  lcd.print(".");
  dtostrf(latSec, 3, 0, buf);
  lcd.setCursor(9, 0);
  lcd.print(buf);
  lcd.setCursor(12, 0);
  lcd.print("'");

  dtostrf(abs(lonDeg), 2, 0, buf);
  lcd.setCursor(0, 1);
  lcd.print("W -");
  lcd.setCursor(3, 1);
  lcd.print(buf);
  lcd.setCursor(5, 1);
  lcd.write(int(0));
  dtostrf(abs(lonMin), 2, 0, buf);
  lcd.setCursor(6, 1);
  lcd.print(buf);
  lcd.setCursor(8, 1);
  lcd.print(".");
  dtostrf(abs(lonSec), 3, 0, buf);
  lcd.setCursor(9, 1);
  lcd.print(buf);
  lcd.setCursor(12, 1);
  lcd.print("'");
}

void displayStats() {
  unsigned int month; 
  unsigned int day;
  unsigned int hour;
  unsigned int minute; 
  unsigned int second;
  char buf[16];

  month = tinyGPS.date.month();
  day = tinyGPS.date.day();
  hour = tinyGPS.time.hour();
  minute = tinyGPS.time.minute();
  second = tinyGPS.time.second();
  sprintf(buf, "%02d/%02d %02d:%02d:%02d", month, day, hour, minute, second);
  lcd.setCursor(0, 0);
  lcd.print(buf);

  lcd.setCursor(0, 1);
  lcd.print("Sats:");
  lcd.setCursor(6, 1);
  sprintf(buf, "%2d", tinyGPS.satellites.value());
  lcd.print(buf);
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
  lcd.setCursor(0, 0);
  lcd.print("Satellite Search");
  lcd.setCursor(0, 1);
  lcd.print("GPS:");
}

void Blink(int time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(time);
  digitalWrite(LED_BUILTIN, LOW);
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
