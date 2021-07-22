#include<string.h>
#include<stdlib.h>
#include "strip.h"
void strip(char* str)
{
    char* ans;
    if(str[0]=='\0')
        return;
    int start=0,len = strlen(str),end = len-1;
    ans = (char*)malloc(len*sizeof(char));
    while(start<len && (str[start]==' ' || str[start]=='\t'))
        start++;
    while(end>=0 && (str[end]==' ' || str[end]=='\t'))
        end--;
    str[end+1]='\0';
    strcpy(ans,&str[start]);
}