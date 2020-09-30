#include <SIM800.h>

String server_data;

void SIM800::tcp_init()
{

    gprsAttach(SET, "1");

    mode_t st = tcp_get_status();

    if (st == OFF)
    {
        turn_on();
        st = IDLE;
    }
        
    if ((st == DATA_MODE) || (st == IDLE))    // no need || use for configuration
        return;


    if (protocol == TCP_TRANSPARENT)
        ipAppMode(SET, "1");
    else
        ipAppMode(SET, "0");

    ipAccess(SET, "\"www\",\"\",\"\"");
    
    ipBegin(EXE);
    read();
    send_failed |= (NULL == strstr(ioBuffer, "OK"));
    
    ipGetLocalAddr(EXE);
    send_failed |= (NULL != strstr(ioBuffer, "ERROR"));

    ipSaveCfg(EXE);
}

void SIM800::tcp_open(char *url, char *port)
{
    char val[100];
    sprintf(val, "\"TCP\",\"%s\",\"%s\"", url, port);
    
    ipOpen(SET, val);
    send_failed |= (NULL == strstr(ioBuffer, "OK"));

    if (!send_failed)  // if previous was OK, wait for "CONNECT OK"
    {
        while(!simCom->available());
        send_failed |= (NULL == strstr(simCom->readString().c_str(), "CONNECT"));
    }
}


void SIM800::tcp_send(char *data)
{
    if (send_failed)
        return;
    
    switch (protocol)
    {
    case TCP:
        ipSend(EXE);
        
        write(data);
        write("\x1A");
        break;
    case TCP_TRANSPARENT:
        writeln(data);    
    default:
        break;
    }
    
    read();
}

void SIM800::tcp_read(char *str)
{
    if (send_failed)
        return;

    char *p = ioBuffer;

    if (protocol == TCP)
    {
        p = strtok(p, "OK");
        p = strtok(NULL, "OK");
    }

    strcpy(str, p);
    free(p);
}

void SIM800::tcp_end()
{
    ipClose(SET, "1");
    ipDeactivate(EXE);
}

/* PRIVATE FUNCTIONS */
mode_t SIM800::tcp_get_status()
{
    gprsAttach(GET);

    if (NULL != strstr(ioBuffer, "TIMEOUT"))
        return OFF;

    if (NULL == strstr(ioBuffer, "+CGATT: 1"))
        return IDLE;

    setTimeout(1000);
    ipGetLocalAddr(EXE);

    if (NULL != strstr(ioBuffer, "ERROR"))
        return NETWORK_CONNECTED;

    return DATA_MODE;
}