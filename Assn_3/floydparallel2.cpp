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

int** floyd(int **w,int n){
	int i,j,k;
	for(k=0;k<n;k++){
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				w[i][j]=min(w[i][j],w[i][k]+w[k][j]);
			}
		}
	}
	return w;
}

int main(int argc, char **argv){
	int n;
	n=1000;
	int myrank, nprocs,k,j,i,p,q,myrank1,row,rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	int **m,**w,ierr;
	m=path_creation(n);
	w=weight_assign(m,n);
	int *buff=new int[n];
	MPI_Status status; 
	/*if(myrank==nprocs-1){
		cout<<"Mapping Matrix: \n";
		print(m,n);
		cout<<"Initial weight Matrix: \n";
		print(w,n);
	}*/
	int t=n/nprocs;
	for(k=0;k<n;k++){
		if(myrank<(nprocs-1)){
			q=t;
		}
		else{
			q=n-(myrank*t);
		}
		
		if(k>=myrank*t && k<(myrank*t+q)){
			for(p=0;p<n;p++){
				buff[p]=w[k][p];
			}
		}
		rank=k/t;
		if(rank>=nprocs){
			rank=nprocs-1;	
		}
		//printf("%d waiting for row %d proc %d\n", myrank, k, rank);
		MPI_Bcast(buff,n,MPI_INT,rank,MPI_COMM_WORLD);
		//printf("%d waiting for row %d proc %d....done\n", myrank, k, rank);
		for(p=0;p<n;p++){
				w[k][p]=buff[p];
		}
		for(i=myrank*t;i<(myrank*t+q);i++){
			for(j=0;j<n;j++){
				w[i][j]=min(w[i][j],w[i][k]+w[k][j]);
			}
		}
	}
	if(myrank==0){
		for(j=1;j<nprocs;j++){
			if(j<nprocs-1){
				q=t;
			}
			else{
				q=n-j*t;
			}
			for(i=j*t;i<j*t+q;i++){
				ierr=MPI_Recv(&row,1,MPI_INT,j, RET_TAG, MPI_COMM_WORLD, &status);
				if(ierr!=MPI_SUCCESS){
					cout<<"Error!";
				}
				ierr=MPI_Recv(&buff[0],n,MPI_INT,j, RET_TAG, MPI_COMM_WORLD, &status);
				if(ierr!=MPI_SUCCESS){
					cout<<"Error!";
				}
				for(p=0;p<n;p++){
					w[row][p]=buff[p];
				}
			}
		}
		/*cout<<"Final output Matrix: \n";
		print(w,n);*/
	}
	if(myrank!=0){
		if(myrank<nprocs-1){
			q=t;
		}
		else{
			q=n-myrank*t;
		}
		for(i=myrank*t;i<myrank*t+q;i++){
			for(p=0;p<n;p++){
				buff[p]=w[i][p];
			}
			row=i;
			ierr=MPI_Send(&row,1,MPI_INT,0,SEND_TAG,MPI_COMM_WORLD);
			if(ierr!=MPI_SUCCESS){
				cout<<"Error!";
			}
			ierr=MPI_Send(&buff[0],n,MPI_INT,0,SEND_TAG,MPI_COMM_WORLD);
			if(ierr!=MPI_SUCCESS){
				cout<<"Error!";
			}
		}
	}
	MPI_Finalize();
	return 0;
}

