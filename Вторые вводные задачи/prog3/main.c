#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

int sharedVar = 0; // Глобальная переменная
pthread_mutex_t mutex; // Мьютекс для защиты доступа к sharedVar

void *incrementValue(void *arg) {
    int thread_id = *((int *)arg);

    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        sharedVar++;
        printf("Thread %d: SharedVar = %d\n", thread_id, sharedVar);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    
    // Инициализация мьютекса
    pthread_mutex_init(&mutex, NULL);

    for (int t = 0; t < NUM_THREADS; t++) {
        thread_args[t] = t + 1;
        pthread_create(&threads[t], NULL, incrementValue, &thread_args[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Уничтожение мьютекса
    pthread_mutex_destroy(&mutex);

    return 0;
}
