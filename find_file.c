#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int n = 9;
char file_name[10] = "stack.c\0";


void DFS(char* path, int debt, char* filename)
{
	if(0 <= debt)
	{
		DIR* d;
		struct dirent *dir;
		struct stat *buf;
	
		dir = malloc(sizeof(struct dirent));
		buf = malloc(sizeof(struct stat));
		d = opendir(path);
		while((dir = readdir(d)) != NULL)
		{
			stat(dir->d_name, buf);
			printf("%s %d %ld %d\n", dir->d_name, dir->d_type, buf->st_size, buf->st_mode);
			if(!strcmp(filename, dir->d_name))
			{
				printf("НАШЛИ В ПАПКЕ %s\n",path);
				exit(-1);
			}
			if(dir->d_type == 4 && strcmp(dir->d_name,"..") && strcmp(dir->d_name,".")) 
			{
				printf("\nвошли в папку %s\n\n", dir->d_name);
				char lol[100];
				strcpy(lol, path);
				strcat(lol, "/");
				strcat(lol, dir->d_name);
				DFS(lol, debt-1, file_name);
				printf("\nвышли из папки %s\n\n", dir->d_name);
			}
		}
	}
	return;
}

int main(int argc, char **argv)
{
	int debt = 2;
	DFS(".", debt, file_name);
	return 0;
}



