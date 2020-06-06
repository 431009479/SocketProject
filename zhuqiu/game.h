/*************************************************************************
	> File Name: game.h
	> Author: 
	> Mail: 
	> Created Time: 2020年05月31日 星期日 16时25分08秒
 ************************************************************************/

#ifndef _GAME_H
#define _GAME_H
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include <inttypes.h>
#include<string.h>
#include<sys/time.h>
#include<stdlib.h>
#include<curses.h>
#include<time.h>
#define X 70
#define Y 20


struct point{
    int x, y;
};

struct point Bpoint; //箱子
struct point Opoint; //球
struct point Bdir, Odir; //箱子，球； 移动的位置
int cx = (X - 2) / 2 + 1; //球场的中间位置
int cy = (Y - 2) / 2 + 1; 

int dir_r[] = {-1, 0, 1}; //随机数组
int flag = 0; 
int score = 0; //得分

uint32_t get_random(uint32_t max){ //随机函数
    uint32_t value;
    srand((unsigned)time(NULL));
    value = rand() % (max + 1);
    return value;
}

void gotoxy(int x, int y){
    move(y, x);
}
    //控制鼠标位置的函数
void gotoxy_putc(int x, int y, char c){
    mvaddch(y, x, c);
    move(LINES - 1, 1);
    refresh();
}

void gotoxy_puts(int x, int y, char *c){
    mvaddstr(y, x, c);
    move(LINES - 1, 1);
    refresh();
}

//球场框架
void initfield(int x, int y){
    mvaddch(0, 0,'0');
    gotoxy_putc(x + 2, 0,'X');
    gotoxy_putc(0, y + 2,'Y');
    attron(COLOR_PAIR(1));
    for(int i = 1; i <= x; i++){
        gotoxy_putc(i, 1, '-');
        gotoxy_putc(i, y, '-');
    }
    for(int i = 1; i <= y; i++){
        gotoxy_putc(1, i, '|');
        gotoxy_putc(x, i, '|');
    }
    attroff(COLOR_PAIR(1));
    
    attron(COLOR_PAIR(2));
    gotoxy_puts(2, y + 3, "Message:");
    gotoxy_puts(x + 3, 2, "Help:");
    attroff(COLOR_PAIR(2));

}

void initgame(){
    if(!has_colors()||start_color() == ERR){
        endwin();
        fprintf(stderr, "终端不支持颜色！\n");
        exit(1);
    }
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_CYAN);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    initfield(X, Y);
    //初始化箱子位置
    Bpoint.x = cx - 3;
    Bpoint.y = Y - 3;
    //箱子的大小
    attron(COLOR_PAIR(3));
    for(int i = 0; i < 8; i++){
        gotoxy_putc(Bpoint.x + i, Bpoint.y, ' ');
        gotoxy_putc(Bpoint.x + i, Bpoint.y + 1, ' ');
    }
    attroff(COLOR_PAIR(3));
    refresh(); //刷新屏幕
}

void drawgame(){
    //球走之后变成空白
    gotoxy_putc(Opoint.x, Opoint.y, ' ');
    //球的移动
    if(!flag){
        Opoint.x = Bpoint.x + 4;
    }
    if(flag){
        Opoint.x += Odir.x;
        Opoint.y += Odir.y;
    }
    //判断右边界
    if(Opoint.x > X - 1) {
        Opoint.x = X - 1;
        Odir.x = -1;
        Odir.y = dir_r[get_random(2)];
    }
    //判断左边界
    if(Opoint.x <= 2){
        Opoint.x = 2;
        Odir.x = 1;
        Odir.y = dir_r[get_random(2)];
    }
    //判断上边界
    if(Opoint.y <= 2){
        Opoint.y = 2;
        Odir.y = 1;
        Odir.x = dir_r[get_random(2)];
    }
    
    //判断是否出界
    if(Opoint.y >= Y - 1){
        char info[1024] = {0};
        sprintf(info, "GamgOver! score %d\n", score);
        gotoxy_puts(3, Y + 4, info);
        flag = 0;
        score = 0;
        //出界后初始化球的位置
        Opoint.x = cx;
        Opoint.y = Y - 4;
        //出界后初始化箱子的位置
        for(int i = 0; i < 8; i++){
            gotoxy_putc(Bpoint.x + i, Bpoint.y, ' ');
            gotoxy_putc(Bpoint.x + i, Bpoint.y + 1, ' ');
        }
        Bpoint.x = cx - 3;
        Bpoint.y = Y - 3;
    }
        //球的位置加颜色
    attron(COLOR_PAIR(4));
    gotoxy_putc(Opoint.x, Opoint.y, 'O');
    attroff(COLOR_PAIR(4));
        //箱子的位置加颜色
    attron(COLOR_PAIR(2));
    for(int i = 0; i < 8; i++){
        gotoxy_putc(Bpoint.x + i, Bpoint.y, ' ');
        gotoxy_putc(Bpoint.x + i, Bpoint.y + 1, ' ');
    }
    attroff(COLOR_PAIR(2));
    //箱子的移动
    Bpoint.x += Bdir.x;
    Bpoint.y += Bdir.y;
    //清空箱子移动的方向
    memset(&Bdir, 0, sizeof(Bdir));
    //判断箱子是否出界
    if(Bpoint.x >= X - 8) Bpoint.x = X - 8;
    if(Bpoint.x <= 2) Bpoint.x = 2;
    //移动后箱子的位置
    attron(COLOR_PAIR(3));
    for(int i = 0; i < 8; i++){
        gotoxy_putc(Bpoint.x + i, Bpoint.y, ' ');
        gotoxy_putc(Bpoint.x + i, Bpoint.y + 1, ' ');
    }
    attroff(COLOR_PAIR(3));


    //球和箱子接触
    if(flag && Opoint.x <= Bpoint.x + 8 && Opoint.x >= Bpoint.x && Opoint.y >= Y - 4){
        score += 100;
        Odir.y = -1;
        Odir.x = dir_r[get_random(2)];
        char info[1024] = {0};
        sprintf(info, "You get %d, Total %d !", 100, score);
        gotoxy_puts(3, Y + 4, info);
    }
}

#endif
