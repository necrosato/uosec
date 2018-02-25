#include <ESP8266WiFi.h>
//#include "pins_arduino.h" //uncomment when building on mac
//////////////////////
// WiFi Definitions //
//////////////////////
const char *ssid = "uosec";
const char *password = "security";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = D4; // ESP's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the ESP
const int DIGITAL_PIN = D3; // Digital pin to be read

IPAddress server(6,6,6,1);
WiFiClient client;
char buf[1024];
String req = "GET / HTTP/1.1";

void setup() 
{
  initHardware();
  setupWiFi();
}

void loop() 
{
    // if we do not have a current connection with the server
    if (!client.connected()) {
        Serial.println("Attempting to connect to server");
        // connect to server
        if (client.connect(server, 80)) {
            Serial.println("Established connection to server\n");
        }
    }
    // else client is already connected
    else {
        // Make HTTP request
        Serial.println("Sending request to server");
        client.println(req);
        client.println();
    }
    
    // if client is still connected OR we have a response buffered
    if (client.connected()) {
        Serial.println("Awaiting response from server");
        // if response recieved, print it
        int i = 0;
        while(client.available() > 0) {
            char r = (char) client.read();
            buf[i] = r;
            i++;
        }
        Serial.println("Response recieved from server:");
        client.flush();
        Serial.println(buf);
        strcpy(buf, "");
    }
    else {
        Serial.println("Terminating connection to server\n");
        client.stop();
    }
    
    delay(100);
}

void setupWiFi()
{
  int wifiStatus;
  Serial.println("\n");
  Serial.print("This device's MAC address is: ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_AP);
  IPAddress ip(6,6,6,2);
  IPAddress gateway(6,6,6,1); 
  IPAddress subnet(255,255,255,0); 
  //WiFi.mode(WIFI_STA);
  //WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  wifiStatus = WiFi.status();
  if(wifiStatus == WL_CONNECTED){
      Serial.println("");
      Serial.print("Connected - Your IP address is: ");
      Serial.println(WiFi.localIP());  
  }
 
}

void initHardware()
{
  Serial.begin(115200);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, HIGH);//on Lolin ESP8266 v3 dev boards, the led is active low
  //delay(1000);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.
}
