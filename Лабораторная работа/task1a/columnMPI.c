#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 20
#define JSIZE 20
#define MAX_ROW_IN_MESSAGE 100

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

    // Каждый процесс считает свой столбик
    for (i = 1; i < ISIZE; i++)
    {
        for (j = start_column; j < end_column; j++)
        {
            a[i][j] = sin(2 * a[i - 1][j + 1]);
        }
    }

    if (rank ==  0)
    {
        for (i = 0; i < ISIZE; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                printf("%f ", a[i][j]);
            }
            printf("\n");
        }
    }

    int rowsInbuffer = (ISIZE <= MAX_ROW_IN_MESSAGE) ? ISIZE : MAX_ROW_IN_MESSAGE;
    int numOfMessagesForProcces = ISIZE / rowsInbuffer;

    if (rank != 0)
    {
        for (i = 0; i < numOfMessagesForProcces; i++)
        {
            MPI_Send(a[i * rowsInbuffer], rowsInbuffer * JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        double buffer[rowsInbuffer][JSIZE];

        for (int p = 1; p < size; p++)
        {
            for (i = 0; i < numOfMessagesForProcces; i++)
            {
                MPI_Recv(buffer, rowsInbuffer * JSIZE, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int i_l, j_l;

                for (i_l = i * rowsInbuffer; i_l < (i + 1) * rowsInbuffer; i_l++)
                {
                    for (j_l = p * portion_size; j_l < (p + 1) * portion_size; j_l++)
                    {
                        a[i_l][j_l] = buffer[i_l % rowsInbuffer][j_l];
                    }
                }
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
