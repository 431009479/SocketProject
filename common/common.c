/*************************************************************************
	> File Name: common.c
	> Author: suyelu
	> Mail: suyelu@haizeix.com
	> Created Time: Sat 28 Mar 2020 08:41:00 PM CST
 ************************************************************************/
 #include"head.h"


void make_nonblock_ioctl(int fd){
    unsigned long ul = 1;
    ioctl(fd, FIONBIO,&ul);
}
void make_block_ioctl(int fd){
    unsigned long ul = 0;
    ioctl(fd, FIONBIO, &ul);
}


void make_nonblock(int fd){
    int flags;
    if(flags = fcntl(fd, F_GETFL, 0) < 0){
        perror("fcntl");
        return ;
    }
    flags |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flags) < 0){
        perror("fcntl");
        return ;
    }

}

void make_block(int fd){
    int flags;
    if(flags = fcntl(fd, F_GETFL, 0) < 0){
        perror("fcntl");
        return ;
    }
    flags &= ~O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flags) < 0){
        perror("fcntl");
        return ;
    }
}

