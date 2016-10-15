#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define N 100000000

/*
 * FIXIT:
 * Стиль названий переменных должен быть одинаковый: например, константы - заглавные буквы, слова отделены подчёркиванием
 * N_CORE
 */
#define n_core 1

int* a;

struct part
{
	double sum_dis;
	int sum;
	int start;
	int end;
};

/*
 * FIXIT:
 * Почему вы назвали массив core?
 * core = ядро ... и?
 * Видимо стоит подумать над более подходящим названием.
 */
struct part core[n_core];

/*
 * FIXIT:
 * при компиляции не должно быть warning'ов:
 * сигнатура ф-и должна быть void* func(void* arg).
 * Приведение arg к нужному типу осуществляется внутри ф-и.
 */
void* my_thread(struct part* arg)
{
	int i, sum = 0;
	double aver, sum_dis = 0.0;
	for(i = arg->start ; i < arg->end; i++)
		sum += a[i] ;
	aver = sum / (N / n_core);
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
	for(i = 0; i < n_core; i++)
	{
		core[i].start = i * N / n_core;
		core[i].end = (i + 1) * N / n_core;
	}
	thread_id = malloc(n_core * sizeof(pthread_t));
	result = malloc(n_core * sizeof(int));

	for(i = 0; i < n_core; i++)
		result[i] = pthread_create(thread_id + i, (pthread_attr_t *)NULL, my_thread, core + i);
	
	
	for(i = 0; i < n_core; i++)
		pthread_join(thread_id[i] , (void **) NULL);
	
	
	for(i = 0; i < n_core; i++)
	{
		sum += core[i].sum;
		sum_dis += core[i].sum_dis;
	}
	aver = (double)sum / N;
	dis = sum_dis / N;
	printf("%.5f %.5f\n", aver, dis);	
	return 0;
}
