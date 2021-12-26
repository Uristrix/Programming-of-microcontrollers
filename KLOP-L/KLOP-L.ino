#include <Adafruit_TCS34725.h>
#include <GyverOLED.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#define SDA1 4
#define SCL1 5
#define SDA2 0
#define SCL2 2
#define buttonPin 14

const char* ssid     = "iPhone12";                                        //логин WIFI
const char* password = "12345678";                                        //Пароль WIFI

bool flag = false;                                                        //Флаг отвечающий за кнопку, нажата она или нет
uint32_t timer = 0;                                                       // для избежания дребезга кнопки

int colorTemp1 = 255;                                                       //Цвектовая температура 1 датчика
int lux1 = 256;                                                             //Освещённость 1 датчика
int colorTemp2 = 257;                                                       //Цвектовая температура 2 датчика
int lux2 = 258;                                                             //Освещённость 2 датчика

Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_300MS, TCS34725_GAIN_1X); //объект TCS для работы с датчиками
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;                          //объект oled для работы с экраном

IPAddress ip(172, 20, 10, 11);                                           //IP
IPAddress gateway(255, 20, 1, 17);                                       //шлюз
IPAddress subnet(255, 255, 255, 0);                                      //маска
WiFiServer server(80);                                                   //Создание объекта сервера

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void draw()
{
  oled.setScale(2);
  oled.setCursor(5, 0);
  oled.print("Клоп-L(01)");
  oled.line(0, 19, 128, 19);
  oled.line(0, 35, 128, 35);
  oled.line(64, 19, 64, 64);
  
  oled.setScale(1);
  
  oled.setCursor(18, 3);
  oled.print("TCS1");
  oled.setCursor(84, 3);
  oled.print("TCS2");
}

void clearNum()
{
  oled.clear(0, 40, 60, 60);
  oled.clear(68, 40, 128, 60);
}

void drawNum()
{
  oled.setCursor(1,5);

  oled.setCursor(5, 5);
  oled.print(String(colorTemp1) + " K");
  oled.setCursor(5, 6);
  oled.print(String(lux1) + " lux");
  
  oled.setCursor(69, 5);
  oled.print(String(colorTemp2) + " K");
  oled.setCursor(69, 6);
  oled.print(String(lux2) + " lux");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void test_TCS()
{
  Wire.begin(SDA1,SCL1);
  if (tcs.begin())  
    Serial.println("Found sensor 1");              
  else
  {
    Serial.println("No TCS34725_1 found");
    while (1);
  }
  Wire.begin(SDA2, SCL2);
  if (tcs.begin())                                 
     Serial.println("Found sensor 2");  
  else
  {                                             
    Serial.println("No TCS34725_2 found");
    while (1);
  }
}

void updateTCS(int SDA, int SCL, int &temp, int &lux)
{
  uint16_t r, g, b, c;
  Wire.begin(SDA,SCL);                                    // меняем пины шины I2C
  tcs.begin();                                            // инициализируем TCS34725
  tcs.getRawData(&r, &g, &b, &c);                         // снимаем показания с TCS34725
  temp = tcs.calculateColorTemperature_dn40(r, g, b, c);  // высчитываем показания цветовой темпеатуры
  lux = tcs.calculateLux(r, g, b);                        // высчитываем показания освещённости
  Serial.println(lux);
  Serial.println(temp);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool button(bool &flag, uint32_t &timer)
{
  bool btState = !digitalRead(buttonPin);
  
  if(btState && !flag && millis() - timer > 100)
  {
    flag = true; 
    timer = millis();
  }
                
  if(!btState && flag && millis() - timer > 100)
  {
    flag = false;    
    timer = millis();  
    return true;  
  }
  return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void getData()
{
  updateTCS(SDA2, SCL2, colorTemp2, lux2);  // получить и обновить данные со 2 датчика
  updateTCS(SDA1, SCL1, colorTemp1, lux1);  // получить и обновить данные с  1 датчика
}

void updateOled()
{
    clearNum();                            // очистить область со значениями на экране
    drawNum();                             // отобразить обновлённые данные
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);             //объяаление серии порта для отладки
  pinMode(buttonPin, INPUT_PULLUP); //инициализируем пин кнопки для получения информации о его нажатии
  oled.init();                      //инициализация экрана
  oled.clear();                     //очистка экрана
  draw();                           //рисуем табличку для показаний на экране
  delay(1000);                      //небольшая задержка, чтобы всё успело отрисоваться
  //test_TCS();                       //проверка подключены ли все датчики

//WIFI CONNECT
  WiFi.begin(ssid, password);       //вводим пароль, логин
  WiFi.config(ip, gateway, subnet); //устаналиваем статический ip
  server.begin();                   //запускааем сервер
  
  Serial.print("\nUse this URL : http://"); //вывод адреса в монитор порта
  Serial.print(ip);
  Serial.println("/");
}

void loop()
{ 
  if(button(flag, timer))                           // если кнопка нажата
  {
    getData();                                      // получить данные с датчиков
    updateOled();                                   // обновить показания на экане
  }   

  
  WiFiClient client = server.available();           // создание клиента, который соединяется с указанным IP
  if (client)                                       // если клиент работает
  {                                    
    String request = client.readStringUntil('\r');  // считывает символы из буффера в строку
    client.flush();                                 // отбрасывает непрочитанные непрочитанные байты
                                                    // полученные клинтом от сервера
    requests(request, client);                      // обработка запросов с клиента
    createHTML(client);                             // генерирует html код
  }  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void requests(String request, WiFiClient client)
{
    if (request.indexOf("GET /get") >= 0)
    {
      getData();
    }
    else if(request.indexOf("GET /update") >= 0)
    {
      getData();
      updateOled();
    }
}
void createHTML(WiFiClient client)
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");

    // CSS
    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println(".button { background-colsor: #195B6A; border: none; color: white; padding: 16px 40px;");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(".button2 {background-color: #77878A;}</style></head>");

    // Web Page Heading
    client.println("<body><h1>KLOP-L(01)</h1>"); 
    
    client.println("<h2>TCS1</h2>");
    client.println("<h4>Color temp: "+ String(colorTemp1) + " K</h4>");
    client.println("<h4>Lux: "+ String(lux1) + "</h4>");

    client.println("<h2>TCS2</h2>");
    client.println("<h4>Color temp: "+ String(colorTemp2) + " K</h4>");
    client.println("<h4>Lux: "+ String(lux2) + "</h4>");
    
    client.println("<p><a href=\"/get\"><button class=\"button\">get data</button></a></p>");   
    client.println("<p><a href=\"/update\"><button class=\"button\">get data and update</button></a></p>");
    
    client.println("</body></html>");
    client.println();
}
