#include "prclient.h"
#include "prclient.p"

char cli_id[10] = {0,};
int id_size = 0;
char logname[20];
char lognameget[256];

void GetIPinfo(){
	system("ifconfig | grep \"inet \" >> procfile.txt");
}

void PrcFileGen(){
	strcpy(logname, getenv("LOGNAME"));
	strcpy(lognameget, "ps -eo uname:11,pid,ppid,c,stime,tty,time,cmd  | awk '$3 ~ /^1$/ {print}' | grep ");
	strcat(lognameget, logname);
	strcat(lognameget, " | grep -v java | grep -v IOTGW > procfile.txt");
	system(lognameget);
	//	system("ps -eo uname:11,pid,ppid,c,stime,tty,time,cmd  | awk '$3 ~ /^1$/ {print}' | grep %s | grep -v java >> procfile.txt",logname);
}

void ServerIpChk(char *str)
{
	FILE *fp = fopen("server_ip.txt","r");
	char temp[30];

	if(fp == NULL) exit(EXIT_FAILURE);

	fgets(temp,sizeof(temp), fp);
	sscanf (temp, "%s", str);
//	printf("\nNot Registered IP in Config File : [ %s ]\n", str); // [ %s ](%d,%d)-->[%s]\n",temp, sizeof(temp), strlen(temp), str);	
	//strcpy(str, temp);

	fclose(fp);
}

int IpCount(){
	char temp[256];
	int i = 0;
	FILE *fp = fopen("procfile.txt", "r");

	strcpy(cli_id, logname);
	id_size = strlen(cli_id);	

	if(fp == NULL)
		puts("FILE OPEN ERROR!");

	while((fgets(temp, sizeof(temp), fp))!=NULL)
	{

		if(!strncmp(temp, cli_id, id_size))
			continue;
		i++;
	}	fclose(fp);

	return i;

}

int PrcCount(){
	char temp[256];
	int i = 0;
	FILE *fp = fopen("procfile.txt", "r");

	if(fp == NULL)
		puts("FILE OPEN ERROR!");
	
//	memset(temp, 0x00, sizeof(temp));
	while((fgets(temp, sizeof(temp), fp))!=NULL)
	{
		if(strncmp(temp, cli_id, id_size))
			continue;
		i++;
	}
	if(strlen(temp) == 0 )
		i = 0;
	
	fclose(fp);
	return i;
}

void PrcInfoGet(IPaddr *ipaddr, int portno, Process *process, int ip_num, int prc_num, int rcv_size){
	char temp[256];
	char *ipget;
	FILE *fp = fopen("procfile.txt", "r");
	int i = 0, j=0, k=0;

	if(fp == NULL)
		puts("FILE OPEN ERROR!");

	while((fgets(temp, sizeof(temp),fp))!=NULL)
	{
		if(strncmp(temp, cli_id, id_size))
		{
			ipget = strtok(temp, ":");
			ipget = strtok(NULL, ":");
			sscanf(ipget,"%s", ipaddr[i].ip );
			ipaddr[i].portno = portno;
			i++;
		}

		else if(!strncmp(temp,cli_id,id_size))
		{
			sscanf(temp,"%s %d %d %d %s %s %s %s",
					process[j].user,
					&process[j].pid,
					&process[j].ppid,
					&process[j].c,
					process[j].stime,
					process[j].tty,
					process[j].time,
					process[j].cmd);
			j++;
		}
		for(k=0; k<j; k++)
		{
			process[k].prc_num = prc_num;
		}
	}
	fclose(fp);

}

void PrintPrcInfo(Process *process, int prc_num)
{
	int i;
        puts("--------------------------------------------------------------------");
        fprintf(stdout, "%-10s%7s %5s %3s %7s %5s %12s %-8s\n", "#USER","PID","PPID","C","STIME","TTY","TIME", "CMD");
        puts("--------------------------------------------------------------------");
        for(i=0; i < prc_num;i++)
        {
                fprintf(stdout, "%-10s%7d %5d %3d %7s %5s %12s %-8s\n",
                                process[i].user,
                                process[i].pid,
                                process[i].ppid,
                                process[i].c,
                                process[i].stime,
                                process[i].tty,
                                process[i].time,
                                process[i].cmd);
        }
        puts("");
                fprintf(stdout, "%s %d\n",
						__func__,
						__LINE__);
}

