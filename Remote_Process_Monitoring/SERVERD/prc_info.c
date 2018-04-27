#include "prcserver.h"

int IpChk(char *ipaddr)
{
	FILE *fp = fopen("cli_ipaddr.txt","r");
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	if(fp == NULL) exit(EXIT_FAILURE);
	while((read = getline(&line, &len, fp)) != -1)
	{
		//              printf("접속IP:%s\n등록IP:%s\n", ipaddr, line);

		if(!strncmp(ipaddr,line,14))
		{
			puts("---------------------------");
			puts("ACCESS_OK ( Registered IP )");
			puts("---------------------------");
			return 1;
		}
	}
	free(line);
	fclose(fp);
	return 0;
}

int GetPrcInfo(Process *process)
{
	int i;
	int prc_num = process[0].prc_num;

	puts("----------------------------------------------------------");
	fprintf(stdout, "%-10s%7s %5s %3s %7s %10s %-8s\n", "#USER","PID","PPID","C","STIME","TIME", "CMD");
	puts("----------------------------------------------------------");
	for(i=0; i < prc_num;i++)
	{
		fprintf(stdout, "%-10s%7d %5d %3d %7s %10s %-8s\n",
				process[i].user,
				process[i].pid,
				process[i].ppid,
				process[i].c,
				process[i].stime,
				process[i].time,
				process[i].cmd);
	}
	puts("");
	return 0;

}

int MntPrcChg(Process *process_new, Process *process_old)
{
	int i,j,k=0;
	if(process_new[0].prc_num != process_old[0].prc_num)
		return PROCESS_CHANGED;

	for(i=0;i<process_old[0].prc_num;i++)
	{
		for(j=0;j<process_new[0].prc_num;j++)
			{
			if(!strcmp(process_old[i].cmd,process_new[j].cmd))
				k++;
				return PROCESS_NOT_CHANGED;
			}
	}
	if(!k)
		return PROCESS_CHANGED;				
	
	return PROCESS_NOT_CHANGED;
}	
				
	

