#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 

char s[] = "lalkaa", c_put, c_get = 0;
int l,i = 0, j = 0, child, bit;
int k = 128;

void get_1_bit(int nsig){
	if(j == 8){
		sleep(1);
		fprintf(stderr,"%c", c_get);
//		printf("%c", c_get);
		fflush (stdin);
		j = 0;
		c_get = 0;
	}
	else{ 
		c_get = (c_get << 1) + 1;
		j++;
	}
	kill(child, SIGUSR1);
}

void get_0_bit(int nsig){
	if(j == 8){
		sleep(1);
		fprintf(stderr,"%c", c_get);
//		printf("%c", c_get);
		fflush (stdin);
		c_get = 0;
		j = 0;
	}
	else{
		c_get = c_get << 1;
		j++;
	}
	kill(child, SIGUSR1);
	
}

void get_char(int nsig){
	if(i == l){
		printf("\nyra\n");
		sleep(2);
		kill(child, SIGINT);
	}
	if(j == 0){
		c_put = s[i];
		i++;
	}
	bit = c_put & k;
	k = k >> 1;
	j++;
	if(j == 9){
		k = 128;
		j = 0;
	}
	if (bit > 0)
		kill(getppid(), SIGUSR1);
	else
		kill(getppid(), SIGUSR2);
	
}

int main(int argc, char **argv)
{
	l = strlen(s);
	child = fork();
	
	if(child > 0){
		signal(SIGUSR1, get_1_bit);
		signal(SIGUSR2, get_0_bit);
	}
	else
		signal(SIGUSR1, get_char);
	if(child > 0)
	{
		printf("\n");
		kill(child, SIGUSR1);
	}
	while(1);
		
	return 0;
}

