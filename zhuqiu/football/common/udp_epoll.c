/*************************************************************************
	> File Name: udp_epoll.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月04日 星期四 19时10分31秒
 ************************************************************************/

#include"./color.h"
#include"./head.h"
#include"./common.h"
#include"./udp_client.h"
#include"./udp_server.h"


void add_event(int epollfd, int fd, int events){
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    DBG(YELLOW "EPOLL" NONE " : After Epoll Add.\n");
}


void del_event(int epollfd, int fd, int events){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
    
}

int udp_connect(int epollfd, struct sockaddr_in *serveraddr){
    int sockfd;
    if((sockfd = socket_udp()) < 0){
        perror("socket_udp");
        return -1;
    }

    if((connect(sockfd, (struct sockaddr*)serveraddr, sizeof(struct sockaddr))) < 0){
        perror("connect");
        return -1;
    }
    DBG(GREEN "INFO" NONE " : After connect.\n");
    int ret = send(sockfd, "login!", sizeof("login!"), 0);
    DBG(RED "RET= %d\n" NONE, ret);
    add_event(epollfd, sockfd, EPOLLIN);
    return sockfd;
}

int udp_accept(int epollfd, int fd){
    struct sockaddr_in client;
    int new_fd, ret;
    char buff[512] = {0};
    socklen_t len = sizeof(struct sockaddr_in);
    ret = recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr *)&client, &len);
    if(ret < 0){
        return -1;
    }
        DBG(GREEN"INFO"NONE" buff = : %s \n", buff);
        DBG(GREEN"INFO"NONE" Login : %s : %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        new_fd = udp_connect(epollfd, &client);
    return new_fd;

}
