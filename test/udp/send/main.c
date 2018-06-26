#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


using namespace std;

int main(int argc, char** argv)
{
	setvbuf(stdout, 0, _IONBF, 0);
	fflush(stdout);

	int sock = -1;
	if(((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("ERROR: socket failed");
		return -1;
	}
	const int opt = 1;
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	if(nb == -1)
	{
		printf("ERROR: setsockopt failt");
		return -1;
	}
	
	return 0;
}
