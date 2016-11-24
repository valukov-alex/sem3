#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/*
 * что это за n?
 */
int n = 9;

/*
 * FIXIT:
 * 1) magic number 10
 * 2) \0 не обязательно самому ставить: поставится автоматически
 */
char file_name[10] = "stack.c\0";


void DFS(char* path, int debt, char* filename)
{
  /* не очень здорово, когда вся логика вашей ф-и в здоровом if-е: сложно разбираться и можно со скобками запутаться.
   * лучше в самом начале просто написать if (debt < 0) return;
  */
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
      // FIXIT: у вас неправильно программа работает. неверные аргементы в stat передаете. на больших глубинах будет особенно проявляться
			stat(dir->d_name, buf);
			printf("%s %d %ld %d\n", dir->d_name, dir->d_type, buf->st_size, buf->st_mode);
			if(!strcmp(filename, dir->d_name))
			{
				printf("НАШЛИ В ПАПКЕ %s\n",path);
        // FIXIT: представьте, что эта ф-я является частью какой-то большой программы. если вы убъете весь процесс, после того, как нашли, то особо толку никакого не будет.
        // нужно, например, последним параметром ф-и передать int* isFound, куда в случае успеха записать 1, иначе 0.
				exit(-1);
			}
			if(dir->d_type == 4 && strcmp(dir->d_name,"..") && strcmp(dir->d_name,".")) 
			{
				printf("\nвошли в папку %s\n\n", dir->d_name);
        /*
         * 1) magic number
         * 2) нужно более адекватное назвать переменную
         */
				char lol[100];
				strcpy(lol, path);
				strcat(lol, "/");
				strcat(lol, dir->d_name);
				DFS(lol, debt - 1, file_name);
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



