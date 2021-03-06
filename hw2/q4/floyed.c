#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <sys/time.h>
//#include <omp.h>

double seconds() {
    struct timeval tp;
    struct timezone tzp;
    int i = gettimeofday(&tp, &tzp);
    return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}
void mySwap(int *A, int *B, int cA){
	int *C=A;
	A=B;
	B=C;

	printf("A:\n");
    for (int i=0;i<cA*cA;i++){
    	printf("%d ",A[i]);
	}
	printf("\n");
	printf("B:\n");
    for (int i=0;i<cA*cA;i++){
    	printf("%d ",B[i]);
	}
}

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

 const int inf=INFINITY;


void FloydWarshallCPU(float *A,float *result, int cA){
	
	{
		for(int k=0;k<cA;k++){
			
			for(int i=0;i<cA;i++){
				for(int j=0;j<cA;j++){
					result[i*cA+j]=min(A[i*cA+j],A[i*cA+k]+A[k*cA+j]);
				}
			}

			if (k<cA-1){

				
				for (size_t i=0;i<cA*cA;i++){
					A[i]=result[i];
				}
			}
		}
	}
}



void FloydWarshallGPU(float *A,float *result, int cA){

	#pragma acc data copyin(A[0:cA*cA-1]) copyout(result[0:cA*cA-1])
	{
		for(int k=0;k<cA;k++){
			#pragma acc kernels
			for(int i=0;i<cA;i++){
				for(int j=0;j<cA;j++){
					result[i*cA+j]=min(A[i*cA+j],A[i*cA+k]+A[k*cA+j]);
				}
			}

			if (k<cA-1){

				#pragma acc parallel loop
				for (size_t i=0;i<cA*cA;i++){
					A[i]=result[i];
				}
			}
		}
	}
}

int checkError(const float* matrixA, const float* matrixB, size_t n) {
    for (size_t i = 0 ; i < n; ++i) {
            if (fabs(matrixA[i] - matrixB[i]) > 1.0e-3) {
                printf(" ! Wrong index: %zu \n", i);
                printf("%f \t %f\n",matrixA[i],matrixB[i]);
                return 0;
            }
        
    }
    return 1;
}
void init(float* A, size_t n) {
  float r;
  for (size_t i = 0;i < n; ++i) {
    for (size_t j = 0;j < n; ++j) {
       if (i==j) {
         A[i*n+j]=0.0;
         continue;
       }
       r = (float)rand() / (float)RAND_MAX * 10.0;
       A[i*n+j] = r;
    }
  }
}
int main(){
	int cA=(1<<6);
	float* A , *B;
    A = (float*) malloc(sizeof(float) * cA * cA);
    B = (float*) malloc(sizeof(float) * cA * cA);
    init(A, cA);
 	//float coordinates_defaults[16] = {0,inf,3,inf,2,0,inf,inf,inf,7,0,1,6,inf,inf,0};
 
 	//memcpy(A, coordinates_defaults, sizeof(coordinates_defaults));
    //for (int i=0;i<cA*cA;i++){
    //	A[i]=i;
    //	B[i]=cA*cA-1-i;
    //}
    
    //randomInit(B,cA*cA);

    double startTime, endTime;
    startTime = seconds();
    FloydWarshallCPU(A,B,cA);
    endTime = seconds() - startTime;
    printf("CPU time %.2f",endTime);
    //printf("A:\n");
    //for (int i=0;i<cA*cA;i++){
    //	printf("%.0f ",A[i]);
	//}
	//printf("\n");
	//printf("B:\n");
    //for (int i=0;i<cA*cA;i++){
    //	printf("%.0f ",B[i]);
	//}

	//printf("GPU version:\n");
	startTime = seconds();
    FloydWarshallGPU(A,B,cA);
    endTime = seconds() - startTime;
    printf("GPU time %.2f",endTime);
    //printf("A:\n");
    //for (int i=0;i<cA*cA;i++){
    //	printf("%.0f ",A[i]);
	//}
	//printf("\n");
	//printf("B:\n");
    //for (int i=0;i<cA*cA;i++){
    //	printf("%.0f ",B[i]);
	//}



}