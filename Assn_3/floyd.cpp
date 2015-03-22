#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
using namespace std;

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
	print(m,n);
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
	print(w,n);
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

int main(){
	int n;
	cout<<"Enter the number of vertexs: ";
	cin>>n;
	int **m,**w;
	m=path_creation(n);
	w=weight_assign(m,n);
	w=floyd(w,n);
	print(w,n);
}
