#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define ISIZE 4000
#define JSIZE 4000

int main(int argc, char **argv)
{
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

    // Запуск таймера
    clock_t start;

    printf("Start computation\n");
    start = clock();

    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = i;
           
        }
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for computation: %f seconds\n", time_taken);

    // Освобождение памяти
    for (int i = 0; i < ISIZE; i++)
    {
        free(a[i]);
    }
    free(a);

    return 0;
}
