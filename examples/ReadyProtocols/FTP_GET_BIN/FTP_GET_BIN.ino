#include <SIM800.h>
#include <SoftwareSerial.h>

#define SERV "\"example.com\""  // server url or ip
#define PATH "\"/\""            // path of file
#define USER "\"username\""     // username
#define PASS "\"password\""     // password
#define FILE "\"file.txt\""     // file name

static const uint8_t sim_power_pin  = 8;

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

  /* For example we want to download 1Kb of binary file in 8 stacks of 128 bytes */
  /* NOTE!!! SIM800 cannot read more than 120 (approximately) characters per one FTP read */ 
  uint8_t arr[8][128];
  for (int i = 0; i < 8; i++)
  {
    sim800.ftp_read(arr[i], 64);
    sim800.ftp_read(arr[i] + 64, 64);


    for (int j = 0; j < 128; j++)
    {
      if (arr[i][j] < 16)
        Serial.print('0');
      
      Serial.print((uint8_t) arr[i][j], HEX);
    }

    Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}