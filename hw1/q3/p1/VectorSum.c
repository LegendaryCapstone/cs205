/*
 ============================================================================
 Name        : test2.c
 Author      : hengte lin
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stddef.h>
//#include "VecotrSum.h"
/**
 * Hello OpenMP World prints the number of threads and the current thread id
 */
long long serial_sum(const char* a,size_t n);
long long time_optimal_sum(const char* a,size_t n);
long long cost_optimal_sum(const char* a,size_t n);

int ** create_matrix(int nrows,int ncolumns);


int main (int argc, char *argv[]) {

}

int test_sum () {
	size_t n = pow(2,32);
	char *a = malloc(n * sizeof(char));
	if (a == NULL) {
		 perror("malloc");
		 return 1;
	}
	unsigned long i;

#pragma omp parallel for
	for (i=0;i<n;i++){
	 a[i]=1;
	}

	clock_t start = clock();
	printf("serial sum: %ll" , serial_sum(a,n));
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf(" time %f \n" , seconds);

	start = clock();
	printf("parallel sum: %ll" , time_optimal_sum(a,n));
	end = clock();
	seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf(" time %f \n" , seconds);

	start = clock();
	printf("cost optimal sum: %ll" , cost_optimal_sum(a,n));
	end = clock();
	seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf(" time %f" , seconds);

	free(a);
	return 0;
}

int test_threads (int argc, char *argv[]) {

  int numThreads, tid;

   //This creates a team of threads; each thread has own copy of variables
#pragma omp parallel private(numThreads, tid)
 {
   tid = omp_get_thread_num();
   printf("Hello World from thread number %d\n", tid);

    //The following is executed by the master thread only (tid=0)
   if (tid == 0)
     {
       numThreads = omp_get_num_threads();
       printf("Number of threads is %d\n", numThreads);
     }
 }
 return 0;
}

long long serial_sum(const char *a, size_t n){
	long long total=0.;
	size_t i;
	for(i=0;i<n;i++){
		total+=a[i];
	}
	return total;
}

long long time_optimal_sum(const char *a, size_t n){
	long long total=0.;
	size_t i;
#pragma omp parallel for reduction(+:total) num_threads(4)
		for(i=0;i<n;i++){
			total+=a[i];
		}

	return total;
}

long long cost_optimal_sum(const char *a, size_t n){
	long long total=0.;
	long long i;
	//double logn = log((double) n);
	//int threads = (int) (n/logn);
	//printf("num threads: ", threads);

#pragma omp parallel reduction(+:total) private(i) num_threads(4)
	{
	int thread_num = omp_get_num_threads();
	int thread_id = omp_get_thread_num();
	long chunk = (long)(n/thread_num);
	for (i=thread_id*chunk;i<(thread_id+1)*chunk;i+=1){
		total+=a[i];
		}

	}
	return total;
}
//---------------------------------------------------------------------------

int ** create_matrix(int nrows,int ncolumns){
	int **array;
	const size_t row_pointers_bytes = nrows * sizeof *array;
	const size_t row_elements_bytes = ncolumns * sizeof **array;
	array = malloc(row_pointers_bytes + nrows * row_elements_bytes);

	size_t i;
	int * const data = array + nrows;
	for(i = 0; i < nrows; i++)
	  array[i] = data + i * ncolumns;

	return array;
}

long long * toptimal_matrix_time_vector(){
	long long * output;

	return output;
}

long long * coptimal_matrix_time_vector(){
	long long * output;

	return output;
}
