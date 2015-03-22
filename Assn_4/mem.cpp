#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;

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

void genmatrix(int **mat,int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			mat[i][j]=rand()%10;
		}
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

int main(){
	int n=10,i;
	int *buff=new int[n*n];
	int **mat=new int*[n];
	for(i=0;i<n;i++){
		mat[i]=buff + (i*n);
	}
	genmatrix(mat,n);
	printmatrix(mat,n);
	printbuffer(buff,n);
	transpose(mat,n);
	printmatrix(mat,n);
	printbuffer(buff,n);
}
