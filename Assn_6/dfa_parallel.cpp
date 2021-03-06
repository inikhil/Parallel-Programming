#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<omp.h>
#include<cmath>

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
	int n, m, f, i, j, t, len, init;
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
	int *inputstring = new int[len];

	for(i=0; i<n; i++){
		fstates[i] = 0;	
	} 

	for(i=0; i<f; i++){
		t = rand() % n; 
		//cout << t << " ";
		fstates[t] = 1;
	}
	//cout << "\n";

	for(i=0; i<len; i++){
		t = rand() % m;
		//cout << t << " ";
		inputstring[i] = t;
	}
	//cout << "\n";

	int max_threads = 2;
	int segment = ceil((double)len/max_threads);
	
	int **temp_state = new int*[max_threads];
	for(i=0; i<max_threads; i++){
		temp_state[i] = new int[n];
		for(j=0; j<n; j++){
			temp_state[i][j] = 0;
		}
	}
	#pragma omp parallel num_threads(max_threads)
	{
		int th_id = omp_get_thread_num();
		int st = th_id * segment;
		int end = min(len, st+segment);
		
		if(th_id == 0){
			init = 0;
			for(i=st; i<end; i++){
				init = a[init][inputstring[i]];
			}
			temp_state[th_id][0] = init;
		}
		else{
			#pragma omp parallel for 
			for(i=0; i<n; i++){
				init = i;
				for(j=st; j<end; j++){
					init = a[init][inputstring[j]];
				}
				temp_state[th_id][i] = init;
			}
		}
	}	
	//printmatrix(temp_state, max_threads, n);
	
	init=0;
	for(i=0; i<max_threads; i++){
		init = temp_state[i][init];
	}

	if(fstates[init] == 1){
		cout << "String Accepted\n";
	}
	else{
		cout << "String Rejected\n";
	}

}		
