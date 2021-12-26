#define led_pin 13
bool state = false;

void setup()
{
  pinMode(led_pin, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  if (Serial.available())
  {
    String str = Serial.readString();
    
    if(str == "on")
      state = true;
    else if(str == "off")
      state = false;     
  }
  digitalWrite(led_pin, state);
}
