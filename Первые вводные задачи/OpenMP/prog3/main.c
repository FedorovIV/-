#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// Функция для генерации случайного целого числа в диапазоне от 1 до max
int generateRandInt(int max) {
    unsigned int now = (unsigned int)time(NULL);
    srand(now + omp_get_thread_num()); // Дополнение для генерации разных чисел в каждом потоке
    return (rand() % max) + 1;
}

int main(int argc, char* argv[]) {
    // Количество потоков
    int numprocs = 4;

    omp_set_num_threads(numprocs);

    // Переменная для хранения случайного числа
    int randInt = 0;
    int message = 0;

    // Переменная для управления порядком выполнения потоков
    volatile int thread_order = 0;

    // Используем параллельную область
    #pragma omp parallel shared(randInt, message, thread_order)
    {
        int myid = omp_get_thread_num();

        // Каждый поток ждет своего номера (myid) для выполнения
        while (thread_order != myid) {
            #pragma omp flush(thread_order) // Обеспечивает видимость переменной между потоками
        }

        // Поток с идентификатором 0 генерирует случайное число
        if (myid == 0) {
            const int MAX_RAND = 100;
            randInt = generateRandInt(MAX_RAND);
            printf("Мой ранг %d. Я отправил: %d\n", myid, randInt);
            message = randInt; // Инициируем передачу
        } else {
            printf("Мой ранг %d. Я получил: %d\n", myid, message);
            message += myid; // Добавляем номер потока к сообщению
            printf("Мой ранг %d. Я отправил: %d\n", myid, message);
        }

        // Переход к следующему потоку
        #pragma omp critical
        {
            thread_order++;
        }
        #pragma omp flush(thread_order) // Обновляем значение переменной
    }

    // Печатаем финальный результат в основном потоке
    printf("Итоговый результат: %d\n", message);

    return 0;
}
