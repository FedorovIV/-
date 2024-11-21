#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 50
#define JSIZE 50

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

    clock_t start = clock();

    int portion_size = ISIZE / size;
    int start_row = rank * portion_size + 1;
    int end_row = (rank == size - 1) ? ISIZE : start_row + portion_size;

    for (i = start_row; i < end_row; i++)
    {
        for (j = 0; j < JSIZE - 1; j++)
        {
            a[i][j] = sin(2 * a[i - 1][j + 1]);
        }
    }

    if (rank != 0)
    {
        MPI_Send(&a[start_row][0], portion_size * JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (int p = 1; p < size; p++)
        {
            int recv_start_row = p * portion_size + 1;
            int recv_end_row = (p == size - 1) ? ISIZE : recv_start_row + portion_size;
            MPI_Recv(&a[recv_start_row][0], (recv_end_row - recv_start_row) * JSIZE, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (rank == 0)
    {
        printf("Time taken for computation: %f seconds\n", time_taken);

        ff = fopen("result.txt", "w");
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
