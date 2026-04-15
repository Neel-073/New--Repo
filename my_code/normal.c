#include<stdio.h>
#include<string.h>

int main()
{
    int new_len ,len;
    char str[50];

    printf("Enter string \n");
    scanf("%s",str);
    printf("your string = %s",str);
    len=strlen(str);

    if(len<new_len && new_len>0){
        str[new_len]='\0';
    }

 printf("your new string = %s",str);
}