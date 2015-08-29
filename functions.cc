#include <iostream>                                                                                                                                 
#include <node.h>
#include <nan.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <unistd.h>
#include <typeinfo>
#include <cstring>
#include <typeinfo>
#include <pthread.h>

using namespace v8;

struct thread_data{
       int  thread_id;
       char *message;
       struct sockaddr_in serv_addr;
    };

using namespace v8;
using namespace Nan;
const int NUM_THREADS = 10;
char results[NUM_THREADS][512];



void *PrintHello(void *threadarg)
{
    struct thread_data *my_data;
    char buffer[512] = "GET / HTTP/1.0\n\n";
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        std::cout << "ERROR opening socket";

    my_data = (struct thread_data *) threadarg;

    //std::cout << sockfd;

    if (connect(sockfd,(struct sockaddr *)&my_data->serv_addr,sizeof(my_data->serv_addr)) < 0) 
        std::cout << "ERROR connecting";
    int n;
    n = write(sockfd,buffer,strlen(buffer));

    if (n < 0) 
        std::cout << "ERROR writing to socket";

    bzero(buffer,512);
    n = read(sockfd,buffer,511);
    if (n < 0) 
        std::cout << "ERROR reading from socket";

    strcpy(results[my_data->thread_id], buffer);
    std::cout << buffer << "\n ============== \n";
    std::cout << "Thread ID : " << my_data->thread_id ;
    close(sockfd);
    pthread_exit(NULL);
    
}

void getUrl(const char *hostname, int portno)
{
    std::cout << "\n\n" << portno << "\n\n";
    int sockfd, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    //const char hostname[] = "ya.ru";
    //int portno = 80;

    //std::cout << "\n" << hostname[0] << hostname[1] << hostname[2] << hostname[3] << hostname[4] << sizeof hostname << "\n";
    int hostnameLength = strlen(hostname);
    char *hostWithZero = new char[hostnameLength + 1];
    strcpy(hostWithZero, hostname); 

    server = gethostbyname(hostWithZero);
    if (server == NULL) {
        std::cout << "ERROR, no such host\n";
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    

    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];
    int rc;
    int i;
    

    for( i=0; i < NUM_THREADS; i++ ){
      std::cout <<"main() : creating thread, " << i << std::endl;
      td[i].thread_id = i;
      td[i].message = "This is message";
      td[i].serv_addr = serv_addr;

      rc = pthread_create(&threads[i], NULL,
                          PrintHello, (void *)&td[i]);

      if (rc){
         std::cout << "Error:unable to create thread," << rc << std::endl;
         exit(-1);
      }
    }

    for( i=0; i < NUM_THREADS; i++ ){
        pthread_join(threads[i], NULL);
    }

    //pthread_exit(NULL);
    std::cout << "ALL WORKERS ARE ITERATED\n\n\n";
    
     //return buffer;
}


 
NAN_METHOD(open)
{
    const char *hostname = *v8::String::Utf8Value(args[0]->ToString());
    std::cout << hostname;
    //string s = string(*cmd);
    getUrl(hostname, args[1]->ToInteger()->Value());
    std::cout << "ALL WORKERS ARE ITERATED\n\n\n";
    NanReturnValue(NanNew<String>("werwer"));
}


