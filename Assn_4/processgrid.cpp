#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<mpi.h>
using namespace std;

#define SEND_TAG 998
#define RET_TAG  998

int main(int argc, char **argv){

	int myrank, nprocs, newrank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	//printf("Hello form %d of %d \n",myrank,nprocs);

	int dims=2;
	int size[2];
	int periodic[2];
	int cords[2];
	MPI_Comm grid_comm, col_comm, row_comm;
	size[0]=0;
	size[1]=0;
	periodic[0]=0;
	periodic[1]=0;
	MPI_Dims_create(nprocs, dims, size);
	MPI_Cart_create(MPI_COMM_WORLD, dims, size, periodic, 1, &grid_comm);

	MPI_Cart_coords(grid_comm, myrank, dims, cords);	
		
	MPI_Comm_split(grid_comm, cords[1], cords[0], &col_comm);
	MPI_Comm_split(grid_comm, cords[0], cords[1], &row_comm);

	if(myrank==11){
		cout<<"My cordinates are: "<<cords[0]<<" "<<cords[1]<<endl;
		MPI_Comm_rank(col_comm, &newrank);
		cout<<"My new rank in column: "<<newrank<<endl;
		MPI_Comm_rank(row_comm, &newrank);
		cout<<"My new rank in row: "<<newrank<<endl;
		//MPI_Comm_color(col_comm, &color);
		//cout<<"Color is "<<color<<endl;
	}

	MPI_Finalize();
	return 0;
}

