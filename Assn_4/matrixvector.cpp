#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<mpi.h>
#include<cmath>
using namespace std;

#define SEND_TAG 998
#define RET_TAG  998

void printmatrix(int **mat,int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			cout<<mat[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";
}

void printbuffer(int *buff,int n){
	int i;
	for(i=0;i<n*n;i++){
		if(i%10==0){
			cout<<"\n";				
		}
		cout<<buff[i]<<" ";
	}
	cout<<"\n";
}

void printvector(int *b,int n){
	int i;
	for(i=0;i<n;i++){
		cout<<b[i]<<" ";
	}
	cout<<"\n";
}

void genmatrix(int **mat,int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			mat[i][j]=rand()%10+1;
		}
	}
}

void genvector(int *b,int n){
	int i;
	for(i=0;i<n;i++){
		b[i]=rand()%10+1;
	}
}

void serial(int **mat, int *b, int n){

	int i,j,temp;
	int *c=new int[n];
	for(i=0;i<n;i++){
		temp=0;
		for(j=0;j<n;j++){
			temp+=mat[i][j]*b[j];
		}
		c[i]=temp;
	}
	printvector(c,n);
}

void scatter_matrix(int *buff, int *local_array, int process,int n, int col, int rank,int nprocs){

	int i,j;
	int sizes[2]= {n, n};
    int subsizes[2]= {col, col}; 
    int starts[2]= {0,0};  
    MPI_Datatype type, subarrtype;
    MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_INT, &type);
    MPI_Type_create_resized(type, 0, col*sizeof(int), &subarrtype);
    MPI_Type_commit(&subarrtype);

	int *sendcnt = new int[nprocs];
	int *disp = new int[nprocs]; 

	if(rank==0){
		for(i=0;i<nprocs;i++){
			sendcnt[i]=1;
		}
		int d=0;
		for(i=0;i<process;i++){
			for(j=0;j<process;j++){
				disp[i*process+j]=d;
				d+=1;
			}
			d+=(col-1)*process;
		}
	}

    MPI_Scatterv(buff, sendcnt, disp, subarrtype, local_array, col*col, MPI_INT, 0, MPI_COMM_WORLD);
}

void scatter_vector(int *b,int *new_b, int process,int n,int col,int cords[2],MPI_Comm row_comm){

	if(cords[0]==0){
		MPI_Scatter(b,col, MPI_INT,new_b,col, MPI_INT,0,row_comm);
	}
	
}

void compute(int *local_array, int *new_b, int *partial,int col){

	int i,j,temp;
	for(i=0;i<col;i++){
		temp=0;
		for(j=0;j<col;j++){
			temp+=local_array[i*col+j]*new_b[j];
		}
		partial[i]=temp;
	}
				
}


int main(int argc, char **argv){

	int myrank, nprocs, col_rank, row_rank, color;
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

	MPI_Comm_rank(col_comm, &col_rank);
	MPI_Comm_rank(row_comm, &row_rank);

	int n=100,i,j,temp;
	int **mat;
	int *buff,*b;

	if(myrank==0){
		mat=new int*[n];
		buff=new int[n*n];
		b=new int[n];
		for(i=0;i<n;i++){
			mat[i]= buff+ (i*n);
		}		
		genmatrix(mat,n);
		genvector(b,n);
		//serial(mat,b,n);
		//printmatrix(mat,n);
		//printbuffer(buff,n);
		//printvector(b,n);
	}
	
	int process=sqrt(nprocs);
	int col=(int)ceil((double)n/process);
	int *local_array=new int[col*col];	
	
	scatter_matrix(buff,local_array,process,n,col,myrank,nprocs);
	
	/*if(myrank==4){
		//cout<<cords[0]<<" "<<cords[1]<<endl;
		printvector(local_array,col*col);
	}*/
	
	int *new_b=new int[col];
	scatter_vector(b,new_b,process,n,col,cords,row_comm);

	/*if(myrank==4){
		cout<<"Old: \n";
		printvector(new_b,col);
	}*/

	for(i=0;i<process;i++){
		if(cords[1]==i){
			MPI_Bcast(new_b,col,MPI_INT,0,col_comm);
		}
	}
	
	/*if(myrank==15){
		cout<<"New: \n";
		printvector(new_b,col);
	}*/
	
	int *partial=new int[col];
	compute(local_array,new_b,partial,col);

	int *output=new int[col*process];
	for(i=0;i<process;i++){
		if(cords[0]==i){
			MPI_Gather(partial,col,MPI_INT,output,col,MPI_INT,0,row_comm);
		}
	}

	/*if(cords[0]==1 && cords[1]==0){
		cout<<"Printing :\n";
		printvector(output,col*process);
	}*/			
	
	int *finall=new int[col];
	int *final_output=new int[col*process];
	if(cords[1]==0){
		for(j=0;j<col;j++){
			temp=0;
			for(i=0;i<process;i++){
				temp+=output[i*col+j];
			}
			finall[j]=temp;
		}
		/*if(cords[0]==1){
			printvector(finall,col);
		}*/
		MPI_Gather(finall,col,MPI_INT,final_output,col,MPI_INT,0,col_comm);
	}
	
	/*if(cords[0]==0 && cords[1]==0){
		//cout<<"Printing: ";
		printvector(final_output,col*process);
	}*/

	MPI_Finalize();
	return 0;
}

