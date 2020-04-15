/*************************************************************************
	> File Name: tcp_client.c
	> Author:fangsong
	> Mail: 
	> Created Time: 2020年03月29日 星期日 22时19分28秒
 ************************************************************************/

#include "head.h"
#include "common.h"

int socket_connect(char *host, int port) {
    int sockfd;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //套接字完成两个程序之间数据传输
        perror("socket_connect");
        return -1;
    }
    printf("Socket creat.\n");
    if(connect(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("connect");
        return -1;
    }
    return sockfd;
}

int socket_connect_timeout(char *host, int port, long timeout) {
    int sockfd, select_num, connect_val;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    make_nonblock(sockfd);//设置非阻塞

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;

    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        int retval, error = -1;
        int len = sizeof(int);
        retval = select(sockfd + 1, NULL, &wfds, NULL, &tv);
        if (retval < 0) {
            close(sockfd);
            return -1;
        } else if (retval) {
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len) < 0) {
                //取错误值判断
                close(sockfd);
                return -1;
            }
            if (error) {
                close(sockfd);
                return -1;
            }
        } else {
            //timeout
            close(sockfd);
            return -1;
        }
    }
    make_block(sockfd);
    return sockfd;
}


/*完成两个应用程序之间的数据传输;
 *网络套接字是:IP地址与端口的组合
 *connect建立与指定socket的链接
 *send是向已经连接的socket发送数据
 * */
