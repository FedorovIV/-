#include <iostream>
#include <mpi.h>
#include <stdint.h>
#include <chrono>
#include <ctime> 

using namespace std;

//

int generateRandInt(int max){
    namespace sc = std::chrono;

    auto time = sc::system_clock::now(); // get the current time

    auto since_epoch = time.time_since_epoch(); // get the duration since epoch

    auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);

    unsigned int now = millis.count();

    srand(now); 

    unsigned int random_integer = rand();
    
    return (random_integer % max) + 1;
}

int main(int argc,char* argv[]){

    MPI_Init(&argc,&argv);

    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    int numprocs;
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    
    if (myid == 0){
        const int MAX_RAND = 100;
        int randInt = generateRandInt(MAX_RAND);


        MPI_Send(&randInt, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD);
        cout<<"My rank is "<<myid<<" I sent: "<<randInt<<endl;

        int result = 0;
        MPI_Recv(&result, 1, MPI_INT, numprocs-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout<<"My rank is "<<myid<<" Result is: "<<result<<endl;

    } else if(myid == numprocs-1){
        int message;
        MPI_Recv(&message, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout<<"My rank is "<<myid<<" I recieved: "<<message<<endl;

        message+=1;

        MPI_Send(&message, 1, MPI_INT, 0,  0, MPI_COMM_WORLD);

        cout<<"My rank is "<<myid<<" I sent: "<<message<<endl;


    } else {

        int message;
        MPI_Recv(&message, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout<<"My rank is "<<myid<<" I recieved: "<<message<<endl;

        message+=1;

        MPI_Send(&message, 1, MPI_INT, myid+1,  0, MPI_COMM_WORLD);

        cout<<"My rank is "<<myid<<" I sent: "<<message<<endl;
    }

    MPI_Finalize();
    return 0;
}