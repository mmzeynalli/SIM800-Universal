#include <SIM800.h>
#include <SoftwareSerial.h>

#define SERV "\"example.com\""  // server url or ip
#define PORT "4040"             // TCP port

static const uint8_t sim_power_pin  = 8;
static const char send[]            = "Hello from Arduino!\r\n";
static char recv[100];

SoftwareSerial ss(2, 3);
SIM800 sim800(ss, sim_power_pin); // for hardware serial, use Serialx instead of ss (for ex. Serial1)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ss.begin(9600);

  sim800.init(TCP);

  sim800.tcp_init();
  sim800.tcp_open(SERV, PORT);

}

void loop() {
  // put your main code here, to run repeatedly:

  sim800.tcp_send(send);
  delay(1000);
  sim800.tcp_read(recv);

  Serial.print("Received data = ");
  Serial.println(recv);

  sim800.tcp_end();

}