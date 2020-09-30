#include <SIM800.h>

// PUBLIC FUNCTIONS
void SIM800::http_init()
{
    mode_t st = http_get_status();

    if (st == OFF)
    {
        turn_on();
        st = IDLE;
    }
        
    if ((st == DATA_MODE) || (st == IDLE))        // no need/use for configuration
        return;

    ipBearer(SET, "3,1,\"Contype\",\"GPRS\"");    // Configure profile 1 connection as "GPRS"
    ipBearer(SET, "3,1,\"APN\",\"internet\"");    // Set profile 1 access point name to "internet"
    
    ipBearer(SET, "1,1");
    send_failed |= (NULL == strstr(ioBuffer, "OK"));

    httpInit(EXE);                                // Initialize HTTP functionality
    httpParams(SET, "\"CID\",1");                 // Choose profile 1 as HTTP channel

    httpSave(EXE);                                // Save settings
}

void SIM800::http_post(char *dest, char *data)
{
    char str[101];
    snprintf(str, 100, "\"URL\",\"%s\"", dest);

    httpParams(SET, str);   // Set URL

    if (protocol == HTTPS)
        httpSSL(SET, "1");

    sprintf(str, "AT+HTTPDATA=%d,20000", strlen(data));
    
    simCom->println(str);
    millis_delay(200); // just a little delay
    simCom->println(data);
    read();

    httpAction(SET, "1");
    send_failed |= (NULL == strstr(ioBuffer, "OK"));
}

void SIM800::http_get(char *dest, char *data)
{
    
    char url[101];
    snprintf(url, 100, "\"URL\",\"%s\"", dest);

    httpParams(SET, url);   // Set URL

    if (protocol == HTTPS)
        httpSSL(SET, "1");
    
    httpAction(SET, "0");
}

void SIM800::http_read(char *arr)
{
    http_get_code();

    if (send_failed)        // if send failed, cannot read
        return "";
    
    httpRead(EXE);

    char *p = strtok(ioBuffer, " ");    // AT+HTTPREAD
    p = strtok(NULL, " ");              // +HTTPREAD:

#ifndef SIM900
    p = strtok(NULL, " ");              // Datalen -> comment this line for SIM900, as Datalen is not seperated from +HTTPREAD:
#endif

    strcpy(arr, strtok(NULL, " "));           // Data itself
    free(p);
}

void SIM800::http_end()
{
    httpEnd(EXE);
    ipBearer(SET, "0,1");
}

// PRIVATE FUNCTIONS
mode_t SIM800::http_get_status()
{
    netReg(GET);
    
    if (!strcmp(ioBuffer, "TIMEOUT"))
        return OFF;

    if ((NULL == strstr(ioBuffer, ",1")) && (NULL == strstr(ioBuffer, ",5")))
        return IDLE;

    ipBearer(SET, "2,1");  // read IP

    if (NULL != strstr(ioBuffer, "0.0.0.0"))  // no ip: not in data mode
        return NETWORK_CONNECTED;

    return DATA_MODE;
}

void SIM800::http_get_code()
{
    if (send_failed)
        return;
    
    setTimeout(5000);
    read();
    send_failed |= (NULL == strstr(ioBuffer, ",200"));
}