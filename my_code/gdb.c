#include<stdio.h>

int * px=NULL;
void foo(){
    printf("inside foo\n");
    *px=10;
}

int main(){
    printf("Hello,main!\n");
    foo();
    return 0;
}