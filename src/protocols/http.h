public:

    void http_init();
    void http_post(char *, char *);
    void http_get(char *);


    void http_end();
    void http_read(char *);


private:
    mode_t http_get_status();              // Checks SIM800 network status (needed to know before sending data). 
    void http_get_code();