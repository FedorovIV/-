#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define ISIZE 500
#define JSIZE 500

int main(int argc, char **argv)
{
    double a[ISIZE][JSIZE];
    double local_a[ISIZE][JSIZE]; // Местный массив
    int i, j;
    FILE *ff;

    // Подготовительная часть – заполнение некими данными
    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Заполнение массива
    if (rank == 0) // только процесс 0 заполняет массив
    {
        for (int i = 0; i < ISIZE; i++)
        {
            for (int j = 0; j < JSIZE; j++)
            {
                a[i][j] = 10 * i + j;
            }
        }
    }

    // Рассылаем данные всем процессам
    MPI_Bcast(a, ISIZE * JSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Параллельные вычисления
    int rows_per_process = ISIZE / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank == size - 1) ? ISIZE : start_row + rows_per_process;

    clock_t start;
    if (rank == 0)
    {
        // Измерение времени начала работы цикла
        start = clock();
    }

    // Рассчитываем часть массива
    for (int i = start_row + 1; i < end_row; i++)
    {
        for (int j = 0; j < JSIZE - 1; j++)
        {
            a[i][j] = sin(2 * a[i - 1][j + 1]);
        }
    }

    // Сбор результатов на процессе 0
    MPI_Gather(&a[start_row][0], rows_per_process * JSIZE, MPI_DOUBLE,
               a, rows_per_process * JSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Измерение времени окончания работы цикла
        clock_t end = clock();

        // Вычисление времени выполнения
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

        // Вывод времени выполнения
        printf("Time taken for computation: %f seconds\n", time_taken);
    }

    // Записываем результат в файл только процессом 0
    if (rank == 0)
    {
        ff = fopen("resultMPI.txt", "w");
        for (i = 0; i < ISIZE; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                fprintf(ff, "%f ", a[i][j]);
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
    }

    MPI_Finalize();
    return 0;
}
