#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 4000
#define JSIZE 4000

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
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }

    printf("init %d\n", rank);


    clock_t start;
    if (rank == 0)
    {
        printf("Start computation\n");
        start = clock();
    }

    // Считаем границы

    int portion_size = JSIZE / size;
    int start_column = rank * portion_size;
    int end_column = (rank + 1) * portion_size;

    if (rank == size - 1)
    {
        end_column = JSIZE - 1;
    }

    ///////////////////////////////////////////
    double tempRow[JSIZE];

    for (i = 1; i < ISIZE; i++)
    {
        for (j = start_column; j < end_column; j++)
        {
            a[i][j] = sin(2 * a[i - 1][j + 1]);
        }

        if (rank != 0)
        {
            MPI_Send(&a[i][start_column], portion_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&a[i][0], JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else
        {
            for (int p = 1; p < size; p++)
            {
                MPI_Recv(&tempRow[0], portion_size, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int k = 0; k < portion_size; k++)
                {
                    a[i][p * portion_size + k] = tempRow[k];
                }
            }
            for (int p = 1; p < size; p++)
            {
                MPI_Send(&a[i][0], JSIZE, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
            }
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
