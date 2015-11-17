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
using namespace Nan;

std::vector<std::string> results;

#include "data.cc"
#include "worker.cc"
#include "cluster.cc"
#include "async.cc"


NAN_METHOD(open) {
    const char *hostname = *Nan::Utf8String(info[1]->ToString());
    const char *path = *Nan::Utf8String(info[2]->ToString());
    
    //getUrl(hostname, info[2]->ToInteger()->Value(), info[0]->ToInteger()->Value());
    WorkerData data;
    data.init(hostname, path, info[3]->ToInteger()->Value());
    Nan::Callback *callback = new Callback(info[4].As<Function>());
    Nan::AsyncQueueWorker(new Async(callback, data, info[0]->ToInteger()->Value() ));

}


