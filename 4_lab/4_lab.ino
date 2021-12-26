#define VCONST 41.2
#define ICONST 183.7

void setup()
{
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  pinMode(11, OUTPUT); // управление
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}
void loop()
{
  double I[5][25];
  double U[5][25];
  for(int i = 1; i <= 5; i++)
  {
    analogWrite(11, i * 51);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    delay(2000);
    for(int j = 0; j < 25; j++)
    {
      I[i-1][j] = analogRead(A0)/ICONST;
      U[i-1][j] = analogRead(A1)/VCONST;
      delay(100);
    }
    analogWrite(11, 0);
    delay(1000);
  }
 
  for(int i = 0; i < 5; i++)
  {
    double sI = 0, sU = 0;
    Serial.println("Power: " + String((i+1)*20));
    for(int j = 0; j < 25; j++){sU += U[i][j]; sI += I[i][j];} 
    Serial.println(String(sU/25) + "\t" + String(sI/25)); 
  }
}
