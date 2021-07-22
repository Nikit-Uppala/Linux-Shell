#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/utsname.h>
#include<sys/wait.h>
#include<string.h>
#include "prompt.h"
#include "strip.h"
#include "parse.h"
#include "linkedlist.h"
#include<signal.h>
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "pinfo.h"
#include "queue.h"
#include "history.h"
#include "setenv.h"
#include "unsetenv.h"
#include "programs.h"
#include "redirection.h"
#include "jobs.h"
#include "kjob.h"
#include "fg.h"
#include "overkill.h"
#include "bg.h"