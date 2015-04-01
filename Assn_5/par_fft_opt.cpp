
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <complex.h>
#include <omp.h>


using namespace std;

void print(int *a, int n){

	int i;
	for(i=0; i<n; i++){
		cout<<a[i]<<" ";
	}
	cout<<"\n";
}

void print_real_complex(double complex *y, int n){

	int i;
	for(i=0; i<n-1; i++){
		cout << creal(y[i]) << " ";
	}
	cout<<"\n";
}

void print_complex(double complex *y, int n){

	int i;
	for(i=0; i<n; i++){
		cout << creal(y[i]) << " + " << cimag(y[i]) << "i" <<endl;
	}
	cout<<"\n";
}

double complex* multiply(double complex *p, double complex *q, int n ){
	
	int i;
	double complex *y = new double complex[n];
	for(i=0; i<n; i++){
		y[i] = p[i] * q[i];
	}
	return y; 
}

double complex* inverse_recursive_fft(double complex *a, int n){

	int k, i;
	double complex w, w_n;

	if(n == 1) {return a;}

	w_n = cos ((2*M_PI*(-1))/n) + sin((2*M_PI*(-1))/n) * I;
	w = 1;

	double complex *b = new double complex[n/2];
 	double complex *c = new double complex[n/2];
	double complex *y0 = new double complex[n/2];
	double complex *y1 = new double complex[n/2];
	double complex *y = new double complex[n];

	for(i=0; i<n/2; i++){
		b[i]=a[i*2];
	}
	
	for(i=1;i<=n/2;i++){
		c[i-1]=a[2*i-1];
	}
	#pragma omp parallel sections num_threads(4)
	{
		#pragma omp section
		{
			y0 = inverse_recursive_fft(b, n/2);
		}
		#pragma omp section
		{ 
			y1 = inverse_recursive_fft(c, n/2);
		}
		#pragma omp barrier
	}
	//#pragma omp barrier
  	for(k=0; k<n/2; k++){
		y[k] = y0[k] + w*y1[k];
		y[k+n/2] = y0[k] - w*y1[k];
		w = w*w_n;
	}
	return y; 

}
	
	

double complex* recursive_fft(int *a,int n){

	int k, i;
	int th_id, nthreads;
	if(n == 1){
		double complex *y = new double complex[n];
		double complex z = a[0];
		y[0] = z;
		return y;
	}
	double complex w, w_n;

	w_n = cos ((2*M_PI)/n) + sin((2*M_PI)/n) * I;
	//cout<<creal(w_n)<<" "<<cimag(w_n)<<endl;
	w = 1;

	int *b = new int[n/2];
 	int *c = new int[n/2];
	double complex *y0 = new double complex[n/2];
	double complex *y1 = new double complex[n/2];
	double complex *y = new double complex[n];

	for(i=0; i<n/2; i++){
		b[i]=a[i*2];
	}
	
	for(i=1;i<=n/2;i++){
		c[i-1]=a[2*i-1];
	}
	#pragma omp parallel sections num_threads(4)
	{
		#pragma omp section
		{
			y0 = recursive_fft(b, n/2);
		}
		#pragma omp section
		{ 
			y1 = recursive_fft(c, n/2);
		}
		#pragma omp barrier
	}
	//#pragma omp barrier

  	for(k=0; k<n/2; k++){
		y[k] = y0[k] + w*y1[k];
		y[k+n/2] = y0[k] - w*y1[k];
		w = w*w_n;
	}
	return y; 
}
 
int main(){

	int n, i, t, th_id, nthreads;
	t = 100;

	n=1;
	while(n < t){
		n = n*2;
	}

	int *a=new int[2*n];
	int *b=new int[2*n];

	for(i=0; i<t; i++){
		a[i] = rand()%10;
		b[i] = rand()%10;
	}
	
	for(i=t; i<2*n; i++){
		a[i] = 0;
		b[i] = 0;
	}

	//print(a, t);
	//print(b, t);

	double complex *y = new double complex[2*n];
	double complex *z = new double complex[2*n];

	#pragma omp parallel sections num_threads(4)
	{

		//th_id = omp_get_thread_num();
		//nthreads = omp_get_num_threads();
		//cout<<th_id<<" "<<nthreads<<endl;
		#pragma omp section
		{
			//cout<<th_id<<" "<<nthreads<<endl;
			y = recursive_fft(a, 2*n);
		}

		#pragma omp section
		{
			//th_id = omp_get_thread_num();
			//nthreads = omp_get_num_threads();
			//cout<<th_id<<" "<<nthreads<<endl;
			z = recursive_fft(b, 2*n);
		}
		#pragma omp barrier
		//print_complex(y, 2*n);
	}

	y = multiply(y, z, 2*n);
	y = inverse_recursive_fft(y, 2*n);	
	
	for(i=0; i<2*n; i++){
		y[i] = y[i] / (2 * n) ;
	}

	//print_real_complex(y, 2*t);
	//print_complex(y, 2*n);

	return 0;
}	
	
