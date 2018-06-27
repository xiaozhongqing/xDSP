#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <netinet/ip.h>


#define IP_FOUND        "IP_FOUND"
#define IP_FOUND_ACK    "IP_FOUND_ACK"
#define MCAST_PORT      8888
#define BUFFER_LEN      32

int main(int argc, char** argv)
{
   
    int ret = -1;
    int sock = -1;
    struct sockaddr_in local_addr;
    struct sockaddr_in from_addr;
    int from_len;
    fd_set readfd;
    char buff[BUFFER_LEN];
    struct timeval timeout;
    timeout.tv_sec = 2; 
    timeout.tv_usec = 0;
    printf("ip founded start....\n");
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if( sock < 0 )
    {
        printf("HandleIPFound: socket init error\n");
        return -1;
    }

    memset((void*)&local_addr, 0, sizeof(struct sockaddr_in));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(MCAST_PORT); 

    ret = bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr));
    if(ret != 0)
    {
        printf("HandleIPFound:bind error\n");
        return -1;
    }
    while(1)
    {
        FD_ZERO(&readfd);
        FD_SET(sock, &readfd);
        ret = select(sock+1, &readfd, NULL, NULL, &timeout);
        switch(ret)
        {
        case -1:
            break;
        case 0:
            break;
        default:
            if(FD_ISSET(sock, &readfd))
            {
                int count = (int)recvfrom(sock, buff, BUFFER_LEN, 0, (struct sockaddr*) &from_addr, (socklen_t*)&from_len);
                printf("Recv msg is (%d)%s\n", count, buff);
                if(strstr(buff, IP_FOUND))
                {
                    memcpy(buff, IP_FOUND_ACK, strlen(IP_FOUND_ACK)+1);
                    count = sendto(sock, buff, strlen(buff), 0, (struct sockaddr*) &from_addr, from_len);
                }
            }
        }
    }
    printf("ip founded end....\n");
    return 0;
}

