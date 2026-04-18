#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT11 setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Bluetooth setup (RX, TX)
SoftwareSerial BT(3, 4);

// Pins
int buzzer = 7;
int led = 6;

float temp;

void setup() {
  lcd.init();
  lcd.backlight();

  dht.begin();
  BT.begin(9600);
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);

  // Startup message
  lcd.setCursor(0, 0);
  lcd.print("MediTrack");
  lcd.setCursor(0, 1);
  lcd.print("Medicine Box");
  delay(2000);
  lcd.clear();
}

void loop() {

  // Read temperature
  temp = dht.readTemperature();

  // Check sensor error
  if (isnan(temp)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error   ");
    BT.println("Sensor Error");
    delay(2000);
    return;
  }

  // Display temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C   ");

  // Condition check (2°C to 8°C safe)
  if (temp > 8 || temp < 2) {
    // ALERT
    digitalWrite(buzzer, HIGH);
    digitalWrite(led, LOW);

    lcd.setCursor(0, 1);
    lcd.print("ALERT!        ");

    BT.println("MediTrack ALERT!");
    BT.println("Temp Unsafe!");
  } 
  else {
    // NORMAL
    digitalWrite(buzzer, LOW);
    digitalWrite(led, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("NORMAL        ");

    BT.println("MediTrack: Normal");
  }

  // Send temperature
  BT.print("Temp: ");
  BT.print(temp);
  BT.println(" C");

  delay(1000);
}
