#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ptrace.h>
#include<string.h>
#include "pinfo.h"
void pinfo(char* command[],int num,int* exitCode)
{
    char filePath[1001];
    int pid,buffSize = 1001;
    char buffer[1001];
    char status;
    long long int mSize;
    if(num>1)
    {

        for(int i=1;i<num;i++)
        {
            sprintf(filePath,"/proc/%s/stat",command[i]);
            FILE* fd1 = fopen(filePath,"r");
            if(!fd1)
            {
                fprintf(stderr,"pinfo : invalid pid %s\n",command[i]);
                *exitCode=1;
                continue;
            }
            fscanf(fd1,"%d%s %c",&pid,buffer,&status);
            for(int i=4;i<23;i++)
                fscanf(fd1,"%s",filePath);
            fscanf(fd1,"%lld",&mSize);
            fclose(fd1);
            sprintf(filePath,"/proc/%s/exe",command[i]);
            int bsize=readlink(filePath,buffer,buffSize);
            buffer[bsize] = '\0';
            printf("pid -- %s\n",command[i]);
            printf("Process Status -- %c\n",status);
            printf("memory -- %lld {Virtual Memory}\n",mSize);
            printf("Executable Path -- %s\n",buffer);
            if(i!=num-1)
                printf("\n");
        }
    }
    else
    {
        pid = getpid();
        sprintf(filePath,"/proc/%d/stat",pid);
        FILE* fd = fopen(filePath,"r");
        int p;
        if(fd!=NULL)
        {   
            fscanf(fd,"%d%s %c",&p,buffer,&status);
            for(int i=4;i<23;i++)
                fscanf(fd,"%s",filePath);
            fscanf(fd,"%lld",&mSize);
            fclose(fd);
        }
        sprintf(filePath,"/proc/%d/exe",pid);
        int bsize = readlink(filePath,buffer,buffSize);
        buffer[bsize] = '\0';
        printf("pid -- %d\n",pid);
        printf("Process Status -- %c\n",status);
        printf("memory -- %lld {Virtual Memory}\n",mSize);
        printf("Executable Path -- %s\n",buffer);
    }
}