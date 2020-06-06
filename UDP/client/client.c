/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月31日 星期日 08时41分17秒
 ************************************************************************/

#include"../common/head.h"
#include"../common/common.h"
#include"../common/udp_client.h"
#include"../common/color.h"


int main(int argc, char **argv){
    char ip[512] = {0}, buff[512] = {0};
    int sockfd, port, n;
    if(argc != 3){
        fprintf(stderr, "Usage: %s ip port!\n", argv[0]);
        return 1;
    }

    struct sockaddr_in server;
    port = atoi(argv[2]);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(argv[1]);
    
    socklen_t len =  sizeof(server);
    if((sockfd = socket_udp()) < 0){
        perror("socket_udp");
        exit(1);
    }


    while(1){
        fgets(buff, sizeof(buff), stdin);
        if(n = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&server, len) < 0){
                perror("sendto");
        }
        if(n = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&server , &len) < 0){
            perror("recvfrom");
        }
            printf("buff: %s", buff);  

    }

    return 0;
}

