#include <ESP8266WiFi.h>
#define output5 D5
#define output4 D4

const char* ssid     = "iPhone12";
const char* password = "12345678";

String output5State = "off";
String output4State = "off";
    
WiFiServer server(80);

void setup() {
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  
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

    if (request.indexOf("GET /5/on") >= 0)
    {
      Serial.println("GPIO 5 on");
      output5State = "on";
      digitalWrite(output5, HIGH);
    } 
    else if (request.indexOf("GET /5/off") >= 0)
    {
      Serial.println("GPIO 5 off");
      output5State = "off";
      digitalWrite(output5, LOW);
    } 
    
    else if (request.indexOf("GET /4/on") >= 0)
    {
      Serial.println("GPIO 4 on");
      output4State = "on";
      digitalWrite(output4, HIGH);
    } 
    else if (request.indexOf("GET /4/off") >= 0)
    {
      Serial.println("GPIO 4 off");
      output4State = "off";
      digitalWrite(output4, LOW);
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
    client.println("<p>GPIO 5 - State " + output5State + "</p>");
      
    if (output5State=="off")
      client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");   
    else
      client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
          
               
    // Display current state, and ON/OFF buttons for GPIO 4  
    client.println("<p>GPIO 4 - State " + output4State + "</p>");
     
    if (output4State=="off")
       client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
    else
       client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
              
    client.println("</body></html>");
    client.println();         
  }
}
