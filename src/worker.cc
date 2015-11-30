void *worker(void *data)
{
    WorkerData my_data = *((WorkerData *) data);
    //std::cout << my_data.path;
    std::string request = my_data.method + " " + my_data.path + " HTTP/1.0\n\n" + my_data.headers;
    //char buffer[512] = "GET / HTTP/1.0\n\n";

    //std::cout << request;

    char *buffer = new char[request.length() + 1];
    strcpy(buffer, request.c_str());

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERROR opening socket";

    //my_data = (struct thread_data *) threadarg;

    if (connect(sockfd,(struct sockaddr *)&my_data.serv_addr,sizeof(my_data.serv_addr)) < 0)
        std::cout << "ERROR connecting";
    int n;
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0)
        std::cout << "ERROR writing to socket";

    char res_buffer[1024];

    bzero(res_buffer,1024);
    n = read(sockfd,res_buffer,1023);
    if (n < 0)
        std::cout << "ERROR reading from socket";

    results.push_back(std::string(res_buffer));

    //std::cout << buffer << "\n ============== \n";
    //std::cout << "Thread ID : " << my_data->thread_id ;
    close(sockfd);
    pthread_exit(NULL);
    
}