#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
}