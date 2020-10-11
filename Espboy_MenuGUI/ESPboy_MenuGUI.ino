#include <Adafruit_MCP23017.h> //to control buttons
#include <TFT_eSPI.h>          //to draw at LCD TFT
#include <ESP8266WiFi.h>       //to control WiFi
#include "lib/ESPboyLogo.h"
#include "ESPboyMenuGUI.h"

#define MCP23017address 0 // actually it's 0x20 but in <Adafruit_MCP23017.h> lib there is (x|0x20) :)
#define MCP4725address  0x60

//PINS
#define LEDPIN         D4
#define SOUNDPIN       D3
#define LEDLOCK        9
#define CSTFTPIN       8 //Chip Select pin for LCD (it's on the MCP23017 GPIO expander GPIO8)

Adafruit_MCP23017 mcp;
TFT_eSPI tft;
ESPboyMenuGUI menuGUI(&tft, &mcp);

const char *menulist1[] PROGMEM = {
  "Item one",
  "-Item two",  //if first symbol "-" then this line can't be selected in menu
  "Item three",
  "Item four",
  "Item five",
  "Item six",
  "Item seven",
  "Item eight",
  "Item nine",
  "Item ten",
  "-Item eleven",
  "Item 12",
  "Item 13",
  "Item 14",
  "Item 15",
  "-Item 16",
  "Item 17",
  "Item 18",
  "Item 19",
  "Item 20",
  "Item 21",
  NULL, // the last element of every menu list should be NULL
};


void setup(){
  Serial.begin(115200); //serial init
  WiFi.mode(WIFI_OFF); // to safe some battery power

//mcp23017 init for buttons, LED LOCK and TFT Chip Select pins
  mcp.begin(MCP23017address);
  delay(100);
  
  for (int i=0;i<8;i++){  
     mcp.pinMode(i, INPUT);
     mcp.pullUp(i, HIGH);}

//sound init and test
  pinMode(SOUNDPIN, OUTPUT);
  tone(SOUNDPIN, 200, 100); 
  delay(100);
  tone(SOUNDPIN, 100, 100);
  delay(100);
  noTone(SOUNDPIN);
  
//LCD TFT init
  mcp.pinMode(CSTFTPIN, OUTPUT);
  mcp.digitalWrite(CSTFTPIN, LOW);
  tft.begin();
  delay(100);
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

//draw ESPboylogo  
  tft.drawXBitmap(30, 24, ESPboyLogo, 68, 64, TFT_YELLOW);
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(37,102);
  tft.print ("Menu test");

  delay(2000);
  tft.fillScreen(TFT_BLACK);
}


uint8_t getKeys() { return (~mcp.readGPIOAB() & 255); }


void loop(){
 uint16_t menuItem;
   menuItem = menuGUI.menuInit(menulist1, TFT_YELLOW, TFT_BLUE, TFT_BLUE);  
   tft.setTextColor(TFT_GREEN);
   tft.drawString("Menu No:", 42, 55);
   tft.drawString((String)(menuItem), 62, 68);
   delay(1000);
}
