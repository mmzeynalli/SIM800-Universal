#include "src/SIM800.h"

unsigned long bauds = 19200;

SIM800 sim800(Serial, 28, 23);

void setup() {
  
  Serial.begin(9600);  
  while (!Serial) {;}
  delay(100);
  
  SIM.setTimeout(5000);

  sim800.ipBearer(SET, "3,1,\"Contype\",\"GPRS\"");    // Configure profile 1 connection as "GPRS"
  sim800.ipBearer(SET, "3,1,\"APN\",\"internet\"");    // Set profile 1 access point name to "internet"
  sim800.ipBearer(SET, "1,1");                         // Open GPRS connection on profile 1
  sim800.ipBearer(SET, "2,1");                         // Display IP address of profile 1
  sim800.httpInit(EXE);                                // Initialize HTTP functionality
  sim800.httpParams(SET, "\"CID\",1");                 // Choose profile 1 as HTTP channel
  sim800.httpParams(SET, "\"URL\",\"http://test.sumaks.com/proje/3/\"");   // Set URL to www.sim.com
  sim800.httpAction(SET, "1");                         // Get the webpage
  while(!sim800.available()) {;}                       // Wait until the webpage has arrived
  //SIM.httpRead(EXE);                                // Send the received webpage to Arduino
  sim800.httpEnd(EXE);                                 // Terminate HTTP functionality
  sim800.ipBearer(SET, "0,1");                         // Close GPRS connection on profile 1
}

void loop() {}
