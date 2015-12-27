class WorkerData
{
   public:
    struct sockaddr_in serv_addr;
    std::string method;
    std::string hostname;
    std::string path;
    std::string headers;
    std::string body;
    int headers_only;
    void init(const char *method, const char *hostname, const char *path, int portno, const char *headers, const char *body, int headers_only);
};

void WorkerData::init(const char *method, const char *hostname, const char *path, int portno, const char *headers, const char *body, int headers_only)
{
    //struct sockaddr_in serv_addr;
    struct hostent *server;
    this->method = method;
    this->hostname = hostname;
    this->path = path;
    this->headers = headers;
    this->body = body;
    this->headers_only = headers_only;

    server = gethostbyname(hostname);

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