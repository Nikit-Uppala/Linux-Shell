#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include "ls.h"
#include "strip.h"
#include "checkhome.h"
void printPermissions(char n)
{
    switch(n)
    {
        case '0' : printf("---");break;
        case '1' : printf("--x");break;
        case '2' : printf("-w-");break;
        case '3' : printf("-rx");break;
        case '4' : printf("r--");break;
        case '5' : printf("r-x");break;
        case '6' : printf("rw-");break;
        case '7' : printf("rwx");break;
    }
}
void displayStats(char* path,char* file,int* exitCode)
{
    char address[1001];
    if(file[0]!='\0')
        sprintf(address,"%s/%s",path,file);
    else
        strcpy(address,path);
    struct stat File;
    if(stat(address,&File)==-1)
    {
        perror("ls");
        *exitCode=1;
        return;
    }
    char permissions[10];
    sprintf(permissions,"%o",File.st_mode);
    int len = strlen(permissions);
    if(File.st_mode & S_IFDIR)
        printf("d");
    else if(File.st_mode & S_IFREG)
        printf("-");
    else if(File.st_mode & S_IFLNK)
        printf("l");
    else if(File.st_mode & S_IFSOCK)
        printf("s");
    for(int i=len-3;i<len;i++)
        printPermissions(permissions[i]);
    printf(" %3ld ",File.st_nlink);
    struct passwd* user = getpwuid(File.st_uid);
    struct group* grp = getgrgid(File.st_gid);
    printf("%s %s %10ld ",user->pw_name,grp->gr_name,File.st_size);
    char modTime[101];
    time_t t= File.st_mtime;
    struct tm* local;
    local = localtime(&t);
    strftime(modTime,sizeof(modTime),"%m %d %H:%M",local);
    int index=0;
    for(char* token = strtok(modTime," ");token;token = strtok(NULL," "))
    {
        if(index==0)
        {
            if(token[0]=='1')
            {
                if(token[1]=='0')
                    printf("Oct ");
                else if(token[1]=='1')
                    printf("Nov ");
                else
                    printf("Dec ");
            }
            else
            {
                switch(token[1])
                {
                    case '1':printf("Jan ");break;
                    case '2':printf("Feb ");break;
                    case '3':printf("Mar ");break;
                    case '4':printf("Apr ");break;
                    case '5':printf("May ");break;
                    case '6':printf("Jun ");break;
                    case '7':printf("Jul ");break;
                    case '8':printf("Aug ");break;
                    case '9':printf("Sep ");break;
                }
            }
            index++;
        }
        else if(index==1)
        {
            index++;
            if(token[0]=='0')
                printf("%2c ",token[1]);
            else
                printf("%2s ",token);
        } 
        else
            printf("%s ",token);
    }
}
void ls(char** command,int argc,char pwd[],char home[],int* exitCode)
{
    int a=0,l=0;
    DIR* dir;
    struct dirent* d;
    int dirNo=0;
    for(int i=1;i<argc;i++)
    {
        if(command[i][0]=='-')
        {
            int length=strlen(command[i]);
            for(int j=1;j<length;j++)
                switch (command[i][j])
                {
                case 'a':a=1;break;
                case 'l':l=1;break;
                default:
                    fprintf(stderr,"ls: invalid option - %c\n",command[i][j]);
                    *exitCode=1;
                    return;
                }
        }
        else
            dirNo++;
    }
    if(dirNo==0)
    {
        if((dir = opendir(pwd))==NULL)
        {
            perror("ls");
            *exitCode=1;
            return;
        }
        while(d=readdir(dir))
        {
            if(a==1 || a==0 && d->d_name[0]!='.')
            {
                if(l==1)
                    displayStats(pwd,d->d_name,exitCode);
                printf("%s\n",d->d_name);
            }
        }
        closedir(dir);
    }
    else
    {
        for(int i=1;i<argc;i++)
        {
            if(command[i][0]!='-')
            {
                dirNo--;
                char path[1001],errorMsg[2001];
                checkhome(home,command[i],path);
                if((dir=opendir(path))==NULL)
                {
                    struct stat file;
                    if(stat(path,&file)==-1)
                    {
                        sprintf(errorMsg,"ls: cannot access %s",path);
                        perror(errorMsg);
                        *exitCode=1;
                        continue;
                    }
                    else
                    {
                        if(l==1)
                            displayStats(path,"\0",exitCode);
                        printf("%s\n",path);
                    }
                }
                else
                {
                    printf("%s:\n",command[i]);
                    while(d=readdir(dir))
                    {
                        if(a==1 || a==0 && d->d_name[0]!='.')
                        {
                            if(l==1)
                                displayStats(path,d->d_name,exitCode);
                            printf("%s\n",d->d_name);
                        }
                    }
                    if(dirNo)
                        printf("\n");
                    if(dir!=NULL)
                        closedir(dir);
                }
            }
        }
    }
}