#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<time.h>
using namespace std;

void print(int **a,int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			cout<<a[i][j]<<" ";
		}
		cout<<"\n";
	}
}

int** memdec(int n){
	int i;
	int **a=new int*[n];
	for(i=0;i<n;i++){
		a[i]=new int[n];
	}
	return a;
}

int** initialize_zero(int **a,int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			a[i][j]=0;
		}
	}
	return a;
}

int** initialise(int **a,int n){
	int i,j,t;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			t=rand()%10;
			a[i][j]=t;
		}
	}
	return a;
}

int** mult(int **a,int **b, int n){
	int i,j,k;
	int **c;
	c=memdec(n);
	clock_t t1=clock();
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				c[i][j]+=a[i][k]*b[k][j];
			}
		}
	}
	clock_t t2=clock();
	cout<<"Time taken by normal mutilication: "<<t2-t1<<endl;
	return c;
}
int** mult_opt1(int **a,int **b,int n){
	int i,j,k,sum;
	int **c;
	c=memdec(n);
	clock_t t1=clock();
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			sum=0;
			for(k=0;k<n;k++){
				sum+=a[i][k]*b[k][j];
			}
			c[i][j]=sum;
		}
	}
	clock_t t2=clock();
	cout<<"Time taken by optimisation_1 mutilication: "<<t2-t1<<endl;
	return c;
}

int** mult_transpose(int **a,int **b,int n){
	int i,j,k,sum;
	int **c,**d;
	c=memdec(n);
	d=memdec(n);
	clock_t t1=clock();
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			d[i][j]=b[j][i];
		}
	}
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			sum=0;
			for(k=0;k<n;k++){
				sum+=a[i][k]*d[j][k];
			}
			c[i][j]=sum;
		}
	}
	clock_t t2=clock();
	cout<<"Time taken by using transpose mutilication: "<<t2-t1<<endl;
	return c;
}

int** mult_block(int **a, int **b,int n){
	int i,j,t,k,sum;
	int **c,**d;
	c=memdec(n);
	d=memdec(n);
	clock_t t1=clock();
	c=initialize_zero(c,n);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			d[i][j]=b[j][i];
		}
	}
	int nblocks=2;
	int m=n/nblocks;
	for(i=0;i<n;i++){
		for(t=0;t<nblocks;t++){
			for(k=0;k<n;k++){
				sum=0;
				for(j=t*m;j<t*m+m;j++){
					sum+=a[i][j]*d[k][j];
				}
				c[i][k]+=sum;
			}
		}
	}
	clock_t t2=clock();
	cout<<"Time taken by using block mutilication: "<<t2-t1<<endl;
	return c;
}

int** gen_matrix(int n){
	int i,j;
	int* *a=memdec(n);
	a=initialise(a,n);
	//print(a,n);
	return a;
}

int main(){
	int n;
	cout<<"Enter your value of n: ";
	cin>>n;
	int **a,**b,**c;
	a=gen_matrix(n);
	b=gen_matrix(n);
	//c=mult(a,b,n);
	//print(c,n);
	//c=mult_opt1(a,b,n);
	//print(c,n);
	c=mult_transpose(a,b,n);
	//print(c,n);
	c=mult_block(a,b,n);
	//print(c,n);
}
