void *worker(void *data)
{
    WorkerData my_data = *((WorkerData *) data);
    //std::cout << my_data.path;
    std::string request = my_data.method + " " + my_data.path + " HTTP/1.0\n" + my_data.headers;
    //char buffer[512] = "GET / HTTP/1.0\n\n";

    //std::cout << request;

    char *buffer = new char[request.length() + 1];
    strcpy(buffer, request.c_str());

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERROR opening socket";

    //my_data = (struct thread_data *) threadarg;
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms1 = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    if (connect(sockfd,(struct sockaddr *)&my_data.serv_addr,sizeof(my_data.serv_addr)) < 0)
        std::cout << "ERROR connecting";
    int n;

    //std::cout << buffer;

    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0)
        std::cout << "ERROR writing to socket";

    int bytes_num = 2;
    bool collect_body = false;
    std::string body_string = "";
    std::string header_string = "";    

    while (1) {
        char res_buffer[bytes_num];
        bzero(res_buffer, bytes_num);
        n = read(sockfd, res_buffer, bytes_num-1);
        

        if (collect_body){
            if (my_data.headers_only > 0)
                break;
            body_string += std::string(res_buffer);
        }
        else
            header_string += std::string(res_buffer);

        if (header_string.find("\r\n\r\n") > 0 && !collect_body){
            collect_body = true;
        }
        
        if (n == 0)
            break;
    }
    

    //std::cout << "RESULT" << res_string << "\n";

    if (n < 0) std::cout << "ERROR reading from socket";

    close(sockfd);
    gettimeofday(&tp, NULL);
    long int ms2 = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    struct Result current_result;
    current_result.time = ms2 - ms1;
    current_result.body = body_string;
    current_result.headers = header_string;

    pthread_mutex_lock(&mutexsum);

    results.push_back(current_result);
    pthread_mutex_unlock (&mutexsum);
    
    pthread_exit(NULL);
    
}