#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;

void printmatrix (int **a, int n, int m){
	int i, j;
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			cout << a[i][j] << " ";
		}
		cout<<"\n";
	}
}


int main(){
	int n, m, f, i, j, t, len;
	//cin >> n >> m >> f >> len;
	n = 50; m = 26; f = 5; len = 1000000; 

	int **a = new int*[n];
	for (i=0; i<n; i++ ){
		a[i] = new int[m];
		for(j=0; j<m; j++){
			a[i][j] = rand() % n;
		}
	}
	//printmatrix(a, n, m);
	int *fstates = new int[n];

	for(i=0; i<n; i++){
		fstates[i] = 0;	
	} 

	for(i=0; i<f; i++){
		t = rand() % n; 
		//cout << t << " ";
		fstates[t] = 1;
	}
	//cout << "\n";
	 
	int initst = 0;
	for(i=0; i<len; i++){
		t = rand() % m;
		//cout << t << " ";
		initst = a[initst][t];
	}

	//cout << "\n";

	if(fstates[initst] == 1){
		cout << "String Accepted\n";
	}
	else{
		cout << "String Rejected\n";
	}

}		
