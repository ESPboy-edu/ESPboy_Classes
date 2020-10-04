/*
ESPboyOTA class -- ESPboy App Store client core v0.13
for www.ESPboy.com project
https://hackaday.io/project/164830-espboy-games-iot-stem-for-education-fun
thanks to DmitryL (Plague) for coding help,
Corax, AlRado, Torabora, MLXXXP for tests and advices.


ESPboy OTA keys:
1. Keep pressed A or B during reset to connect ESPboy App Store
  - A for attemp to connect last known WiFi
  - B to scan available WiFi's
2. There are two modes in OTA: keyboard and console
3. To switch modes use RGT+LGT or A+B
4. In keyboard mode
  - LEFT+UP - switch on caps lock
  - RGT or LFT scroll console window up/down
  - UP/DOWN/LEFT/RIGHT to chose char
  - short press A - enter char to typing string
  - long press A - finish entering, finish typing
  - short press B - delete last char in typing string
  - long press B - delete typing string
5. In console mode
  - UP/DOWN, RGT/LFT, LEFT/RIGHT - scroll console window
  - B - switch to keyboard mode

Check "ESPboyGUI.h" for defines:
  - #define U8g2 - if defined then using font 4x6, if commented using font 6x8
  - #define buttonclicks - if defined - button are clicking but it takes more than 1kb RAM, if commented - no clicks and more free RAM

 2Do
 - add Type menu
 - add Genre menu
*/


#include <Adafruit_MCP23017.h>
#include <TFT_eSPI.h>
#include "lib/ESPboyLogo.h"
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include "ESPboyOTA.h"
#include "ESPboyGUI.h"

#define PAD_LEFT        0x01
#define PAD_UP          0x02
#define PAD_DOWN        0x04
#define PAD_RIGHT       0x08
#define PAD_ACT         0x10
#define PAD_ESC         0x20
#define PAD_LFT         0x40
#define PAD_RGT         0x80
#define PAD_ANY         0xff

#define MCP23017address 0  // actually it's 0x20 but in <Adafruit_MCP23017.h> lib there is (x|0x20) :)
#define LEDPIN D4
#define SOUNDPIN D3
#define CSTFTPIN 8  // CS MCP23017 PIN to TFT

Adafruit_MCP23017 mcp;
TFT_eSPI tft;
ESPboyOTA *OTAobj = NULL;
ESPboyGUI *GUIobj = NULL;


uint8_t getKeys() { return (~mcp.readGPIOAB() & 255); }


void setup() {
  Serial.begin(115200);
  delay(100);

  // MCP23017 and buttons init, should preceed the TFT init
  mcp.begin(MCP23017address);
  delay(100);

  for (int i = 0; i < 8; ++i) {
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);
  }

  // Sound init and test
  pinMode(SOUNDPIN, OUTPUT);
  tone(SOUNDPIN, 200, 100);
  delay(100);
  tone(SOUNDPIN, 100, 100);
  delay(100);
  noTone(SOUNDPIN);

  // TFT init
  mcp.pinMode(CSTFTPIN, OUTPUT);
  mcp.digitalWrite(CSTFTPIN, LOW);
  tft.begin();
  delay(100);
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // draw ESPboylogo
  tft.drawXBitmap(30, 20, ESPboyLogo, 68, 64, TFT_YELLOW);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString(F("App Store"), 38, 95);

  delay(1000);

  // init ADC voltage meter
  pinMode(A0, INPUT);

  // clear screen
  tft.fillScreen(TFT_BLACK);

  // check OTA
  if (getKeys()&PAD_ACT || getKeys()&PAD_ESC) { GUIobj=new ESPboyGUI(&tft, &mcp); OTAobj=new ESPboyOTA(GUIobj);}
  
  WiFi.mode(WIFI_OFF);
}

void loop() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    tft.setCursor(0, random(70));
    tft.print("Keep A or B pressed\nduring reset to start\nESPboy App store\n\nA-connect last WiFi\nB-scan networks");
    while (!getKeys()) delay(200);
    tone(SOUNDPIN, 100, 100);
    delay(200);
}
