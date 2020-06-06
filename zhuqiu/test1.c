/*************************************************************************
	> File Name: test1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月31日 星期日 14时07分53秒
 ************************************************************************/

#include<stdio.h>
#include<curses.h>
int main(){
    initscr();
    clear();
    move(10, 20);
    addstr("Hello");
    refresh();
    getchar();
    endwin();
    return 0;

}

