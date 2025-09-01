
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#define ESPALEXA_A  1SYNC
#include <Espalexa.h>
#include <Adafruit_NeoPixel.h>

#define WSPIN          5 //ESP8266 PIN
#define NUMPIXELS      46 //NUMBER OF LEDS

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_RGB + NEO_KHZ800);

boolean connectWifi();

void colorLightChanged(uint8_t brightness, uint32_t rgb);

// Change this!!
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

boolean wifiConnected = false;

Espalexa espalexa;

void setup()
{
  Serial.begin(115200);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  pixels.begin();
  if(wifiConnected){
    espalexa.addDevice("RGB da Mesa", colorLightChanged);

    espalexa.begin();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}
 
void loop()
{
   espalexa.loop();
   delay(1);
}

//the color device callback function has two parameters
void colorLightChanged(uint8_t brightness, uint32_t rgb) {
  //do what you need to do here, for example control RGB LED strip
  Serial.print("Brightness: ");
  Serial.print(brightness);
  Serial.print(", Red: ");
  Serial.print((rgb >> 16) & 0xFF); //get red component
  Serial.print(", Green: ");
  Serial.print((rgb >>  8) & 0xFF); //get green
  Serial.print(", Blue: ");
  Serial.println(rgb & 0xFF); //get blue
float hell = brightness / 255.0;
int r=((rgb >> 16) & 0xFF)*hell;
int g=((rgb >>  8) & 0xFF)*hell;
int b=(rgb & 0xFF)*hell;

   for(int i=0;i<NUMPIXELS;i++){
  pixels.setPixelColor(i, pixels.Color(g,r,b)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
     }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 40){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
