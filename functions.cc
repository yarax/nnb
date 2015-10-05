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
std::vector<std::string> results;

void *worker(void *threadarg)
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

    results.push_back(std::string(buffer));

    std::cout << buffer << "\n ============== \n";
    std::cout << "Thread ID : " << my_data->thread_id ;
    close(sockfd);
    pthread_exit(NULL);
    
}

void getUrl(const char *hostname, int portno)
{
    std::cout << "\n\n" << portno << " " << hostname << "\n\n";
    int sockfd, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    //const char hostname[] = "ya.ru";
    //int portno = 80;

    //std::cout << "\n" << hostname[0] << hostname[1] << hostname[2] << hostname[3] << hostname[4] << sizeof hostname << "\n";
    int hostnameLength = strlen(hostname);
    char *hostWithZero = new char[hostnameLength + 1];
    strcpy(hostWithZero, hostname); 
    std::cout << "before";
    server = gethostbyname(hostWithZero);
    std::cout << "after";
    if (server == NULL) {
        std::cout << "ERROR, no such host\n" << hostWithZero << " " << hostname;
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
                          worker, (void *)&td[i]);

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

NAN_METHOD(open) {
    Local<Array> v8Array = Nan::New<Array>();
    std::string str = "erwer";
    v8Array->Set(0, Nan::New<String>(str.c_str()) );
    //v8Array->Set(0, Nan::New<Integer>(12) );
    info.GetReturnValue().Set(v8Array);
}

/*NAN_METHOD(open) {
    Local<Array> v8Array = Nan::New<Array>();
    std::string str = "erwer";
    v8Array->Set(0, Nan::New<Integer>(12) );
    info.GetReturnValue().Set(v8Array);
}*/
 
/*NAN_METHOD(open) {
    const char *hostname = *Nan::Utf8String(info[0]->ToString());
    std::cout << "host: ";
    std::cout << *hostname;
    std::cout << "port: ";
    std::cout << *Nan::Utf8String(info[1]->ToString());

    //string s = string(*cmd);
    getUrl(hostname, info[1]->ToInteger()->Value());
    std::cout << "ALL WORKERS ARE ITERATED\n\n\n";

    Local<Array> v8Array = Nan::New<Array>();
    std::cout << "before";
    int i = 0;
    std::cout << "!!!" << results.size();
    for( i=0; i < results.size(); i++ ){
        //std::cout << results[i] << i << "EWRWRWEREWRWER";
        //v8Array->Set(i, Nan::New<String>( results[i].c_str() ));
        v8Array->Set(i, Nan::New<String>(results[i].c_str()) );
    }

    
    info.GetReturnValue().Set(v8Array);
    std::cout << "after";
    //NanReturnValue(NanNew<String>("werwer"));
}*/


