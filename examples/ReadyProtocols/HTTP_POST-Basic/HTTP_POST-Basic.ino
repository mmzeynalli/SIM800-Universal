#include <SIM800.h>
#include <SoftwareSerial.h>
#include <stdlib.h>

/* VARIABLES */
static const uint8_t  sim_power_pin = 8;
static const char     URL[]         = "example.com";

char imei[16];
static char recv[100];
static const char send[] = "Hello from Arduino";

SoftwareSerial ss(2, 3);
SIM800 sim800(ss, sim_power_pin); // for hardware serial, use Serialx instead of ss (for ex. Serial1)

void setup()
{
  pinMode(sim_power_pin, OUTPUT);

  Serial.begin(9600);
  ss.begin(9600); 
  
  sim800.init(HTTP);  // change it to `HTTPS` for secure communication
  sim800.http_init();
  
  /* Read and print imei */
  sim800.get_imei(imei);
  Serial.print("imei = ");
  Serial.println(imei);
}

void loop()
{
  sim800.http_post(URL, send);
  sim800.http_read(recv);

  Serial.print("Data from server: ");
  Serial.println(recv);
  
  sim800.http_end();
}