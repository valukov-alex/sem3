#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int n_clients = 0;
int cliefd[100];

void* my_thread(int* fd)
{
	int n;
	char line[1000];
	while((n = read(*fd, line, 999)) > 0){
		printf("%s", line);	
		for(int i = 0; i < n_clients; i++)
			if((n = write(cliefd[i], line, strlen(line)+1)) < 0){
				perror(NULL);
				close(*fd);
				exit(1);
			}
	}
    if(n < 0){
            perror(NULL);
            close(*fd);
            exit(1);
        }
	return NULL;	
}

int main()
{
    int sockfd;
    unsigned int clilen; 
    int result[100];
    pthread_t thread_id[100]; 
    struct sockaddr_in servaddr, cliaddr; 
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family= AF_INET;
    servaddr.sin_port= htons(51009);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    if(listen(sockfd, 5) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    while(1){
        clilen = sizeof(cliaddr);
        if((cliefd[n_clients] = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        result[n_clients] = pthread_create(&thread_id[n_clients], (pthread_attr_t *)NULL, my_thread, (int*)&cliefd[n_clients]);
        n_clients++;
 //       close(newsockfd);
    }
}
