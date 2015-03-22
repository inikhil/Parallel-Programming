#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstdlib>

using namespace std;

void print(int *a, int n){
	int i;
	for(i=0; i<n; i++){
		cout<<a[i]<<" ";
	}
	cout<<"\n";
}

int main(){
	int n, i, j;
	cin>>n;
	int *a=new int[n];
	int *b=new int[n];
	int *c=new int[2*n-1];
	for(i=0; i<n; i++){
		a[i] = rand()%10;
		b[i] = rand()%10;
	}

	//print(a, n);
	//print(b, n);

	for(i=0; i<2*n-1; i++){
		c[i] = 0;
	}

	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			c[i+j] = c[i+j] + a[i]*b[j];
		}
	}

	//print(c, 2*n-1);
	return 0;
}	
	
