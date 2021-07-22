#include "headers.h"
LL* processes = NULL;
char historyPath[1501];
queue history;
int currentFG=-1,bgctr=0,inHistory,exitCode;
char* username;
char home[1001],present[1001],hostname[1001],prevDir[1001],processName[1001];
void sigchld_handler(int signum)
{
    signal(SIGCHLD,sigchld_handler);
    int statusExit,id,check=0;
    while((id=waitpid(-1,&statusExit,WNOHANG))>0)
    {
        LL* node = get(processes,id,-1);
        if(node!=NULL)
        {
            check=1;
            if(WIFEXITED(statusExit))
                fprintf(stderr,"\n%s with pid %d exited normally\n",node->name,node->pid);
            else
                fprintf(stderr,"\n%s with pid %d exited abruptly\n",node->name,node->pid);
            if(node->jobNumber==bgctr)
                bgctr--;
            processes = pop(processes,node);
        }
    }
    if(check==1)
        prompt(username,hostname,home,present,stderr,0);
    return;
}
void giveCommand(char* str,char command[],char* delim,int *flag,int* cond)
{
    int i=0,len=strlen(str);
    while(i<len && str[i]!=delim[0] && str[i]!=delim[1])
    {
        command[i]=str[i];
        i++;
    }
    if(str[i]=='&')
        *cond=1,command[i]='&',command[i+1]='\0';
    else
        *cond=0,command[i]='\0'; 
    if(i==len)
        *flag=1;
}
void redirectSpace(char comm[],char result[])
{
    int len = strlen(comm),start=0;
    for(int i=0;i<len;i++)
    {
        if(i<len-1 && comm[i]=='>' && comm[i+1]=='>')
        {
            result[start++]=' ';
            result[start++]='>';result[start++]='>';
            result[start++]=' ';i++;
        }
        else if(comm[i]=='>' || comm[i]=='<' || comm[i]=='|')
        {
            result[start++]=' ';
            result[start++]=comm[i];
            result[start++]=' ';
        }
        else
            result[start++]=comm[i];
    }
    result[start]='\0';
}
void sigint_handler(int signalnumber)
{
    signal(SIGINT,sigint_handler);
    if(currentFG!=-1)
    {
        kill(currentFG,SIGINT);
        exitCode=1;
    }
    else if(currentFG==-1)
        printf("\n"),prompt(username,hostname,home,present,stderr,-1);
}
void sigtstp_handler(int signum)
{
    exitCode=-1;
    signal(SIGTSTP,sigtstp_handler);
    if(currentFG!=-1)
    {
        setpgid(currentFG,0);
        processes = push(processes,currentFG,processName,++bgctr);
        kill(currentFG,SIGTSTP);
        printf("[%d]\tStopped\t%s [%d]\n",bgctr,processName,currentFG);
        exitCode = 1;
    }
    else
        printf("\n"),prompt(username,hostname,home,present,stderr,exitCode);
}
int checkPipes(char command[])
{
    int len = strlen(command),count=0;
    for(int i=0;i<len;i++)
        if(command[i]=='|')
            count++;
    return count;
}
void resetRedirections(int inB,int outB,int dup_in,int dup_out)
{
    if(dup_in!=-1) close(dup_in);
    if(dup_out!=-1) close(dup_out);
    dup2(inB,STDIN_FILENO);
    dup2(outB,STDOUT_FILENO);
}
int setRedirections(char inpFile[],char outFile[],int append,int* dup_in,int* dup_out)
{
    if(inpFile[0]!='\0')
    {
        int fdin = open(inpFile,O_RDONLY);
        if(fdin==-1)
        {
            perror(inpFile);
            exitCode=1;
            return 1;
        }
        *dup_in = dup2(fdin,STDIN_FILENO);
        close(fdin);
        if(*dup_in==-1)
        {
            perror("dup2");
            exitCode=1;
            return 1;
        }
    }
    if(outFile[0]!='\0')
    {
        int fdout;
        if(append)
        {
            fdout = open(outFile,O_WRONLY | O_CREAT,0644);
            if(fdout!=-1)
                lseek(fdout,0,SEEK_END);
        }
        else
            fdout = open(outFile,O_WRONLY | O_TRUNC | O_CREAT,0644);
        if(fdout==-1)
        {
            perror(outFile);
            exitCode=1;
            return 1;
        }
        *dup_out = dup2(fdout,STDOUT_FILENO);
        close(fdout);
        if(*dup_out==-1)
        {
            perror("dup2");
            exitCode=1;
            return 1;
        }
    }
    return 0;
}
void executeCommand(char* command[],int argCount,int bg)
{
    if(strcmp(command[0],"pwd")==0)
        printf("%s\n",present);
    else if(strcmp(command[0],"cd")==0)
        executeCD(command,home,present,prevDir,argCount,&exitCode);
    else if(strcmp(command[0],"echo")==0)
        executeEcho(command,argCount,&exitCode);
    else if(strcmp(command[0],"ls")==0)
        ls(command,argCount,present,home,&exitCode);
    else if(strcmp(command[0],"pinfo")==0)
        pinfo(command,argCount,&exitCode);
    else if(strcmp(command[0],"history")==0)
        executeHistory(command,argCount,history,inHistory,&exitCode);
    else if(strcmp(command[0],"setenv")==0)
        setEnv(command,argCount,&exitCode);
    else if(strcmp(command[0],"unsetenv")==0)
        unsetEnv(command,argCount,&exitCode);
    else if(strcmp(command[0],"jobs") == 0)
        jobs(processes,argCount,&exitCode);
    else if(strcmp(command[0],"kjob")==0)
        kjob(command,argCount,processes,&exitCode);
    else if(strcmp(command[0],"fg")==0)
        fg(command,argCount,&processes,&bgctr,&currentFG,processName,&exitCode);
    else if(strcmp(command[0],"bg")==0)
        execute_bg(command,argCount,processes);
    else if(strcmp(command[0],"overkill")==0)
        overkill(&processes,&exitCode);
    else if(strcmp(command[0],"quit")==0)
    {
        updateHistory(historyPath,history);
        exit(0);
    }
    else
        executeProg(command,argCount,bg,&bgctr,&processes,&currentFG,processName,&exitCode);
}
void executePipes(char currCommand[],int count,int inB,int outB)
{
    int pipes[2],backup=-1;
    char* saveptr = currCommand;
    char* token;
    char* command[1001];
    char* temp[1001];
    int argCount=0,append=0;
    char inpFile[1001],outFile[1001];
    int dup_in,dup_out,ctr=0,commandsNum=count+1;
    for(token = strtok_r(saveptr,"|",&saveptr);token!=NULL;token = strtok_r(saveptr,"|",&saveptr))
    {
        pipe(pipes);
        if(ctr==0)
            dup2(pipes[1],STDOUT_FILENO);
        else if(ctr==commandsNum-1)
        {
            close(pipes[1]);
            dup2(backup,STDIN_FILENO);
            dup2(outB,STDOUT_FILENO);
            close(backup);
        }
        else
        {
            dup2(backup,STDIN_FILENO);
            dup2(pipes[1],STDOUT_FILENO);
            close(backup);
        }
        dup_in=-1,dup_out=-1,argCount=0,inpFile[0]='\0',outFile[0]='\0',append=0;
        parse(token," \t",temp,&argCount);
        checkRedirection(temp,&argCount,inpFile,outFile,&append,command);
        setRedirections(inpFile,outFile,append,&dup_in,&dup_out);
        if(argCount>0)
        {
            if(strcmp(command[0],"cd")==0)
            {
                backup = dup(pipes[0]);
                close(pipes[0]),close(pipes[1]);
                continue;
            }
            executeCommand(command,argCount,0);
        }
        resetRedirections(inB,outB,dup_in,dup_out);
        backup = dup(pipes[0]);
        if(pipes[0]!=-1)
            close(pipes[0]);
        if(pipes[1]!=-1)
            close(pipes[1]);
        ctr++;
        for(int i=0;i<argCount;i++)
        {
            if(command[i]!=NULL)
                free(command[i]);
            command[i]=NULL;
        }
    }
}
int main()
{
    exitCode = -1;
    signal(SIGINT,sigint_handler);
    signal(SIGTSTP,sigtstp_handler);
    signal(SIGCHLD,sigchld_handler);
    FILE* historyFile;
    history.front = NULL;
    history.back = NULL;
    inHistory=0;
    printf("\e[1;1H\e[2J");
    size_t buffSize=1001;
    getcwd(home,buffSize);
    sprintf(historyPath,"%s/history.txt",home);
    historyFile = fopen(historyPath,"a");
    if(historyFile!=NULL)
        fclose(historyFile);
    historyFile = fopen(historyPath,"r");
    char temp[1001];
    while(fscanf(historyFile,"%[^\n]%*c",temp)==1)
    {
        history = enQueue(history,temp);
        inHistory++;
    }
    fclose(historyFile);
    strcpy(present,home);
    gethostname(hostname,buffSize);
    username=getenv("USER");
    char *commands=(char*)malloc(buffSize*sizeof(char));
    char currCommand[1001],commandTemp[1001];
    int flag,total,argCount=0,bg=0,id,cond=0,append=0,dup_in,dup_out,inB = dup(STDIN_FILENO),outB=dup(STDOUT_FILENO);
    char* command[1001]={NULL};
    char* finalCommand[1001]={NULL};
    char inpFile[1001],outFile[1001];
    strcpy(prevDir,"\0");
    while(1)
    {
        prompt(username,hostname,home,present,stdout,exitCode);
        if(getline(&commands,&buffSize,stdin)==-1)
        {
            updateHistory(historyPath,history);
            printf("quit\n");
            return 0;
        }
        commands[strlen(commands)-1]='\0';
        strip(commands);
        if(history.back==NULL || history.back!=NULL && strcmp(commands,history.back->command)!=0)
        {
            if(inHistory==20)
                history = deQueue(history),inHistory--;
            history = enQueue(history,commands),inHistory++;
        }
        flag=0;
        total=0;
        while(!flag)
        {
            exitCode = 0;
            bg=0,append=0,argCount=0;
            strcpy(inpFile,"\0"),strcpy(outFile,"\0");
            giveCommand(&commands[total],commandTemp,";&",&flag,&cond);
            redirectSpace(commandTemp,currCommand);
            int noPipes = checkPipes(currCommand);
            if(!cond)
                total+=strlen(commandTemp)+1;
            else
                total+=strlen(commandTemp);
            if(noPipes>0)
            {
                executePipes(currCommand,noPipes,inB,outB);
                continue;
            }
            parse(currCommand," \t",finalCommand,&argCount);
            checkRedirection(finalCommand,&argCount,inpFile,outFile,&append,command);
            if(setRedirections(inpFile,outFile,append,&dup_in,&dup_out))
                continue;
            if(argCount>0)
            {
                if(strcmp(command[argCount-1],"&")==0)
                {
                    bg=1;
                    free(command[argCount-1]);
                    command[argCount-1]=NULL;
                    argCount--;
                }
                else if(command[argCount-1][strlen(command[argCount-1])-1]=='&')
                {
                    bg=1;
                    command[argCount-1][strlen(command[argCount-1])-1]='\0';
                }
            }
            if(argCount>0)
                executeCommand(command,argCount,bg);
            resetRedirections(inB,outB,dup_in,dup_out);
            for(int i=0;i<argCount;i++)
                if(command[i]!=NULL)
                    free(command[i]),command[i]=NULL;
        }
    }
    close(inB);
    close(outB);
    free(commands);
    return 0;
}