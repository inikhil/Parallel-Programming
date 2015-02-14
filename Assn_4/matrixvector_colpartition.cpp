#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<mpi.h>
#include<cmath>
#include<ctime>
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

void transpose(int **mat,int n){
	int i,j,t;
	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			t=mat[i][j];
			mat[i][j]=mat[j][i];
			mat[j][i]=t;
		}
	}
}

void serial(int **mat, int *b, int n){
	clock_t t1=clock();
	int i,j,temp;
	int *c=new int[n];
	for(i=0;i<n;i++){
		temp=0;
		for(j=0;j<n;j++){
			temp+=mat[i][j]*b[j];
		}
		c[i]=temp;
	}
	clock_t t2=clock();
	cout<<"Time taken for Serial Output : "<<t2-t1<<endl;
	//printvector(c,n);
}

int main(int argc, char **argv){

	int myrank, nprocs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	//printf("Hello form %d of %d \n",myrank,nprocs);
	int n=1000,i,j,temp;
	int **mat;
	int *buff,*b;
	clock_t t3,t4;
	if(myrank==0){
		
		mat=new int*[n];
		buff=new int[n*n];
		b=new int[n];
		for(i=0;i<n;i++){
			mat[i]= buff+ (i*n);
		}		
		genmatrix(mat,n);
		genvector(b,n);
		serial(mat,b,n);
		//printmatrix(mat,n);
		//printbuffer(buff,n);
		t3=clock();
		transpose(mat,n);
		//printmatrix(mat,n);
		//printbuffer(buff,n);
	}
	int col=(int)ceil((double)n/nprocs);
	int *newb=new int[col];
	int *newbuff= new int[n*col];

	MPI_Scatter(b,col, MPI_INT,newb,col, MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(buff,n*col,MPI_INT,newbuff,n*col,MPI_INT,0,MPI_COMM_WORLD);

	int *partsum=new int[n];
	for(i=0;i<n;i++){
		temp=0;
		for(j=0;j<col;j++){
			temp+=newbuff[j*n+i]*newb[j];
		}
		partsum[i]=temp;		
	}

	int *output=new int[n*nprocs];
	MPI_Gather(partsum,n,MPI_INT,output,n,MPI_INT,0,MPI_COMM_WORLD);

	int *final=new int[n];
	if(myrank==0){
		for(j=0;j<n;j++){
			temp=0;
			for(i=0;i<nprocs;i++){
				temp+=output[i*n+j];
			}
			final[j]=temp;
		}
		t4=clock();
		cout<<"Time taken by Parallel Output: "<<t4-t3<<endl;
		//printvector(final,n);
	}
	
	MPI_Finalize();
	return 0;
}

