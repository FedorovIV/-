#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 1

long long int N = 1000000000;
double global_sum = 0;
pthread_mutex_t mutex; // Объявляем мьютекс как глобальную переменную

void *calculatePartialSum(void *arg) {
    int thread_num = *((int *)arg);
    double partial_sum = 0;
    
    for (int i = thread_num; i <= N; i += NUM_THREADS) {
        partial_sum += 1.0 / i;
    }

    pthread_mutex_lock(&mutex);
    global_sum += partial_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        N = atoi(argv[1]);
    }

    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL); // Инициализируем мьютекс

    int thread_args[NUM_THREADS];

    for (int t = 0; t < NUM_THREADS; t++) {
        thread_args[t] = t + 1;
        pthread_create(&threads[t], NULL, calculatePartialSum, &thread_args[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("The partial sum of the harmonic series with N=%d is: %f\n", N, global_sum);

    pthread_mutex_destroy(&mutex); // Уничтожаем мьютекс

    pthread_exit(NULL);
}