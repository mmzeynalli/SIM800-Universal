public:

    void tcp_init();
    void tcp_open(char *, char *);
    void tcp_send(char *);
    void tcp_end();
    void tcp_read(char *);

private:
    mode_t tcp_get_status();              // Checks SIM800 network status (needed to know before sending data). 