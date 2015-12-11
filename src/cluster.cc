void cluster(WorkerData &data, int concurency)
{
    
    int i;
    int rc;

    pthread_t threads[concurency];
    pthread_mutex_init(&mutexsum, NULL);
    for( i=0; i < concurency; i++ ){

      rc = pthread_create(&threads[i], NULL,
                          worker, (void *)(&data));

      if (rc){
         std::cout << "Error:unable to create thread," << rc << std::endl;
         exit(-1);
      }
    }

    for( i=0; i < concurency; i++ ){
        pthread_join(threads[i], NULL);
    }

    //pthread_exit(NULL);
     //return buffer;
}