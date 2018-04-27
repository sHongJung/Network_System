#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MSGQ_TYPE	5

#define PROCESS_CHANGED         1
#define PROCESS_NOT_CHANGED     0

#define MAX_FD_SOCKET 		0xff
#define MAX(a,b)	a > b ? a : b

typedef struct {
        int prc_num;
//	int rcv_size;
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
	char ip[30];
	int portno;
}IPaddr; 

typedef struct{
	long msgtype;
	char msgtext[3072];
}Mq_buf;

int fd_socket[MAX_FD_SOCKET];
int cnt_fd_socket;
fd_set fds_read;
int fd_biggest; 
  
