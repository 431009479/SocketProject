/*************************************************************************
	> File Name: 2.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月31日 星期日 14时32分11秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<curses.h>

#define LEFTEDCE 10
#define RIGHTEDCE 70

int main(){
    char message[20] = "      ";
    char blank[20] = "      ";
    int col = 0;
    int dir = 1;
    int row = 0;
    int dirx = 1;
    int diry = 1;
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_RED);
    while(1){
        move(row, col); 
        attron(COLOR_PAIR(1));
        addstr(message);
        attroff(COLOR_PAIR(1));
        move(LINES - 1, COLS -1);
        refresh();
        usleep(10000);
        move(row, col);
        addstr(blank);
        col += diry;
        row += dirx;
        if(col >= RIGHTEDCE) diry--;
        if(col <= LEFTEDCE) diry++;
        if(row >= LINES - 1) dirx--;
        if(row <= 1) dirx++;
    }
    return 0;
}
