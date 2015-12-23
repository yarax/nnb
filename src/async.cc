class Async : public Nan::AsyncWorker {
 public:
  Async(Nan::Callback *callback, WorkerData data, int concurency)
    : AsyncWorker(callback), data(data), concurency(concurency) {}
  ~Async() {}
 private:
  Local<Array> obj = Nan::New<Array>();
  WorkerData data;
  int concurency;

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    cluster(data, concurency);
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    Nan::HandleScope scope;

    Local<Array> obj = Nan::New<Array>();
    unsigned long i = 0;
    std::string time("time");
    std::string body("body");
    std::string headers("headers");


    for( i=0; i < results.size(); i++ ){
        Local<Object> item = Nan::New<Object>();
        item->Set(Nan::New<String>(time.c_str()).ToLocalChecked(), Nan::New<Number>(results[i].time));
        item->Set(Nan::New<String>(body.c_str()).ToLocalChecked(), Nan::New<String>(results[i].body.c_str()).ToLocalChecked());
        item->Set(Nan::New<String>(headers.c_str()).ToLocalChecked(), Nan::New<String>(results[i].headers.c_str()).ToLocalChecked());
        
        obj->Set(i, item);
    }

    Local<Value> argv[] = {
        Nan::Null()
      , obj
    };
    callback->Call(2, argv);
  }
};