Compiling the code:
Run make command and main executable is created.

main.c modifications from assignment 2:
Added signal handlers for CTRL + C and CTRL + Z and also for detecting SIGCHLD.
Also added a functions redirectSpace(), setRedirections(), resetRedirections().

sigint_handler(): Handles the ctrl+c signal. If there is any foreground process, it will exit and the return will be returned to the shell. Id there is not foreground process running then it prints a prompt.

sigtstp_handler() : Handles the ctrl+z signal. It sends the active foreground process to the background(it would be stopped) and control will be returned to the shell.

sigchld_handler(): Handles the case where a background process exits. It prints the appropriate message about the process exited.

redirectSpace(): It adds space before and after '>', '<', ">>" and '|' in the command.(Becomes easy to parse.)

checkPipes(): checks whether the given command has pipes.

executePipes(): It is executes the commands which contain pipes.

executeCommands(): It checks which command is given and calls the appropriate fucntions.


redirection.c:
This file detects the input and output files(to be redirected) and also gives the command with redirection statements.

setenv.c:
It contains the implementation of setenv command.

unsetenv.c:
It conatins the implementation of unsetenv command.

jobs.c:
It contains the implementation of jobs command. Basically it prints whatever is stored in the list of background processes.(name, pid, jobnumber, status is retrieved from proc/pid/stat file).

fg.c:
It conatians the implemntation of fg command.

bg.c:
It conatians the implemntation of bg command. (SIGCONT signal is sent to the process id whose job number is given otherwise error is printed)

kjob.c:
It contains the implementation of kjob command.(Sends the given signal number(if valid) to the process mentioned by job number).

overkill.c:
It contains the implementation of overkill command.(SIGKILL signal is sent to each of the child processes)

BONUS part:
1) cd - is implemented by maintaing a global variable which stores the previous working directory.

2) Exit codes are implemented by maintaining a global variable and its value will be 0 if the command exits successfully and 1 otherwise.


Remaining Files mentioned in Assignment 2 are below-
Compiling the code:
Run make command(make or make all) and it will compile the code and gives 'main' executable.

main.c:
main function contains the shell loop. In this function, the username, hostname and the directory from where the executable is being run are also taken.
pwd command is directly included the loop.
The function giveCommand gives commands seperated by ; and &.
function checkBG checks if there are any background processes that have exited(terminated or stopped). If the background process exits, then on pressing any command or just enter it would display which process(along with pid) that had exited.

pinfo.c:
This file contains the implementation of pinfo command(pinfo function).

prompt.c:
The prompt function displays the prompt after each command has been entered.
The checkHome function checks whether the current directory has home directory as a substring or not.

checkhome.c:
checkhome function checks whether the directory given has ~ and if it is present then it would be replaced by the absolute directory of home.

parse.c:
This function parses the given command as argument seperated by the delimiter provided.(In this case it is space or tab).

cd.c:
This file contains the implementation of cd command(executeCD function).
This command displays error when more than 1 argument is passed and also when the directory provided is invalid.

echo.c:
This file conatins the implementation of echo command(executeEcho function).
This command does not support "" as special, that is it prints the raw string by condensing the spaces to 1 space.

linkedlist.c:
This file contains the struct and the basic functions of a doubly linked list.
push(head,name,id) function inserts the given node at the end of the list and returns its head.
pop(head,node) function deletes the given node(pointer) from the list.
get(head,pid) function returns the node which contains the details of the pid given.

ls.c:
This file contains the implementations of ls command. For /proc/[pid] this command won't work properly.
The command displays error if the options given are different from -l or -a(that is -b or anything which contains alphabets other than a and l).
displayStats() function prints the permissions(by calling printPermissions() function), type of file, number of links, etc. which are displayed when -l flag is given.

programs.c:
This file implements the execution of system programs in foreground and background.

strip.c:
This file contains the strip function which removes leading and trailing spaces in the string passed as argument.

queue.c:
This file conatins the implementation of queue data structure. But this can also be used as linked list to traverse from first element to last element in the queue. This is implemented for maintaing history.

history.c:
This file contains the implementation of history. It has 2 functions-
1. updateHistory - this function updates the history file.(Adds new command and also if number of commands > 20 then remove the oldest and add the newest)
2. executeHistory - this function has the implementation of history command.