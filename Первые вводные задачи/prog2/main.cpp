#include <iostream>
#include <mpi.h>
#include <stdint.h>

using namespace std;

//mpiexec -n 4 main.exe 77  

int getMinIndexForSum(int myid, int numprocs, int N)
{

    return N / (numprocs - 1) * (myid - 1) + 1;
}

int getMaxIndexForSum(int myid, int numprocs, int N)
{
    if (myid == (numprocs - 1))
    {
        return N;
    }
    else
    {
        return N / (numprocs - 1) * (myid);
    }
}
int main(int argc, char *argv[])
{

    const int N = stoi(argv[1]);

    MPI_Init(&argc, &argv);

    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int numprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    if (myid > 0)
    {
        cout << "My proc id is " << myid << ", min index is " << getMinIndexForSum(myid, numprocs, N) << ", my max index is " << getMaxIndexForSum(myid, numprocs, N) << endl;

        double sum = 0;

        int imin = getMinIndexForSum(myid, numprocs, N);
        int imax = getMaxIndexForSum(myid, numprocs, N);

        for (int i = imin; i <= imax; i++)
        {
            sum += 1.0 / i;
        }
        
        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {

        double resultSum = 0;
        for (int i = 1; i < numprocs; i++){
            double isum = 0;
            MPI_Recv(&isum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            resultSum+=isum;
        }
        cout<<"My id is "<<myid<<", result is: "<<resultSum;
    }

    // cout<<"hello"<<endl;
    // int myid, numprocs;
    // int  namelen;
    // char processor_name[MPI_MAX_PROCESSOR_NAME];
    // MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    // MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    // MPI_Get_processor_name(processor_name,&namelen);
    // std::cout<<"Hello World! Process "<<myid<<" of "<<numprocs<<" on "<<processor_name<<std::endl;

    
    MPI_Finalize();
    return 0;
}