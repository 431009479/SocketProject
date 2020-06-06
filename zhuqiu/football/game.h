/*************************************************************************
	> File Name: game.h
	> Author: 
	> Mail: 
	> Created Time: 2020年06月02日 星期二 18时35分37秒
 ************************************************************************/

#ifndef _GAME_H
#define _GAME_H
#include<curses.h>
#define MAX 50

struct LogData{
    char name[20];
    int team;//0 RED 1 BLUE
};

struct Point{
    int x, y;
};

struct User{
    int team; //0  1
    char name[20];
    int flag; //三次没有响应
    struct sockaddr_in addr;//记录用户远程地址
    struct Point loc;//目前所在位置
};
struct Map {
    int width;
    int height;
    struct Point start;//区域大小
    int gate_width;//球门
    int gate_height;
};

struct Ctrl{
    int carry;//带球
    int kick;//踢球
};

struct TransMsg{//客户端只负责控制，服务端负责计算
    int dirx, diry;//方向
    struct Ctrl ctrl;
};

struct Map court; 

WINDOW *Football, *Help,*Message, *Score, *Write;

WINDOW *create_newwin(int width, int height, int startx, int starty){
    WINDOW *win;
    win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

void destroy_win(WINDOW *win){
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');//用来把box画出来的线删掉
    wrefresh(win);
    delwin(win);
}

void gotoxy(int x, int y){
    move(y, x);
}

void gotoxy_putc(int x, int y, char c){
    move(y, x);
    addch(c);
    move(LINES - 1, 1);
    refresh();
}

void gotoxy_puts(int x, int y, char *s){
    move(y, x);
    addstr(s);
    move(LINES - 1, 1);
    refresh();
}

void w_gotoxy_putc(WINDOW *win, int x, int y, char c){
    mvwaddch(win, y, x, c);
    move(LINES - 1, 1);
    wrefresh(win);
}

void w_gotoxy_puts(WINDOW *win, int x, int y, char *s){
    mvwprintw(win, y, x, s);
    move(LINES - 1, 1);
    wrefresh(win);
}


void initfootball(){
    initscr();
    clear();
    if(!has_colors() || start_color() == ERR){
        endwin();
        fprintf(stderr, "终端不支持颜色！\n");
        exit(1);
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_CYAN);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    Football=create_newwin(court.width, court.height, court.start.x, court.start.y);
    Message=create_newwin(court.width, 5, court.start.x, court.start.y + court.height);
    Help=create_newwin(20, court.height, court.start.x + court.width, court.start.y);
    Score=create_newwin(20, 5, court.start.x + court.width, court.start.y + court.height);
    Write=create_newwin(court.width + 20, 5, court.start.x, court.start.y + court.height + 5);
}
void *draw(void *arg){
    initfootball();
    while(1){
        sleep(10);
    }
}

#endif
