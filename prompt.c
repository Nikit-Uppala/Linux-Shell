#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include "prompt.h"
int checkHome(char home[],char path[],int len1,int len2)
{
    if(len2<len1)
        return 0;
    else
    {
        for(int i=0;i<len1;i++)
            if(path[i]!=home[i])
                return 0;
    }
    return 1;
}
void prompt(char* user,char host[],char home[],char path[],FILE* stream,int exitCode)
{
    int len1=strlen(home),len2=strlen(path);
    char success[] = ":')";
    char error[] = ":'(";
    fprintf(stream,"%s\033[0;32m<%s@%s:",(exitCode==0?success:(exitCode==1?error:"")),user,host);
    fprintf(stream,"\033[0;34m");
    if(checkHome(home,path,len1,len2))
        fprintf(stream,"~%s>",&path[len1]);
    else
        fprintf(stream,"%s>",path);
    fprintf(stream,"\033[0m");
}