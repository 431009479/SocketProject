/*************************************************************************
	> File Name: udp_server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月28日 星期四 19时56分06秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>


int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(1);
    }
    int fd, port = atoi(argv[1]), n = 0;
    char buff[512];
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server, clien;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(fd, (struct sockaddr*)&server, sizeof(server)) <0){
        perror("bind");
        exit(1);
    }

    while(1){
        socklen_t clien_len = sizeof(clien);
        if((n = recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr*)&clien, &clien_len )) == -1){
            perror("recvfrom");
        }

        for(int i = 0; i < n; i++){
            printf("%c", buff[i]);
            buff[i] = toupper(buff[i]);
        }
        if((n = sendto(fd, buff, n, 0, (struct sockaddr*)&clien, clien_len )) == -1){
        
            perror("sendto");
        }

    }
    close(fd);
    return 0;
}

