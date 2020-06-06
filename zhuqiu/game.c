/*************************************************************************
	> File Name: game.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月31日 星期日 16时24分28秒
 ************************************************************************/

#include "game.h"

int main(){
    initscr(); //初始化屏幕
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    initgame();
    signal(14, drawgame);//信号量函数

    struct itimerval itimer; //计时器；
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 100000;
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 100;

    setitimer(ITIMER_REAL, &itimer, NULL); //可用来实现延时和定时的功能

    Opoint.x = cx;//球的初始化位置
    Opoint.y = Y - 4;
    attron(COLOR_PAIR(4));
    gotoxy_putc(Opoint.x, Opoint.y, 'O');//球加入屏幕
    attroff(COLOR_PAIR(4));
    refresh(); //刷新屏幕
    while(1){
        int c = getch();
        switch(c){
            case KEY_LEFT: //左键
                Bdir.x -= 2; break;       
            case KEY_RIGHT://右键
                Bdir.x += 2; break;
            case KEY_UP://下键
                if(flag) break; //关键值
                flag = 1;
                Odir.x = dir_r[get_random(2)];//随机化
                Odir.y = -1;
            default:
                break;
        }
    }
    //getch();
    endwin();
    return 0;
}

