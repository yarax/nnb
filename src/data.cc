class WorkerData
{
   public:
    struct sockaddr_in serv_addr;
    std::string path;
    void init(const char *hostname, const char *path, int portno);
};

void WorkerData::init(const char *hostname, const char *path, int portno) 
{
    //struct sockaddr_in serv_addr;
    struct hostent *server;
    this->path = path;
    int hostnameLength = strlen(hostname);
    char *hostWithZero = new char[hostnameLength + 1];
    strcpy(hostWithZero, hostname); 
    server = gethostbyname(hostWithZero);
    if (server == NULL) {
        std::cout << "ERROR, no such host\n" << hostWithZero << " " << hostname;
        exit(0);
    }
    bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
    this->serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    this->serv_addr.sin_port = htons(portno);
}