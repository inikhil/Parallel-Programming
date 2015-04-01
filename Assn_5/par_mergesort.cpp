#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <limits.h>
#include <omp.h>

using namespace std;

void print(int *a, int n){

	int i;
	for(i=0; i<n; i++){
		cout<<a[i]<<" ";
	}
	cout<<"\n";
}

int search(int *a, int p, int r, int x){

	int q;
	while(p < r){
		q = (p+r)/2;
		if(a[q] <=x && a[q+1]>x){
			return q+1;
		}			
		else if(a[q] > x){
			r = q;
		}	
		else{
			p = q;
		}
	}
	if(p == r){
		return p;
	}
}

void recur_merge(int *a, int p, int r, int *left, int p1, int r1, int *right, int p2, int r2){

	int q1, q2, q3, x, i, j, k;
	j = p2;
	i = p1;
	int n1 = r1-p1;
	int n2 = r2-p2;
	static int iter=1;
	if(n1 <= 5 && n2 <= 5){
		for(k=p; k<=r; k++){
			if(left[i] <= right[j]){
				a[k] = left[i];
				i++;
			}
			else{
				a[k] = right[j];
				j++;
			} 
		}
		//print(a,4);
		return;
		
	}

	/*if(p1 > r1 && p2 > r2){return;}
	else if(p1 > r1){
		for(i=p; i<r; i++){
			a[i] = right[p2+j];
			j++;
		}
		j = 0;
	}
	else if(p2 > r2){
		for(i=p; i<r; i++){
			a[i] = left[p1+j];
			j++;
		}
		j = 0;
	}	*/
	else if(p < r){
		q1 = (r1-p1)/2;
		x = left[q1];
		q2 = search(right, p2, r2, x);
		q3 = p + (q1-p1) + (q2-p2);
		a[q3] = x;
		//print(a, r);
		#pragma omp task shared(a, left, right)
		{
    		recur_merge(a, p, q3-1, left, p1, q1-1, right, p2, q2-1);
		}
		#pragma omp task shared(a, left, right)
		{
			if(iter == 1){
				recur_merge(a, q3+1, r, left, q1+1, r1-1, right, q2, r2-1);
				iter++;
			}
			else{
				recur_merge(a, q3+1, r, left, q1+1, r1, right, q2, r2);
			}
		}
		return;
	}
}

void merge_par(int *a, int p, int q, int r){

	int n1, n2, i, j, k;
	n1 = q-p+1;
	n2 = r-q;
	int *left = new int[n1+1];
	int *right = new int[n2+1];

	for(i=0; i<n1; i++){
		left[i] = a[p+i];
	}
	
	for(i=0; i<n2; i++){
		right[i] = a[q+i+1];
	}

	left[n1] = INT_MAX;
	right[n2] = INT_MAX;

	recur_merge(a, p, r, left, 0, n1, right, 0, n2);

}		

void mergesort(int *a, int p, int r){

	int q;
	if(p < r){
		q = (p+r)/2;
		#pragma omp task shared(a)
		{		
			mergesort(a, p, q);
		}
		#pragma omp task shared(a)
		{
			mergesort(a, q+1, r);
		}
		#pragma omp taskwait
		merge_par(a, p, q, r);
	}

}

int main(){
	
	int n, i;
	cin >> n;
	int *a = new int[n];

	for(i=0; i<n; i++){
		a[i] = rand()%n;
	}
	print(a, n);
	#pragma omp task shared(a)
	{
		mergesort(a, 0, n-1);
	}
	print(a, n);
}
	
