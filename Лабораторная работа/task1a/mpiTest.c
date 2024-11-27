#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 3000
#define JSIZE 3000

int main(int argc, char **argv)
{
    FILE *ff;

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Rank: %d, Size: %d\n", rank, size);

    int i, j;
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
    if (rank == 1)
    {
        for (int i = 0; i < ISIZE; i++)
        {
            for (int j = 0; j < JSIZE; j++)
            {
                a[i][j] = 10 * i + j;
            }
        }
    }
    else
    {
        for (int i = 0; i < ISIZE; i++)
        {
            for (int j = 0; j < JSIZE; j++)
            {
                a[i][j] = 0;
            }
        }
    }

    // Запуск таймера
    clock_t start;
    if (rank == 0)
    {
        printf("Start computation\n");
        start = clock();
    }

    // Пересылаем массив 0-му исполнителю
    int buf_size = 10000;
    double buffer[buf_size];
    int count = 0;

    if (rank == 1)
    {
        int count = 0; // Инициализация счетчика
        for (int i = 0; i < ISIZE; i++)
        {
            for (int j = 0; j < JSIZE; j++)
            {
                // buffer[count] = a[i][j]; // Запись элемента в буфер
                count += 1;

                if (count == buf_size)
                { // Проверка на заполнение буфера
                    MPI_Send(buffer, buf_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                    count = 0; // Сброс счетчика
                }
            }
        }

        // Оставшиеся элементы буфера (если они есть)
        if (count > 0)
        {
            MPI_Send(buffer, count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }
    else if (rank == 0)
    {
        int count = 0; // Инициализация счетчика
        for (int i = 0; i < ISIZE; i++)
        {
            for (int j = 0; j < JSIZE; j++)
            {
                if (count == 0)
                { // Получение нового буфера, если текущий был пуст
                    MPI_Recv(buffer, buf_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }

                a[i][j] = buffer[count]; // Запись данных из буфера в массив
                count += 1;

                if (count == buf_size)
                { // Сброс счетчика, если буфер исчерпан
                    count = 0;
                }
            }
        }

        // Последний прием (если остались "хвостовые" данные)
        if (count > 0)
        {
            MPI_Recv(buffer, count, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if (rank == 0)
    {
        clock_t end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Time taken for computation: %f seconds\n", time_taken);
    }

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

    // Освобождение памяти
    for (int i = 0; i < ISIZE; i++)
    {
        free(a[i]);
    }
    free(a);

    MPI_Finalize();
    return 0;
}
