public:

    void ftp_init();
    void ftp_connect(char *, char *, char * = "");
    void ftp_put(char *, char * = "\"/\"");
    void ftp_get(char *, char * = "\"/\"");
    void ftp_write(char *, uint16_t);
    uint16_t ftp_read(char *, uint16_t);
    void ftp_end();