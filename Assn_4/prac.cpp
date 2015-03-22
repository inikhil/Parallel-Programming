#include<stdio.h>
#include<iostream>
#include<mpi.h>
using namespace std;

int main(int argc, char** argv){
	int myrank, x=1, y=2, z=3;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	switch(myrank){
		case 0:
			x=4;y=5;z=6;
			MPI_Bcast(&x,1,MPI_INT,0,MPI_COMM_WORLD);
			MPI_Send(&y,1,MPI_INT,2,43,MPI_COMM_WORLD);
			MPI_Bcast(&z,1,MPI_INT,1,MPI_COMM_WORLD);
			break;
		case 1:
			MPI_Bcast(&x,1,MPI_INT,0,MPI_COMM_WORLD);
			MPI_Bcast(&y,1,MPI_INT,1,MPI_COMM_WORLD);
			break;
		case 2:
			MPI_Bcast(&z,1,MPI_INT,0,MPI_COMM_WORLD);
			MPI_Recv(&x,1,MPI_INT,0,43,MPI_COMM_WORLD,&status);
			MPI_Bcast(&y,1,MPI_INT,1,MPI_COMM_WORLD);
	}
	printf("[Process %d] x = %d y = %d z = %d\n", myrank,x,y,z);
	/*if(myrank==2){
		cout<<x<<" "<<y<<" "<<z<<endl;
	}*/
	MPI_Finalize();
	return 0;


}

