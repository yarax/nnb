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
std::vector<std::string> results;

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

void *worker(WorkerData &my_data)
{

    std::cout << my_data.path;
    return 0;
    //struct thread_data *my_data;
    char buffer[512] = "GET / HTTP/1.0\n\n";
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        std::cout << "ERROR opening socket";

    //my_data = (struct thread_data *) threadarg;

    if (connect(sockfd,(struct sockaddr *)&my_data.serv_addr,sizeof(my_data.serv_addr)) < 0) 
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

    //std::cout << buffer << "\n ============== \n";
    //std::cout << "Thread ID : " << my_data->thread_id ;
    close(sockfd);
    pthread_exit(NULL);
    
}

void cluster(WorkerData &data, int num_threads)
{
    
    int i;
    int rc;
    pthread_t threads[num_threads];
    for( i=0; i < num_threads; i++ ){

      rc = pthread_create(&threads[i], NULL,
                          worker, (void *)(&data));

      if (rc){
         std::cout << "Error:unable to create thread," << rc << std::endl;
         exit(-1);
      }
    }

    for( i=0; i < NUM_THREADS; i++ ){
        pthread_join(threads[i], NULL);
    }

    //pthread_exit(NULL);
     //return buffer;
}

NAN_METHOD(open) {
    const char *hostname = *Nan::Utf8String(info[1]->ToString());
    const char *path = *Nan::Utf8String(info[2]->ToString());
    
    //getUrl(hostname, info[2]->ToInteger()->Value(), info[0]->ToInteger()->Value());
    WorkerData data;
    data.init(hostname, path, info[3]->ToInteger()->Value());
    cluster(data, info[0]->ToInteger()->Value());

    Local<Array> obj = Nan::New<Array>();
    int i = 0;

    for( i=0; i < results.size(); i++ ){
        obj->Set(i, Nan::New<String>(results[i].c_str()).ToLocalChecked() );
    }

    info.GetReturnValue().Set(obj);
}


