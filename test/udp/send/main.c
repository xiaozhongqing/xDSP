#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


int main(int argc, char** argv)
{
	int sock;
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("ERROR: socket failed\n");
		return -1;
	}
	const int opt = 1;
	int nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	if(nb == -1)
	{
		printf("ERROR: setsockopt failed\n");
		return -1;
	}
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	addrto.sin_port = htons(6000);
	int nlen = sizeof(addrto);
	printf("INFO: udp send start....\n");
	while(1)
	{
		char msg[] = {"adfsdfsdfsdf"};
		int ret = sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&addrto, nlen);
		if(ret < 0)
		{
			printf("ERROR: sendto failed(%d)\n", ret);
			break;
		}
		else
		{
			printf("OK....\n");
		}
		sleep(100);
	}
	printf("INFO: udp send exit ....\n");
	return 0;
}
