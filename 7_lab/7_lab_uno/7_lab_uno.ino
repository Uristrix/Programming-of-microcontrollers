#include <OneWire.h>
OneWire ds(2);
SoftwareSerial espSerial(5, 6);
void setup()
{
  Serial.begin(115200);
}
float getDS18B20()
{
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);

  delay(1000);
  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);

  byte data[2] = {ds.read(), ds.read()};
  return ((data[1] << 8) | data[0]) * 0.0625;
}

void loop()
{
  float temp = getDS18B20();
  Serial.println("UNO: " + String(temp));
  espSerial.println(temp);
}
