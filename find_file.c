#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

char file_name[] = "stack.c\0";


void DFS(char* path, int debt, char* filename, int *isFound)
{
	if(debt < 0)
		return;
	DIR* d;
	struct dirent *dir;
	struct stat *buf;

	dir = malloc(sizeof(struct dirent));
	buf = malloc(sizeof(struct stat));
	d = opendir(path);
	while((dir = readdir(d)) != NULL)
	{
		printf("%s %d %ld %d\n", dir->d_name, dir->d_type, buf->st_size, buf->st_mode);
		if(!strcmp(filename, dir->d_name))
		{
			printf("НАШЛИ В ПАПКЕ %s\n",path);
			*isFound = 1;
			return;
		}
		if(dir->d_type == 4 && strcmp(dir->d_name,"..") && strcmp(dir->d_name,".")) 
		{
			printf("\nвошли в папку %s\n\n", dir->d_name);
			char way[100];
			strcpy(way, path);
			strcat(way, "/");
			strcat(way, dir->d_name);
			DFS(way, debt - 1, file_name, isFound);
			printf("\nвышли из папки %s\n\n", dir->d_name);
		}
	}

	return;
}

int main(int argc, char **argv)
{
	int debt = 2;
	int isFound = 0;
	DFS(".", debt, file_name, &isFound);
	printf("%d\n", isFound);
	return 0;
}

