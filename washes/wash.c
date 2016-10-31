#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>


#define TABLE_LIMIT 3
#define N_DISHES 6

struct dishes
{
	char dish[15]; 	//тип посуды
	int time;		//время время выполнения
}washer[N_DISHES], wiper[N_DISHES];

char pathname[] = "key"; 
int semid;
struct sembuf mybuf;
void sem(int n_sem, int n)
{
	mybuf.sem_op = n;
	mybuf.sem_flg = 0;
	mybuf.sem_num = n_sem;
	if (semop(semid , &mybuf , 1) < 0) 
	{
		printf("Can`t wait for condition\n");
		exit(-1);
	}
}




int main()
{
	
	key_t key; 
	FILE* fl;
	int i,fd[2],j,n,result;
	char one_d[15];
	char one_d1[15];
	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}
	if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) 
    {
        printf("Can`t get semid\n");
        exit(-1);
	}
	if ((fl = fopen("washer.txt","r"))==NULL) //считываем из файла для мойщика в массив структур для мойщика
	{
		printf("File not found.\n");
	}
	for(i = 0; i < N_DISHES; i++)
	{
		fscanf(fl, "%s %*c %d", washer[i].dish, &washer[i].time);
	}
	fclose(fl);
	
	if ((fl = fopen("wiper.txt","r"))==NULL) //считываем из файла для протерающего в структуру для протерающего
	{
		printf("File not found.\n");
	}
	for(i = 0; i < N_DISHES; i++)
	{
		fscanf(fl, "%s %*c %d", wiper[i].dish, &wiper[i].time);
	}
	fclose(fl);
	sem(0,2); //создаем место на столе для нашей зеркально-чистой посуды
	pipe(fd);
	result = fork();
	if(result > 0) // родак
	{
		if ((fl = fopen("dishes.txt","r"))==NULL) //открываем фаил
		{
			printf("File not found.\n");
		}
		while(fscanf(fl, "%s %*c %d", one_d, &n) != EOF) //читаем строки до конца(one_d - название посуды, n - кол-во)
			for(i = 0; i < N_DISHES; i++)  //перебираем название посуды мойщика и сравниваем с читаемым типом
				if(strcmp(one_d, washer[i].dish) == 0) 
				{
					for(j = 0; j < n; j++)  //делаем для n посуд этого типа
					{
						sleep(washer[i].time); //моем нашу посудку
						sem(0, -1);  //занимаем место на столе
						printf("посудомойщик моет %s около %d секунд\n",washer[i].dish, washer[i].time);
						write(fd[1], washer[i].dish, 15); //пишем название типа в pipe
					}
					break;
				}
		fclose(fl);
	}
	else
	{
		while(1)
		{
			read(fd[0], one_d1, 15);
			for(i = 0; i < N_DISHES; i++)  //перебираем название посуды вытиральщика и сравниваем с читаемым типом
				if(strcmp(one_d1, wiper[i].dish) == 0) 
					{
						sem(0,1); //добавляем место на столе
						sleep(wiper[i].time); //вытираем нашу посудку
						printf("протиральщик протер %s за %d\n", wiper[i].dish, wiper[i].time);
					}
		}
	}
	return 0;
}
		
			
			
			
					
						
						
					
			

	
	
	
