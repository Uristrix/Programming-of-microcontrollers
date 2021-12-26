#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <OneWire.h>
LiquidCrystal_I2C lcd(0x27,16,2);
OneWire ds(2);

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.display();
}

void loop()
{
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);

  delay(1000);
  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);

  byte data[2] = {ds.read(), ds.read()};

  lcd.setCursor(0,0);
  lcd.print("DS18B20:   " +String(((data[1] << 8) | data[0]) * 0.0625));

  float reading = analogRead(A0);
  reading = (100000 / (1023 / reading - 1));
  float temp = reading / 100000;
  temp = log(temp);
  temp /= 3950;
  temp += 1.0 / (25 + 273.15);
  temp = 1.0 / temp;
  temp -= 273.15;
  lcd.setCursor(0,1);
  Serial.println(temp);
  lcd.print("TERMISTOR: " + String(temp));
}
