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
#include <sys/time.h>

using namespace v8;
using namespace Nan;

struct Result {
    std::string body;
    std::string headers;
    int time;

};

std::vector<Result> results;

pthread_mutex_t mutexsum;
int a = 0;

#include "data.cc"
#include "worker.cc"
#include "cluster.cc"
#include "async.cc"


NAN_METHOD(open) {
    const char *method = *Nan::Utf8String(info[1]->ToString());
    const char *hostname = *Nan::Utf8String(info[2]->ToString());
    const char *path = *Nan::Utf8String(info[3]->ToString());
    const char *headers = *Nan::Utf8String(info[5]->ToString());
    int headers_only = info[6]->ToInteger()->Value();
    int port = info[4]->ToInteger()->Value();

    //getUrl(hostname, info[2]->ToInteger()->Value(), info[0]->ToInteger()->Value());
    WorkerData data;
    data.init(method, hostname, path, port, headers, headers_only);
    Nan::Callback *callback = new Callback(info[7].As<Function>());
    Nan::AsyncQueueWorker(new Async(callback, data, info[0]->ToInteger()->Value() ));

}


