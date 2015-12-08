class WorkerData
{
   public:
    struct sockaddr_in serv_addr;
    std::string method;
    std::string hostname;
    std::string path;
    std::string headers;
    void init(const char *method, const char *hostname, const char *path, int portno, const char *headers);
};

void WorkerData::init(const char *method, const char *hostname, const char *path, int portno, const char *headers)
{
    //struct sockaddr_in serv_addr;
    struct hostent *server;
    this->method = method;
    this->hostname = hostname;
    this->path = path;
    this->headers = headers;
    /*int hostnameLength = strlen(hostname);
    char *hostWithZero = new char[hostnameLength + 1];
    strcpy(hostWithZero, hostname);*/
    server = gethostbyname(hostname);
    //std::cout << "!!!!!!";
    //std::cout << "h_name" << server->h_name << "h_aliases" << server->h_aliases << "h_addrtype" << server->h_addrtype;
    //std::cout << "!!!!!!<<";

    if (server == NULL) {
        std::cout << "ERROR, no such host " << hostname << "\n";
        exit(0);
    }
    bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
    this->serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    this->serv_addr.sin_port = htons(portno);
}