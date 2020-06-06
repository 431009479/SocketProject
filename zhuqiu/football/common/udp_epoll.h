/*************************************************************************
	> File Name: udp_epoll.h
	> Author: 
	> Mail: 
	> Created Time: 2020年06月04日 星期四 19时24分42秒
 ************************************************************************/

#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
void add_event(int epollfd, int fd, int events);
void del_event(int epollfd, int fd, int events);
void udp_connet(int epollfd, struct sockaddr_in *serveraddr);
void udp_accept(int epollfd, int fd);
#endif
