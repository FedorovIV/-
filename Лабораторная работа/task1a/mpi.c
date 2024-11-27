#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 4000
#define JSIZE 4000

int main(int argc, char **argv)
{
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;

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

    printf("Rank: %d, Size: %d\n", rank, size);

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
            double tempRow[JSIZE];
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

    MPI_Finalize();
    return 0;
}
