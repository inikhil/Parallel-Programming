#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
  int myrank, nprocs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  cout << "Hello from processor " << myrank << " of " << nprocs << endl;

  MPI_Finalize();
  return 0;
}
