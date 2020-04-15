/*************************************************************************
	> File Name: server.c
	> Author: suyelu
	> Mail: suyelu@haizeix.com
	> Created Time: 六  3/28 14:38:37 2020
 ************************************************************************/

#include "head.h"
#include "tcp_server.h"
#include "common.h"

#define MAXCLIENT 150
#define POLL 10


void ch_char(char c) {
        if (c >= 'a' && c <= 'z') {
            return c -= 32;
        return c;
}

int main(int argc, char **argv) {
    int port, server_listen;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port!\n", argv[0]);
        return 1;
    }
    port = atoi(argv[1]);

    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        return 2;
    }
    struct pollfd event_set[POLL];
    
    for(int i = 0; i < POLL; i++){
        event_set[i].fd = -1;
    }
    event_set[0].fd = server_listen;
    event_set[0].events = POLLIN;

    while(1){
        int retval;
        if((retval = poll(event_set, POLL, -1)) < 0){
            return 1;
        }
        if(event_set[0].revents & POLLIN){
            if ((fd = accept(server_listen, NULL, NULL)) < 0) {
                perror("accept");
                continue;
            }
            retval--;
            int i;
            for(i = 1; i < POLL; i++){
                if(event_set[i].fd < 0){
                    event_set[i].fd = fd;
                    event_set[i].events = POLLIN:
                    break;
                }
            }
            if(i == POLL){
                printf("Too many clients!\n");
            }
        }
        for(int i = 0; i < POLL; i++){
            if(event_set[i].fd < 0) continue;

            if(event_set[i].revents & (POLLIN | POLLHUP | POLLERR)){
                retval--;
                char buff[512];
                if(recv(event_set[i].fd, buff, sizeof(buff), 0) > 0){
                    printf("Recv : %s \n", buff);
                    for(int i = 0; i < strlen(buff); i++){
                        buff[i] = ch_char(buff[i]);
                    }
                    send(event_set[i].fd, buff, strlen(buff), 0);
                }else{
                    close(event_set[i].fd);
                    event_set[i].fd = -1;
                }
            }
            if(retval <= 0) break;
        }
    }


    return 0;
}   

