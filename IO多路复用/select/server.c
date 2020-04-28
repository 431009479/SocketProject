/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月16日 星期四 15时53分21秒
 ************************************************************************/

#include "../common/color.h"
#include "../common/common.h"
#include "../common/tcp_server.h"
#include "../common/head.h"

#define CLIENTSIZE 100
#define BUFSIZE 4096

struct Buffer {
    int fd;
    char buff[BUFSIZE];
    int flag;
    int sendindex;
    int recvindex;
};

char ch_char(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 32;
    return c;
}

struct Buffer *AllocBuffer() {//函数指针
    struct Buffer *buffer = (struct Buffer *)malloc(sizeof(struct Buffer));
    if (buffer == NULL) 
        return NULL;
    buffer->fd = -1;
    buffer->flag = buffer->recvindex = buffer->sendindex = 0;
    memset(buffer->buff, 0, BUFSIZE);
    return buffer;
}

void FreeBuffer(struct Buffer *buffer) {
    free(buffer);
}

int RecvToBuffer(int fd, struct Buffer *buffer) {
    //char buff[BUFSIZE];
    int recv_num;
    while (1) {
        //先放到buff中
        char buff[BUFSIZE];
        recv_num = recv(fd, buff, sizeof(buff), 0);
        //printf("rec");
        if (recv_num <= 0) break;
         
        for (int i = 0; i < recv_num; i++) {
            //还有空间
            if (buffer->recvindex < sizeof(buffer->buff))
                //将buff中接收到的信息，变大写，存放到缓冲区Buffer->buff中
                buffer->buff[buffer->recvindex++] = ch_char(buff[i]);
            if (buffer->recvindex > 1 && buffer->buff[buffer->recvindex - 1] == '\r' && buffer->buff[buffer->recvindex - 2] == '\n'){ 
                buffer->flag = 1;
            }
        }
    }
    if (recv_num < 0) {
        if (errno = EAGAIN)//再试试，不算错 
            return 0;
        return -1;//出错
    } else if (recv_num == 0) {
        return 1;//关闭连接
    }
}

int SendFromBuffer(int fd, struct Buffer *buffer) {
    int send_num;
    while (buffer->sendindex < buffer->recvindex) {
        send_num = send(fd, buffer->buff + buffer->sendindex, buffer->recvindex - buffer->sendindex, 0);
        if (send_num < 0) {
            if (errno == EAGAIN) 
                return 0;
            buffer->fd = -1;
            return -1;
        }
        buffer->sendindex += send_num;
    }
    if (buffer->sendindex == buffer->recvindex) 
        buffer->sendindex = buffer->recvindex = 0;
    buffer->flag = 0;
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port!\n", argv[0]);
        exit(1);
    }
    int server_listen, fd, max_fd;
    
    if ((server_listen = socket_create(atoi(argv[1]))) < 0) {
        perror("socket_create");
        exit(1);
    }
    struct Buffer *buffer[CLIENTSIZE];
    
    for (int i = 0; i < CLIENTSIZE; i++) {
        buffer[i] = AllocBuffer();
    }

    make_nonblock(server_listen);

    fd_set rfds, wfds, efds;
    max_fd = server_listen;
     
    while (1) {
        FD_ZERO(&rfds);//清空集合
        FD_ZERO(&wfds);
        FD_ZERO(&efds);
      //  printf("abdsfb\n");
        FD_SET(server_listen, &rfds);//serverlisten放进去
        
        for (int i = 0; i < CLIENTSIZE; i++) {
            if (buffer[i]->fd == server_listen) continue;
            if (buffer[i]->fd > 0) {
                if (max_fd < buffer[i]->fd) max_fd = buffer[i]->fd;
                FD_SET(buffer[i]->fd, &rfds);
                if (buffer[i]->flag == 1) //俩\n情况
                    FD_SET(buffer[i]->fd, &wfds);
            }
        }
        if (select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0) {
            perror("select");
            return 1;
        }

        if (FD_ISSET(server_listen, &rfds)) {
            //server_listen可读，说明有人连接
            printf("Connect ready on serverlisten!\n");
            if ((fd = accept(server_listen, NULL, NULL)) < 0) {
                perror("accept");
                return 1;
            }
            if (fd > CLIENTSIZE) {//大于最大
                printf("Too many clients!\n");
                close(fd);
            } else {
                printf("Login!\n");
                make_nonblock(fd);
                if (buffer[fd]->fd == -1)//把当前fd给client[fd]
                    buffer[fd]->fd = fd;//数组下标为fd
            }
        }
        for (int i = 0; i <=  max_fd; i++) {
            int retval;
            if (i == server_listen) continue;
            if (FD_ISSET(i, &rfds)) {
                retval = RecvToBuffer(i, buffer[i]);
               // printf("asbdsfb\n");
            }
            if (retval == 0 && FD_ISSET(i, &wfds)) {
                retval = SendFromBuffer(i, buffer[i]);
            }
            if (retval) {
                printf("Logout !\n");
                buffer[i]->fd = -1;
                close(i);
            }
        }
    }  
    return 0;
}
