/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月30日 星期六 23时15分14秒
 ************************************************************************/

#include "../common/common.h"
#include"../common/head.h"

#include"../common/udp_server.h"

int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr, "Usage : %s port\n", argv[0]);
        exit(0);
    }
    int port, sockfd;
    struct sockaddr_in client;
    socklen_t addr_len = sizeof(client);
    port = atoi(argv[1]);
    if((sockfd = socket_create_udp(port)) < 0){
        perror("udp_create");
        exit(1);
    }
    memset(&client, 0, sizeof(struct sockaddr_in));
    while(1){
        char buff[512] = {0};
        if((recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&client, &addr_len)) > 0){
            printf("[%s] : %s\n", inet_ntoa(client.sin_addr), buff);
        }
        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&client, addr_len);
    }
    return 0;
}

