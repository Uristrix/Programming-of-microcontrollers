#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <OneWire.h>
OneWire ds(D2);
WiFiServer server(80);

const char* ssid     = "iPhone12";
const char* password = "12345678";
double DStemp = 0.0;
double NTCtemp = 0.0;
    
double getDS18B20()
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

double getNTCtemp()
{
 double Vout, Rt;
 double T, Tc = 0;

 Vout = analogRead(A0) * 5.0/4095.0;
 Rt = 100000 * Vout / (5.0 - Vout);
 T = 1/(1/298.15 + log(Rt/10000)/3950); // Temperature in Kelvin
 Tc = T - 273.15; // Celsius
 return Tc;
}
void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  server.begin();
  
  Serial.print("\nUse this URL : http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop(){
  WiFiClient client = server.available();   

  if (client)
  {                            
    Serial.println("New Client.");            
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
          
    if (request.indexOf("GET /go") >= 0)
    {
      NTCtemp = getNTCtemp();
      DStemp = getDS18B20();
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");
            
    // CSS
    client.println("<style>html {color: white; font-family: Helvetica; display: inline-block; margin: 0px auto;"); 
    client.println("text-align: center; background: linear-gradient(90deg, rgba(156,58,180,1) 0%, rgba(69,177,252,1) 100%);}");
    client.println(".button { display: inline-flex; align-items: center; justify-content: center; padding: 0.7rem 2rem;");
    client.println(" font-weight: 700; font-size: 18px; text-align: center; text-decoration: none; color: #fff;");
    client.println("backface-visibility: hidden; border: 0.3rem solid transparent; border-radius: 3rem;");
    client.println("border-color: #fff; transition: transform 0.2s cubic-bezier(0.235, 0, 0.05, 0.95);}</style></head>" );   
       
    // Web Page Heading
    client.println("<body><h1>ESP8266 Web Server</h1>"); 
    client.println("<h2>DS18B20 temp: "+String(DStemp) + "</h2>"); 
    client.println("<h2>NTC temp:     "+String(NTCtemp) + "</h2>");       
    client.println("<a href=\"/go\" class=\"button\">get temp</a>");
    client.println("</body></html>");
    client.println();         
  }
}
