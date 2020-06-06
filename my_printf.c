/*************************************************************************
	> File Name: my_printf.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月01日 星期一 21时39分29秒
 ************************************************************************/

#include<stdio.h>
#include<stdarg.h>
#define PUTC(a) putchar(a), ++cnt

int reverse_num(int num, int *temp){
    int digit = 0;
    do{
        *temp = *temp * 10 + num % 10;
        num /= 10;
        digit += 1;
    }while(num);
    return digit;
}

int output_num(int temp, int digit){
    int cnt = 0;
    while(digit--){
        putchar(temp % 10 + '0');
        temp /=10;
        cnt ++;
    }
    return cnt;
}

int my_printf(const char *frm, ...){
    va_list arg;
    int cnt = 0;
    va_start(arg, frm);
    for(int i = 0; frm[i]; i++){
        switch(frm[i]){
            case '%':{
                switch (frm[++i]){

                    case 'd':{
                        int x = va_arg(arg, int);
                        if(x < 0) PUTC('-'), x = -x;
                        int num1 = x / 100000, num2 = x % 100000;
                        int temp1 = 0, temp2 = 0;
                        int digit1 = reverse_num(num1, &temp1);
                        int digit2 = reverse_num(num2, &temp2);
                        if(num1)digit2 = 5;
                        else digit1 = 0;
                        cnt += output_num(temp1, digit1);
                        cnt += output_num(temp2, digit2);
                    }break;

                    case '%':{
                        PUTC(frm[i]);
                    }break;

                    case 's': {
                        const char *str = va_arg(arg, const char *);
                        for (int i = 0; str[i]; i++) {
                            PUTC(str[i]);
                        }
                    } break;

                }
            }break;
            default:PUTC(frm[i]);break;
        }
    }
    va_end(arg);
    return cnt;

}

int main(){
    int a;
    scanf("%d", &a);
    printf("hello world\n");
    my_printf("hello world\n");
    printf("(a) = %d\n", a);
    my_printf("(a) = %d\n", a);
    char str[100] = "I love Bei Jing";
    printf("str[100] = %s\n", str);
    my_printf("str[100] = %s\n", str);
    return 0;
}
