#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Расчет индекса начала для текущего потока
int getMinIndexForSum(int thread_id, int num_threads, int N) {
    return N / num_threads * thread_id + 1;
}

// Расчет индекса конца для текущего потока
int getMaxIndexForSum(int thread_id, int num_threads, int N) {
    if (thread_id == num_threads - 1) {
        return N;
    } else {
        return N / num_threads * (thread_id + 1);
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    // Считываем входное значение N
    const int N = atoi(argv[1]);

    // Получаем количество доступных потоков
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    double resultSum = 0.0;

    // Основной параллельный блок
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();       // ID текущего потока
        int num_threads = omp_get_num_threads();   // Общее число потоков

        // Получаем диапазон индексов для текущего потока
        int imin = getMinIndexForSum(thread_id, num_threads, N);
        int imax = getMaxIndexForSum(thread_id, num_threads, N);

        // Вывод информации о текущем потоке
        printf("Thread %d: min index = %d, max index = %d\n", thread_id, imin, imax);

        // Подсчет частичной суммы для текущего потока
        double partialSum = 0.0;
        for (int i = imin; i <= imax; i++) {
            partialSum += 1.0 / i;
        }

        // Добавляем частичную сумму к общей (синхронизация через reduction)
        #pragma omp atomic
        resultSum += partialSum;
    }

    // Вывод общего результата
    printf("Result sum is: %f\n", resultSum);

    return 0;
}