#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    struct sockaddr_in addrto;  
    bzero(&addrto, sizeof(struct sockaddr_in));  
    addrto.sin_family = AF_INET;  
    addrto.sin_addr.s_addr = htonl(INADDR_ANY);  
    addrto.sin_port = htons(6000);  

    struct sockaddr_in from;  
    bzero(&from, sizeof(struct sockaddr_in));  
    from.sin_family = AF_INET;  
    from.sin_addr.s_addr = htonl(INADDR_ANY);  
    from.sin_port = htons(6000);  
        
    int sock = -1;  
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
    {     
        printf("ERROR: socket failed\n");
        return -1;  
    }     
    const int opt = 1;   
    int nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
    if(nb == -1)  
    {  
        printf("ERROR: set socket error...\n");  
        return -1;  
    }  

    if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1)   
    {     
        printf("ERROR: bind error...\n");  
        return -1;  
    }  

    int len = sizeof(struct sockaddr_in);  
    char smsg[100] = {0};
    printf("INFO: sock:%d start....\n", sock);
    while(1)  
    {  
        int ret = recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&from, (socklen_t*)&len);  
        if(ret <= 0)  
        {  
            printf("ERROR: (%d)read error....\n", sock);    
        }  
        else 
        {         
            printf("%s\t", smsg);     
        }  
        sleep(1);  
    }  

    return 0;
}



