#include <LiquidCrystal_I2C.h>
#include <Wire.h>
# define song 1
# define soundPin 3
# define BPM 120

LiquidCrystal_I2C lcd(0x27, 16, 2);

double duration(double dur)
{
  return 4 / dur * 60 / BPM * 1000;
}


void note(double hz, double dur) {
  tone(soundPin, hz);
  delay(duration(dur));
  noTone(soundPin);
  delay(50);
}

void pause(double dur) {
  noTone(soundPin);
  delay(duration(dur));
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PIONEER - 1");
  lcd.setCursor(0, 1);
  lcd.print("TK OP1 - 2");

  pinMode(soundPin, OUTPUT);
}

void loop() {
  
if(song == 1)
{
  double dur[] = {
  4,6,16,4,4,4,6,16,4,4,
  4,6,16,3,8,4,6,16,4,4,
  4,6,16,4,4,4,6,16,4,4,
  4,6,16,4,6,16,4,6,16,2};
  double nt[] = {
    349,440,261,349,261,349,440,349,392,523,
    392,440,466,523,466,440,392,349,392,261,
    349,440,261,349,261,349,440,349,392,523,
    587,466,392,440,523,523,392,440,440,349
  };
  for (int i = 0; i < 40; i++)
    note(nt[i], dur[i]);
}
else
{
  double dur[] = {8,4,4,8,4,4,4,4,4,8,3,8,6,6,1.25};
  double nt[] = {784,880,784,740,659,880,784,740,659,659,583,523,523,587,494};
  for (int i = 0; i < 15; i++)
    note(nt[i], dur[i]);
}
    
  pause(1);
}
