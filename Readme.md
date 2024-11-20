Работаем через Linux
### MPI
Компиляция
```
mpicc -o mpi.out mpi.c -lm
```
Запуск
```
mpirun -np 4 ./mpi.out
```
### OpenMP

gcc -fopenmp -o program_name program_name.c -lm
