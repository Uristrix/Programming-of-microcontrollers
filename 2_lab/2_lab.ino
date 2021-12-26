#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
LiquidCrystal_I2C lcd(0x27,16,2); // Указываем I2C адрес (наиболее распространенное значение), 
//а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой строке

void setup(){
  pinMode(13, OUTPUT);
  lcd.init();                     
  lcd.backlight();   

  lcd.setCursor(6,0); 
  lcd.print("SOS!");
  lcd.setCursor(1,1); 
  lcd.print("Accept the lab");
}
void loop(){
  for(int i = 0; i < 2; i++){
    lcd.display();
    digitalWrite(13, HIGH);
    delay(150);
    lcd.noDisplay();
    digitalWrite(13, LOW);
    delay(150);
  }
  for(int i = 0; i < 4; i++){
    lcd.display();
    digitalWrite(13, HIGH);
    delay(400);
    lcd.noDisplay();
    digitalWrite(13, LOW);
    delay(400);
  }
  for(int i = 0; i < 3; i++){
    lcd.display();
    digitalWrite(13, HIGH);
    delay(150);
    lcd.noDisplay();
    digitalWrite(13, LOW);
    delay(150);
  }
  delay(1500);
}
