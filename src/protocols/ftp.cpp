#include "SIM800.h"

void SIM800::ftp_init()
{

    setTimeout(3000);
    mode_t st = http_get_status();

    if (st == OFF)
    {
        turn_on();
        st = IDLE;
    }
        
    if ((st == DATA_MODE) || (st == IDLE))    // no need/use for configuration
        return;

    ipBearer(SET, "3,1,\"Contype\",\"GPRS\"");    // Configure profile 1 connection as "GPRS"
    ipBearer(SET, "3,1,\"APN\",\"internet\"");    // Set profile 1 access point name to "internet"
    
    setTimeout(10000);
    ipBearer(SET, "1,1");
    send_failed |= (NULL == strstr(ioBuffer, "OK"));

    http_get_status();

}

/* Set parameters for connection */
void SIM800::ftp_connect(char *url, char *username, char *pass)
{
    if (protocol == FTPS)
        httpSSL(SET, "1");

    ftpBearerID(SET, "1");
    ftpServerAddr(SET, url);
    ftpUsername(SET, username);
    ftpPassword(SET, pass);
}


void SIM800::ftp_get(char *filename, char *filepath)
{
    ftpDownName(SET, filename);
    ftpDownPath(SET, filepath);

    ftpGetFile(SET, "1");

    while(!simCom->available());
    Serial.print(simCom->readString());
}


void SIM800::ftp_put(char *filename, char *filepath)
{
    ftpUpName(SET, filename);
    ftpUpPath(SET, filepath);

    ftpPutFile(SET, "1");
    
    while(!simCom->available());
    Serial.print(simCom->readString());
}

void SIM800::ftp_write(char *str, uint16_t len)
{
    char val[8];
    sprintf(val, "2,%u", len);
    ftpPutFile(SET, val);

    writeln(str);
    read(); // get success code
}

uint16_t SIM800::ftp_read(char *str, uint16_t len)
{
    char val[8];
    sprintf(val, "2,%u", len);

    // Manually handle this part, to be able to read any type of file (bin, hex etc.)
    outBuilder(SET, val, P("FTPGET"));
    writeln(ioBuffer);

    while(!simCom->available());
   
    Serial.println(simCom->readStringUntil('\n'));  // AT+FTPGET=2,len
    
#ifdef SIM900    
    simCom->readStringUntil('\n');
#endif

    String ans = simCom->readStringUntil('\n');     // +FTPGET=2,len
    Serial.println(ans);

    uint16_t available = ans.substring(ans.indexOf(",") + 1).toInt();
    uint16_t i;

    for (i = 0; i < available; ++i)
    {
        str[i] = simCom->read();
        delay(2);
    }

    simCom->readString();  // OK

    return available;
}