#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define N 100000000
#define N_CORE 4


int* a;


struct part
{
	double sum_dis;
	int sum;
	int start;
	int end;
}arg;

struct part threads[N_CORE];

void* my_thread(void* arg1)
{
	struct part* arg = (struct part*) arg1;
	int i, sum = 0;
	double aver, sum_dis = 0.0;
	for(i = arg->start ; i < arg->end; i++)
		sum += a[i] ;
	aver = sum / (N / N_CORE);
	for(i = arg->start; i < arg->end; i++)
		sum_dis += (aver - a[i])*(aver - a[i]);
	arg->sum = sum;	
	arg->sum_dis = sum_dis;

	return NULL;
}

int main()
{
	int sum = 0, i;
	double sum_dis = 0.0, dis, aver;
	pthread_t* thread_id;
	int* result;
	a = (int *)malloc(N * sizeof(int));
	for(i = 0; i < N; i++)
		a[i] = rand() % 20 - 10; 
	for(i = 0; i < N_CORE; i++)
	{
		threads[i].start = i * N / N_CORE;
		threads[i].end = (i + 1) * N / N_CORE;
	}
	thread_id = malloc(N_CORE * sizeof(pthread_t));
	result = malloc(N_CORE * sizeof(int));

	for(i = 0; i < N_CORE; i++)
		result[i] = pthread_create(thread_id + i, (pthread_attr_t *)NULL, my_thread, (threads + i));
	
	
	for(i = 0; i < N_CORE; i++)
		pthread_join(thread_id[i] , (void **) NULL);
	
	
	for(i = 0; i < N_CORE; i++)
	{
		sum += threads[i].sum;
		sum_dis += threads[i].sum_dis;
	}
	aver = (double)sum / N;
	dis = sum_dis / N;
	printf("%.5f %.5f\n", aver, dis);	
	return 0;
}
