#include "ESPboyInit.h"
#include "ESPboyTerminalGUI.h"

ESPboyInit myESPboy;
ESPboyTerminalGUI* terminalGUIobj = NULL;

void setup(){
  myESPboy.begin("ESPboy Terminal GUI");
  terminalGUIobj = new ESPboyTerminalGUI(&myESPboy.tft, &myESPboy.mcp);
  terminalGUIobj->printConsole("Testing terminal GUI", TFT_MAGENTA, 1, 0);
}


void loop(){
 
  terminalGUIobj->printConsole("Testing scroll", TFT_YELLOW, 1, 0);
  delay(2000);
  for (uint8_t i=50; i>0; i--){
    String toPrint = (String) i + " ";
    toPrint += "to start";
    if (i>10) terminalGUIobj->printConsole(toPrint, TFT_WHITE, 1, 0); 
    else terminalGUIobj->printConsole(toPrint, TFT_RED, 1, 0);
    delay(10);  
  }
  
  terminalGUIobj->getUserInput();

  terminalGUIobj->printConsole("Testing one line", TFT_YELLOW, 1, 0);
  delay(2000);
  for (uint8_t i=100; i>0; i--){
    String toPrint = (String) i + "%";
    if (i>10) terminalGUIobj->printConsole(toPrint, TFT_WHITE, 1, 1); 
    else terminalGUIobj->printConsole(toPrint, TFT_RED, 1, 1);
    delay(10);  
  }
  
  terminalGUIobj->getUserInput();

  terminalGUIobj->printConsole("Testing info", TFT_YELLOW, 1, 0);
  delay(2000);
  terminalGUIobj->drawOwnTypingLine(F("Testing print..."), TFT_GREEN);
  delay(2000);
  terminalGUIobj->drawOwnTypingLine("", TFT_BLACK);

  terminalGUIobj->getUserInput();
}
