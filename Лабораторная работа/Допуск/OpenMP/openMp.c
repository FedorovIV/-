#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h> // Добавляем заголовочный файл OpenMP

#define ISIZE 2500
#define JSIZE 2500

int main(int argc, char **argv) {
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;

    // Подготовительная часть – заполнение некими данными
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i + j;
        }
    }

    // Измерение времени начала работы цикла
    double start = omp_get_wtime();  // Используем omp_get_wtime() вместо clock()

    // Распараллеливаем цикл с помощью OpenMP
    #pragma omp parallel for private(i, j) shared(a)
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = sin(2 * a[i][j]);
        }
    }

    // Измерение времени окончания работы цикла
    double end = omp_get_wtime();  // Используем omp_get_wtime() для получения окончательного времени

    // Вычисление времени выполнения
    double time_taken = end - start;

    // Вывод времени выполнения
    printf("Time taken for computation: %f seconds\n", time_taken);

    ff = fopen("result.txt", "w");
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            fprintf(ff, "%f ", a[i][j]);
        }
        fprintf(ff, "\n");
    }
    fclose(ff);

    return 0;
}
