#include "psmanager.h"
#include "psmanager.p"

int main()
{
	Mq_buf msgq;
	int msgq_size = 0;
	ssize_t nbytes;
	key_t key_msgq = 3339;
	int msg_que_id = msgget(key_msgq, IPC_CREAT|0666);

	int shm_id, i, j, k=0;
	iProcess *shm_addr;
	key_t key_shm = 99995;
	int prc_status;

	iProcess rcv_process[10];
	iProcess rcv_process_cmp;


	//공유메모리 생성
	shm_id = shmget(key_shm,10*sizeof(iProcess), IPC_CREAT|0666);
	if(shm_id == -1)
	{
		puts("Shared memory access error!");
		fprintf(stderr, "%s (KEY_NUM)%d (MEM_SIZE)%ld\n", strerror(errno), key_shm, sizeof(rcv_process[10]));
		return -1;
	} 
	if((shm_addr = shmat(shm_id, (iProcess *)0, 0)) == (void *)-1)
	{
		puts("Shared memory attatch fail!");
		return -1;
	}
	//공유메모리 초기화
//	memset(shm_addr, 0x00, 10*sizeof(iProcess));
	
	//메시지 큐 check
	if(msg_que_id < 0)
	{
		puts("msgget() Error!");
		exit(0);
	}
	 msgq_size = sizeof(msgq) - sizeof(msgq.msgtype);
	
//	if((nbytes = msgrcv(msg_que_id, &msgq, msgq_size,0,0)) > 0)
//		memcpy(shm_addr[0],msgq.msgtext,msgq_size);
	
	
	//Thread로 공유메모리에 저장된 값을 주기적으로 화면에 출력
	PrintAllCli_Th();	
	
	while(1)
	{
		memset(&rcv_process_cmp, 0x00, sizeof(iProcess));
		
		nbytes = msgrcv(msg_que_id, &msgq, msgq_size,0,0);
		if(nbytes > 0)
		{
			memcpy(&rcv_process_cmp,&msgq.msgtext, sizeof(iProcess));
//			printf("Client IP (%s) nbytes (%ld)\n",rcv_process_cmp.cli_ip, nbytes);
			j = 0;
		//같은 IP인지 확인하여 프로세스 변화 감지 및 다른 IP인 경우 shm 배열에 클라이언트 정보 추가
			for(i=0; i< 10; i++)
			{
			//동일 IP인 경우 프로세스 변화 모니터링하여 출력 
				if(!strcmp(shm_addr[i].cli_ip, rcv_process_cmp.cli_ip))
				{
//					printf("공유메모리 ip값[ %s ] 들어온 ip값[ %s ]\n", shm_addr[i].cli_ip, rcv_process_cmp.cli_ip);
					prc_status = MntPrcChg(&rcv_process_cmp, &shm_addr[i]);
				//	printf("공유메모리 ip값[%s] 들어온 ip값[%s]", shm_addr[i].cli_ip, rcv_process_cmp.cli_ip);
					if(prc_status == PROCESS_CHANGED)
						PrintPrc(&rcv_process_cmp);	
					j++;
				}
			}
//			printf("%d\n", j);

			//동일 IP가아닌경우 공유메모리에 새로운 클라이언트 구조체를 추가
			if(!j)
			{
				memcpy(&shm_addr[k],&rcv_process_cmp, msgq_size);
				k++;
			}	
		}

	// 키보드 특정 문자 입력시 공유 메모리내 클라이언트 화면출력 시그널 처리
	signal(SIGIO, (void *)Print_SigHandler); 
	 
	}	

}
