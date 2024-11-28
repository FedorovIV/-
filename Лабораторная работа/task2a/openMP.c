#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define ISIZE 4000
#define JSIZE 4000

//Введи кол-во потоков как аргумент

int main(int argc, char *argv[])
{
    // Проверка аргументов командной строки
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    // Получение количества потоков из аргументов командной строки
    int num_threads = atoi(argv[1]);
    if (num_threads <= 0)
    {
        fprintf(stderr, "Invalid number of threads: %d. Must be positive.\n", num_threads);
        return 1;
    }

    omp_set_num_threads(num_threads);
    printf("Using %d threads\n", num_threads);

    FILE *ff;

    // Динамическое выделение памяти
    double **a = (double **)malloc(ISIZE * sizeof(double *));
    if (a == NULL)
    {
        fprintf(stderr, "Memory allocation failed for rows!\n");
        return 1;
    }
    for (int i = 0; i < ISIZE; i++)
    {
        a[i] = (double *)malloc(JSIZE * sizeof(double));
        if (a[i] == NULL)
        {
            fprintf(stderr, "Memory allocation failed for columns!\n");
            return 1;
        }
    }

    // Инициализация массива
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }

    printf("Start computation\n");
    double start = omp_get_wtime();

    // Распараллеливание расчета
    for (int i = 1; i < ISIZE; i++)
    {
// Переменная `i` определяет текущее состояние, тут нет гонки данных.
#pragma omp parallel for
        for (int j = 0; j < JSIZE - 1; j++)
        {
            a[i][j] = sin(2 * a[i - 1][j + 1]);
        }
    }

    double end = omp_get_wtime();
    double time_taken = end - start;
    printf("Time taken for computation: %f seconds\n", time_taken);

    // Запись результата в файл
    ff = fopen("resultOpenMP.txt", "w");
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {
            fprintf(ff, "%f ", a[i][j]);
        }
        fprintf(ff, "\n");
    }
    fclose(ff);

    // Освобождение памяти
    for (int i = 0; i < ISIZE; i++)
    {
        free(a[i]);
    }
    free(a);

    return 0;
}
