/****************************************************************************
 *  Matrix vector multiplication using one thread
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

int N ;
int P ;
float** a ;
float* b ;
float* x ;

void *thread_execute(int k) {
	int i;
	int range = N / P;

	if (k == P - 1 && N % P != 0) range = N % P;

/* multiply partial matrix */
	for (i = k * N/P; i < (k+1) * N/P; i++} {
	
	}
}

main (int argc, char *argv[] )    {
/* the array size should be supplied as a command line argument */
  if(argc != 3) {printf("wrong number of arguments") ; exit(2) ;}
  N = atoi(argv[1]) ;
	P = atoi(argv[2]);
  printf("Array size = %d, # of Threads: %d\n", N, P);
  int mid = (N+1)/2;
  int i, j;
  double time_start, time_end;
  struct timeval tv;
  struct timezone tz;
	pthread_t t_id[P];
	
/* allocate arrays dynamically */
  a = malloc(sizeof(float*)*N);
  for (i = 0; i < N; i++) {
		a[i] = malloc(sizeof(float)*N);
  }
  b = malloc(sizeof(float)*N);
  x = malloc(sizeof(float)*N);

  /* Inititialize matrix A and vector B. */
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      if (j == i)                    { a[i][j] = 2.0; }
      else if (j == i-1 || j == i+1) { a[i][j] = 1.0; }
      else                           { a[i][j] = 0.01; }
    }
    b[i] = mid - abs(i-mid+1);
  }

	rows_per_thread = (N - (N % P)) / P;
	
	if(N % P != 0) rows_last_thread = N % P;
	else rows_last_thread = rows_per_thread;

/* create T threads to multiply partial arrays */
	for(i = 0; i < P; i++) {
		pthread_create(&(t_id[i]), NULL, &thread_execute, );
	}

/* exit thread */
	for(i = 0; i < P; i++) {
		pthread_join(t_id[i], NULL);
	}


  gettimeofday (&tv ,   &tz);
  time_start = (double)tv.tv_sec +
            (double)tv.tv_usec / 1000000.0;

    for (i=0; i<N; i++) x[i] = 0.0;
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
         x[i] += a[i][j] * b[j];
      }
    }

  gettimeofday (&tv ,  &tz);
  time_end = (double)tv.tv_sec +
          (double)tv.tv_usec / 1000000.0;

/* this is for checking the results */
  for (i=0; i<N; i+=N/10) { printf(" %10.6f",x[i]); }
  printf("\n");
  printf ("time = %lf\n", time_end - time_start);

}
