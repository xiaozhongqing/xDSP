
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
#include <netinet/ether.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>

#define MCAST_PORT      8888
#define IP_FOUND        "IP_FOUND"
#define IP_FOUND_ACK    "IP_FOUND_ACK"
#define IFNAME          "enp0s3"
#define BUFFER_LEN      32

int main(int argc, char** argv)  
{
    int ret = -1;
    int sock = -1;
    int so_broadcast =  1;
    struct ifreq ifr;
    struct sockaddr_in broadcast_addr; 
    struct sockaddr_in from_addr;
    int from_len;
    int count = -1;
    fd_set readfd;
    char  buff[BUFFER_LEN];
    struct timeval timeout;
    timeout.tv_sec = 2;  
    timeout.tv_usec = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        printf("HandleIPFound: socket init error\n");
        return -1;
    }
    strcpy(ifr.ifr_name, IFNAME);
    if(ioctl(sock, SIOCGIFBRDADDR, &ifr) == -1)
    {
        perror("ioctl error");
        exit(1);
    }  
    memcpy(&broadcast_addr, &ifr.ifr_broadaddr, sizeof(struct sockaddr_in ));
    broadcast_addr.sin_port = htons(MCAST_PORT);
    ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));
    int times = 10;
    int i = 0; 
    for(i = 0; i < times; i++)
    {
        ret = sendto(sock, IP_FOUND,strlen(IP_FOUND), 0,(struct sockaddr*)&broadcast_addr,sizeof(broadcast_addr));
        if(ret == -1)
        {
            continue;
        }
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
                count = recvfrom(sock, buff, BUFFER_LEN, 0, (struct sockaddr*)&from_addr, (socklen_t*)&from_len);
                printf("Recv msg is (%d)%s\n", count, buff);
                if(strstr(buff, IP_FOUND_ACK))
                {
                    printf("found server, IP is %s\n", inet_ntoa(from_addr.sin_addr));
                }
            }
        break;
        }
    }
    return 0;
}

