#include <SIM800.h>
#include <SoftwareSerial.h>

#define SERV "\"example.com\""  // server url or ip
#define PATH "\"/\""            // path of file
#define USER "\"username\""     // username
#define PASS "\"password\""     // password
#define FILE "\"file.txt\""     // file name

static const uint8_t sim_power_pin  = 8;
static char recv[100];

SoftwareSerial ss(2, 3);
SIM800 sim800(ss, sim_power_pin); // for hardware serial, use Serialx instead of ss (for ex. Serial1)

void setup() {
  // put your setup code here, to run once:
  ss.begin(9600);
  Serial.begin(9600);
  
  sim800.init(FTP);
  sim800.ftp_init();
  sim800.ftp_connect(SERV, USER, PASS);
  sim800.ftp_get(FILE, PATH);

  int len = sim800.ftp_read(recv, 50);
  recv[len] = 0; // terminating character. Not done automatically, as one would want to concatenate other string

  Serial.print("Available ");
  Serial.print(len);
  Serial.println(" bytes:");
  Serial.println(recv);
}

void loop() {
  // put your main code here, to run repeatedly:

}