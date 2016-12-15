#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

struct clients{
	char name[30];
	int IP;
	int port;
} clnts[100], new_client;

int main()
{
	int sockfd, i;
	unsigned int clilen, n, n_clients = 0;
	char line[1000], line1[1000], name[30];
	struct sockaddr_in servaddr, cliaddr, clntsaddr[100];
  
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	} else
	{
		printf("Sock fd:%d\n", sockfd);
	}
  
	if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
  
	while (1)
	{
		bzero(line, 1000);
		clilen = sizeof(cliaddr);
		if ((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr*)&cliaddr, &clilen)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		
		new_client.IP = cliaddr.sin_addr.s_addr;
		new_client.port = cliaddr.sin_port;
		strcpy(new_client.name, line);
		
		int flg = 0;
		for(i = 0; i < n_clients; i++)
			if(new_client.IP == clnts[i].IP && new_client.port == clnts[i].port){
				strcpy(name, clnts[i].name);
				flg = 1;
				break;
			}
		if(flg == 0){
			printf("челик %s вписался\n", line);
			clnts[n_clients].IP = new_client.IP;
			clnts[n_clients].port = new_client.port;
			strcpy(clnts[n_clients].name, line);
			clntsaddr[n_clients].sin_addr.s_addr = cliaddr.sin_addr.s_addr;
			clntsaddr[n_clients].sin_port = cliaddr.sin_port;
			n_clients++;
		}
		else{
			strcpy(line1, clnts[i].name);
			strcat(line1, ":");
			strcat(line1, line);
			printf("%s\n", line1);
			for(int i = 0; i < n_clients; i++){
				if(strcmp(name, clnts[i].name))
					if (sendto(sockfd, line1, strlen(line1) + 1, 0, (struct sockaddr*) &clntsaddr[i], clilen) < 0){
						perror(NULL);
						close(sockfd);
						exit(1);
					}
			}
		}
	}
	return 0;
}

