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
pthread_mutex_t lock;

struct arg_to_thread { int num; float sum; };

//static pthread_mutex_t *mutex_lock;

void* thread_execute(void* s) {
	int i, j, start, range;
	float sum;
	struct arg_to_thread *local_arg;
			
	local_arg = s;
	range = N / P;
	start = (*local_arg).num * range;
	
	if ((*local_arg).num == P - 1 && N % P != 0) { range = N % P; }

/* multiply partial matrix */
	for (i = start; i < start+range; i++) {
		sum = 0.0;
		for (j = 0; j < N; j++) {
			sum = sum + a[j][i] * b[j];
		}
		pthread_mutex_lock(&lock);
		x[i] = sum;
		pthread_mutex_unlock(&lock);
	}

	pthread_exit(0);
}

int main (int argc, char *argv[] )    {
/* the array size should be supplied as a command line argument */
  if(argc != 3) {printf("wrong number of arguments") ; exit(2) ;}
  N = atoi(argv[1]) ;
  P = atoi(argv[2]);
  printf("Array size = %d, # of Threads: %d\n", N, P);
  int mid = (N+1)/2;
  int i, j, w;
  double time_start, time_end;
  struct timeval tv;
  struct timezone tz;
  pthread_t t_id[P];
  struct arg_to_thread my_arg[P];

  if (pthread_mutex_init(&lock, NULL) != 0) printf("Mutex init has failed\n");	

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

/* compute start time */
	gettimeofday (&tv ,   &tz);
  time_start = (double)tv.tv_sec +
            (double)tv.tv_usec / 1000000.0;

/* initialize product array */
  for (i=0; i<N; i++) x[i] = 0.0;
  
/* create T threads to multiply partial arrays */
	for(i = 0; i < P; i++) {
		my_arg[i].num = i;
		pthread_create(&(t_id[i]), NULL, &thread_execute, (void*) &my_arg[i]);
	}

/* exit thread */
	for(i = 0; i < P; i++) {
		pthread_join(t_id[i], NULL);
	}
	pthread_mutex_destroy(&lock);
 
/* compute end time */
  gettimeofday (&tv ,  &tz);
  time_end = (double)tv.tv_sec +
          (double)tv.tv_usec / 1000000.0;

/* this is for checking the results */
  for (i=0; i<N; i+=N/10) { printf(" %10.6f",x[i]); }
  printf("\n");
  printf ("time = %lf\n", time_end - time_start);
}
