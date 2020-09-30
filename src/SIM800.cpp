#include <SIM800.h>

SIM800::SIM800(Stream &serial, uint8_t sim_key_pin, uint8_t sim_rst_pin) 
{
    simCom = &serial;
	key_pin = sim_key_pin;
	rst_pin = sim_rst_pin;

	pinMode(key_pin, OUTPUT);
	pinMode(rst_pin, OUTPUT);
}

void SIM800::init(protocol_t p)
{
	protocol = p;

	setTimeout(10000);
	turn_on();

	test();
	reset_settings();
}

void SIM800::turn_on()
{
	if (is_on())
		return;
		
	digitalWrite(key_pin, HIGH);
	millis_delay(1300);
	digitalWrite(key_pin, LOW);
}

void SIM800::turn_off()
{
	powerOff("1");
}

void SIM800::reset()
{
	if (rst_pin != 255)
	{
		digitalWrite(rst_pin, LOW);
		delay(100);
		digitalWrite(rst_pin, HIGH);
	}
	else
	{
		turn_off();
		turn_on();
	}
	
}

bool SIM800::is_on()
{
	// if (led_pin != 255)
	// 	return digitalRead(led_pin);

	return false;
}

void SIM800::reset_settings()
{
	userReset();
	factoryReset();
}

bool SIM800::detect_sim()
{
	getSelfAddr(EXE);					// try to get address of the number
	return strstr(ioBuffer, "OK");

/* ANOTHER WAY DETECTING SIMCARD (NOT USED FOR NOW) */
#if 0
	detectSimCard(SET, "1");
	simPresent(GET);

	return (NULL == (strstr(ioBuffer, "0,0")));
#endif
}

void SIM800::get_imei(char *imei)
{
	reqIMEI(EXE);
	
	char *p = strtok(ioBuffer, " ");
	p = strtok(NULL, " ");
	
	strncpy(imei, p, 15);
	free(p);
}

void SIM800::millis_delay(uint64_t timeout)
{

	uint64_t prev = millis();

	while ((millis() - prev) < timeout)
	{
#ifdef USING_WDT
		wdt_reset();
#endif
	}
}