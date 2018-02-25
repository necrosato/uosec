#include <ESP8266WiFi.h>
//#include "pins_arduino.h" //uncomment when building on mac
//////////////////////
// WiFi Definitions //
//////////////////////
// other esp ep credentials
const char *ssid = "uosec";
const char *password = "security";

// this esp's ap credentials
const char AP_NAME[] = "uosec2";
const char WiFiAPPSK[] = "security";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = D4; // ESP's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the ESP
const int DIGITAL_PIN = D3; // Digital pin to be read

int wifiStatus;
IPAddress server(6,6,6,1);
WiFiClient client;
char buf[1024];
String req_on = "GET /led/1 HTTP/1.1";
String req_off = "GET /led/0 HTTP/1.1";

void setup() 
{
  initHardware();
  setupWiFi();
}

int loop_count = 0;
void loop() 
{
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("lost connection to other esp, waiting for reconnection...");
        delay(3000);
        return;
    }
    /*
    if (loop_count % 10 == 0) {
        setupWiFi();
        loop_count = 0;
    }
    */
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
        Serial.print("Sending request to server: ");
        if (loop_count % 2 == 1) {
            Serial.println(req_on);
            client.println(req_on);
            client.println();
        }
        else {
            Serial.println(req_off);
            client.println(req_off);
            client.println();
        }
    }
    
    // if client is still connected OR we have a response buffered
    if (client.connected()) {
        // if response recieved, print it
        int i = 0;
        while(client.available() > 0) {
            char r = (char) client.read();
            buf[i] = r;
            i++;
        }
        client.flush();
        Serial.println(buf);
        strcpy(buf, "");
    }
    else {
        Serial.println("Terminating connection to server\n");
        client.stop();
    }
    
    loop_count++;
    delay(2500);
}

void setupWiFi()
{
  Serial.println("\n");
  Serial.print("This device's MAC address is: ");
  Serial.println(WiFi.macAddress());
  IPAddress ip(6,6,6,2);
  IPAddress ip2(6,6,6,3);
  IPAddress gateway(6,6,6,1); 
  IPAddress subnet(255,255,255,0); 
  //WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(AP_NAME, WiFiAPPSK, 6, 0);
  Serial.println("");
  Serial.print("This AP's IP address is: ");
  Serial.println(WiFi.softAPIP());  
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

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
