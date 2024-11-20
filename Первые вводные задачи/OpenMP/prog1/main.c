#include <stdio.h>
#include <omp.h>

int main() {
    // Устанавливаем количество потоков (в данном случае 4, или можно указать динамически OMP_NUM_THREADS).
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    printf("hello\n");

    // Параллельный блок с использованием OpenMP
    #pragma omp parallel 
    {
        int thread_id = omp_get_thread_num();   // Получаем ID текущего потока
        int total_threads = omp_get_num_threads(); // Общее количество потоков

        // Выводим сообщение от каждого потока
        printf("Hello World! Thread %d of %d on processor.\n", thread_id, total_threads);
    }

    return 0;
}
