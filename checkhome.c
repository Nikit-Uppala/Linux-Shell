#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "checkhome.h"
void checkhome(char home[],char* address,char ans[])
{
    int i=0,len=strlen(address),ctr=0;
    for(int i=0;i<len;i++)
    {
        if(address[i]!='~')
            ans[ctr++]=address[i];
        else
        {
            int length=strlen(home);
            if(i!=0)
                for(int j=1;j<length;j++)
                    ans[ctr++]=home[j];
            else
                for(int j=0;j<length;j++)
                    ans[ctr++]=home[j];
        }
    }
    ans[ctr]='\0';
}