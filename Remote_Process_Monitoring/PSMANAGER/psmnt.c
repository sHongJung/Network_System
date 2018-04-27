#include "psmanager.h"
#include "psmanager.p"

int MntPrcChg(iProcess *client_new, iProcess *shm)
{
	int i,j,k=0;
	//	printf("공유메모리 프로세스 숫자: (%d) 새로 들어온 프로세스 숫자: (%d)\n", shm->prc[0].prc_num, client_new->prc[0].prc_num);
	if(client_new->prc[0].prc_num != shm->prc[0].prc_num)
	{
		puts("");
		puts("***************************************************");
		printf("(%s) Client's processes status changed!\n",shm->cli_ip);
		puts("***************************************************");

		memcpy(shm, client_new, sizeof(iProcess));

		return PROCESS_CHANGED;
	}
	for(i=0;i<shm->prc[0].prc_num;i++)
	{
		k=0;
		for(j=0;j<client_new->prc[0].prc_num;j++)
		{
			if(!strcmp(shm->prc[i].cmd, client_new->prc[j].cmd))
				k++;
		}
		if(!k)
		{
			puts("");
			puts("***************************************************");
			printf("%s Client's processes status changed!\n",shm->cli_ip);
			puts("***************************************************");

			memcpy(shm, client_new, sizeof(iProcess));

			return PROCESS_CHANGED;
		}
	}

	return PROCESS_NOT_CHANGED;
}

void PrintPrc(iProcess *iprocess)
{
	int i;
	int prc_num = iprocess -> prc[0].prc_num;

	printf("\nThe number of processes of [ %s ]: ( %d )\n", iprocess->cli_ip, prc_num);
	puts("----------------------------------------------------------");
	fprintf(stdout, "%-10s%7s %5s %3s %7s %10s %-8s\n", "#USER","PID","PPID","C","STIME","TIME", "CMD");
	puts("----------------------------------------------------------");
	for(i=0; i < prc_num;i++)
	{       
		fprintf(stdout, "%-10s%7d %5d %3d %7s %10s %-8s\n",
				iprocess->prc[i].user,
				iprocess->prc[i].pid,
				iprocess->prc[i].ppid,
				iprocess->prc[i].c,
				iprocess->prc[i].stime,
				iprocess->prc[i].time,
				iprocess->prc[i].cmd);
	}
	puts("");
} 

void PrintAllCli_Th(void)
{
	//Thread 관련 변수
	int state;
	pthread_t t_id;
	pthread_attr_t attr;

	//쓰레드 생성
	state=pthread_attr_init(&attr);
	if(state)
	{
		puts("Thread Attribute Initialize Error!");
		exit(1);
	}

	pthread_attr_setscope ( &attr, PTHREAD_SCOPE_SYSTEM);

	state=pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(state)
	{
		puts("Thread Detach  Error!");
		exit(1);
	}
	state=pthread_create(&t_id, &attr, Th_PrintPrc, NULL);
	if(state)
	{
		puts("Thread Creation Error!");
		exit(1);
	}
	/*	state = pthread_attr_destroy(&attr);
		if(state)
		{
		puts("Thread Destruction Error!");
		exit(1);
		}
		*/
}

void *Th_PrintPrc(void *arg)
{

	int shm_id, i, j;
	iProcess *shm_addr;
	key_t key_shm = 99995;

	arg=arg;

	shm_id = shmget(key_shm,10*sizeof(iProcess), IPC_CREAT|0666);
	if(shm_id == -1)
	{
		puts("Shared memory access error!");
		fprintf(stderr, "%s (KEY_NUM)%d (MEM_SIZE)%ld\n", strerror(errno), key_shm, 10*sizeof(iProcess));
	}
	if((shm_addr = shmat(shm_id, (iProcess *)0, 0)) == (void *)-1)
	{
		puts("Shared memory attatch fail!");
	}

	while(1)
	{
		for(i=0; i< 10 ;i++)
		{
			if(!shm_addr[i].cli_ip[0])
				continue;

			printf("\nThe processes of Client [ %s ]\n", shm_addr[i].cli_ip);
			puts("----------------------------------------------------------");
			fprintf(stdout, "%-10s%7s %5s %3s %7s %10s %-8s\n", "#USER","PID","PPID","C","STIME","TIME", "CMD");
			puts("----------------------------------------------------------");
			for(j=0; j < (shm_addr[i].prc[0].prc_num);j++)
			{       
				fprintf(stdout, "%-10s%7d %5d %3d %7s %10s %-8s\n",
						shm_addr[i].prc[j].user,
						shm_addr[i].prc[j].pid,
						shm_addr[i].prc[j].ppid,
						shm_addr[i].prc[j].c,
						shm_addr[i].prc[j].stime,
						shm_addr[i].prc[j].time,
						shm_addr[i].prc[j].cmd);
			}
			puts("");
		}

		sleep(10);
	}

}


// 시그널 핸들러
void Print_SigHandler(int signo)
{
	int shm_id, i, j;
	iProcess *shm_addr;
	key_t key_shm = 99995;

	shm_id = shmget(key_shm,10*sizeof(iProcess), IPC_CREAT|0666);
	if(shm_id == -1)
	{
		puts("Shared memory access error!");
		fprintf(stderr, "%s (KEY_NUM)%d (MEM_SIZE)%ld\n", strerror(errno), key_shm, 10*sizeof(iProcess));
	}
	if((shm_addr = shmat(shm_id, (iProcess *)0, 0)) == (void *)-1)
	{
		puts("Shared memory attatch fail!");
	}

	for(i=0; i< 10 ;i++)
	{
		if(!shm_addr[i].cli_ip[0])
			continue;
		printf("\nThe processes of Client [ %s ]\n", shm_addr[i].cli_ip);
		puts("----------------------------------------------------------");
		fprintf(stdout, "%-10s%7s %5s %3s %7s %10s %-8s\n", "#USER","PID","PPID","C","STIME","TIME", "CMD");
		puts("----------------------------------------------------------");
		for(j=0; j < (shm_addr[i].prc[0].prc_num);j++)
		{       
			fprintf(stdout, "%-10s%7d %5d %3d %7s %10s %-8s\n",
					shm_addr[i].prc[j].user,
					shm_addr[i].prc[j].pid,
					shm_addr[i].prc[j].ppid,
					shm_addr[i].prc[j].c,
					shm_addr[i].prc[j].stime,
					shm_addr[i].prc[j].time,
					shm_addr[i].prc[j].cmd);
		}
		puts("");
	}
	puts("Signal Print Out!");
	puts("-----------------------------------------------------------");
	puts("");
	puts("");
	puts("");
}
