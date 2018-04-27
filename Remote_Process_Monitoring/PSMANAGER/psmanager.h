#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>

#define MSGQ_TYPE               5

#define PROCESS_CHANGED         1
#define PROCESS_NOT_CHANGED     0

typedef struct {
        int prc_num;
        char user[20];
        int pid;
        int ppid;
        int c;
        char stime[10];
        char tty[3];
        char time[20];
        char cmd[50];
}Process;

typedef struct{
	int fd;
        char cli_ip[30];
        Process prc[30];
}iProcess;

typedef struct{
        long msgtype;
        char msgtext[3072];
}Mq_buf;

