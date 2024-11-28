#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ISIZE 3000
#define JSIZE 3000

int main(int argc, char **argv)
{
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

    int i, j;
    FILE *ff;
    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }

    clock_t start = clock(); 
                
    for (i = 1; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE - 1; j++)
        {
            a[i][j] = sin(2 * a[i - 1][j + 1]);
        }
    }

    // Измерение времени окончания работы цикла
    clock_t end = clock();
    
    // Вычисление времени выполнения
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Вывод времени выполнения
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

    // Освобождение памяти
    for (int i = 0; i < ISIZE; i++)
    {
        free(a[i]);
    }
    free(a);
}