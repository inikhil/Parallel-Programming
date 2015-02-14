#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<mpi.h>
using namespace std;

#define SEND_TAG 998
#define RET_TAG  998

void print(int **a, int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(a[i][j]==INT_MAX/2){
				cout<<"-"<<" ";
			}
			else{
				cout<<a[i][j]<<" ";
			}
		}
		cout<<"\n";
	}
	cout<<"\n";
}

int** memdec(int n){
	int i;
	int **a=new int*[n];
	for(i=0;i<n;i++){
		a[i]=new int[n];
	}	
	return a;
}

int** path_creation(int n){
	int **m,i,j;
	m=memdec(n);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(i==j){
				m[i][j]=1;				
			}
			else{
				m[i][j]=rand()%2;
			}
		}
	}
	//print(m,n);
	return m;
}

int** weight_assign(int **m,int n){
	int **w,i,j;
	w=memdec(n);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(m[i][j]==1){
				if(i==j){
					w[i][j]=0;
				}
				else{
					w[i][j]=rand()%10+1;
				}
			}
			else{
				w[i][j]=INT_MAX/2;
			}
		}
	}
	//print(w,n);
	return w;
}

void floyd(int **w,int n){
	int i,j,k;
	for(k=0;k<n;k++){
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				w[i][j]=min(w[i][j],w[i][k]+w[k][j]);
			}
		}
	}
	print(w,n);
}

int main(int argc, char **argv){
	int n;
	//cout<<"Enter the number of vertexs: ";
	//cin>>n;
	int myrank, nprocs,k,j,i;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	n=nprocs;
	int **m,**w,ierr;
	m=path_creation(n);
	w=weight_assign(m,n);
	int *buff=new int[n];
	MPI_Status status; 
	if(myrank==n-1){
		print(m,n);
		print(w,n);
		//floyd(w,n);
	}
	
	for(k=0;k<n;k++){
		if(myrank==k){
			for(i=0;i<n;i++){
				buff[i]=w[k][i];
			}
		}
		MPI_Bcast(buff,n,MPI_INT,k,MPI_COMM_WORLD);
		for(i=0;i<n;i++){
			w[k][i]=buff[i];
		}
		for(j=0;j<n;j++){
			w[myrank][j]=min(w[myrank][j],w[myrank][k]+w[k][j]);
		}
	}
	if(myrank==0){
		for(j=1;j<n;j++){
			ierr=MPI_Recv(&buff[0],n,MPI_INT,j, RET_TAG, MPI_COMM_WORLD, &status);
			if(ierr!=MPI_SUCCESS){
				cout<<"Error!";
			}
			for(i=0;i<n;i++){
				w[j][i]=buff[i];
			}
		}
		print(w,n);
	}
	if(myrank!=0){
		for(i=0;i<n;i++){
			buff[i]=w[myrank][i];
		}
		ierr=MPI_Send(&buff[0],n,MPI_INT,0,SEND_TAG,MPI_COMM_WORLD);
		if(ierr!=MPI_SUCCESS){
			cout<<"Erroe!";
		}
	}
	MPI_Finalize();
	return 0;
}

