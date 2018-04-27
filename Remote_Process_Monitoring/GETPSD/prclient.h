#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio_ext.h>
#include <netdb.h>
#include <signal.h>

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
	char ip[30];
	int portno;
}IPaddr;	

