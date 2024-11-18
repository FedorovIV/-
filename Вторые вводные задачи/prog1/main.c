#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *printHello(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello World from thread %ld\n", tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return -1;
    }

    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    int rc;
    long t;

    for(t=0; t<num_threads; t++) {
        rc = pthread_create(&threads[t], NULL, printHello, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            return -1;
        }
    }

    pthread_exit(NULL);
}