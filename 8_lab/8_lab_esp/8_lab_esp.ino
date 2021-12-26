#include <ESP8266WiFi.h>
WiFiServer server(80);

const char* ssid     = "iPhone12";
const char* password = "12345678";
String outputState = "off";

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

void loop() {
  WiFiClient client = server.available();   
  if (client)
  {                                    
    String request = client.readStringUntil('\r');
    client.flush();
          
    if (request.indexOf("GET /on") >= 0)
    {
      outputState = "on";
      Serial.print(outputState);
    }
    else if(request.indexOf("GET /off") >= 0)
    {
      outputState = "off";
      Serial.print(outputState);
    }

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
    client.println("<body><h1>ESP8266 Web Server</h1>"); 
    client.println("<p>Led State: " + outputState + "</p>");
      
    if (outputState=="off")
      client.println("<p><a href=\"/on\"><button class=\"button\">ON</button></a></p>");   
    else
      client.println("<p><a href=\"/off\"><button class=\"button button2\">OFF</button></a></p>");

    client.println("</body></html>");
    client.println();
  }
}
