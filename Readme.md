Работаем через Linux
### MPI
Компиляция
```
mpicc -o mpi.o MPI.c -lm
```
Запуск
```
mpirun -np 4 ./mpi_program
```
### OpenMP

gcc -fopenmp -o program_name program_name.c -lm
