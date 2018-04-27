#include "prcserver.h"
#include "prcserver.p"

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

char cli_ip[30];

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno,ret_select, i,j;
	socklen_t clilen;
//	char buffer_old[5120];
//	char buffer[5120];
	struct sockaddr_in serv_addr, cli_addr;
	int cli_num;
	int client_n[10];
//	char cli_ip[30];
	char cli_ipaddr[10][30];		// Ŭ���̾�Ʈ �ּҸ� 10������ �޴� 2���� �迭, Ŭ���̾�Ʈ ���� ������ �����Ҵ��ʿ�
	int n, chk; // prc_status;
	int rcv_buf=5120, snd_buf=5120;
	int reuse = 1;
	Process buffer[30];

//	mqd_t mq_fd; // POSIX �޽��� ť �ĺ��� mq_open()�Լ� return������ ����
	//	char buf_err[128];

	Mq_buf msgq;
	int msgq_size = 0;
//	ssize_t nbytes;
	key_t key_msgq = 3339;
	int msg_que_id = msgget(key_msgq, IPC_CREAT|0666);
	iProcess rcv_process[10];
	Process process[30];

	//�޽��� ť check
	if(msg_que_id < 0)
	{
		puts("msgget() Error!");
		exit(0);
	}

	msgq_size = sizeof(msgq) - sizeof(msgq.msgtype);
	msgq.msgtype = MSGQ_TYPE;

	//TCP ���� �ʱ�ȭ
	for(i=0; i<MAX_FD_SOCKET; i++)
		fd_socket[i] = -1;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
		
	//TCP ����  reuse ����
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
	{
		error("REUSE SET error");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");

	listen(sockfd,5);

	add_socket(sockfd);   //������ ���� �迭�� fd_socket�� �����߰�


	/*
	//���� ������ ����
	rcv_buf = 5000;
	if(setsockopt(newsockfd, SOL_SOCKET, SO_RCVBUF, &rcv_buf, sizeof(rcv_buf)))
	{
	error("SET error");
	}
	*/


	/* POSIX �޽���ť
	   struct mq_attr mq_attrib={.mq_maxmsg = 40}
	   if((mq_fd = mq_open("/my_mq", O_RDWR|OCREAT, 0666, &mq_attrib)) >0)
	   {
	   puts("Creat Message Queue!");
	   }
	   else {
	   if(errno != EEXIST){
	   strerror_r(errno, buf_err, sizeof(buf_err));
	   exit(EXIT_FAILURE);
	   }
	   }
	   if((mq_fd = mq_open("/my_mq", O_RDWR)) == (mqd_t)-1){	//�̹� �����ϰ� �ִ� ���
	   strerror_r(errno, buf_err, sizeof(buf_err));
	   printf("FAIL: mq_open(): %s\n", buf_err);
	   exit(EXIT_FAILURE);
	   }	
	   */


/*
	   memset(buffer_old, 0x00, sizeof(buffer_old));

	//Ŭ���̾�Ʈ�� ���� process_old �ʱⰪ ����
	n = recv(newsockfd,buffer_old,sizeof(buffer_old),0);
	if (n < 0) error("ERROR reading from socket");
	process_old = (struct Process*)buffer_old;
	sleep(5);

*/

	while(1)
	{
		fd_biggest = mk_fds(&fds_read, fd_socket); //fd_set ����ü�� �����ϰ� ���� ū fd���� ��ȯ
		if((ret_select = select(fd_biggest+1, &fds_read, NULL, NULL, NULL)) == -1)
		{
			puts("Select Error!");
		}
		// listen ���� ���Ͽ� �̺�Ʈ�� �ִ��� üũ
		if(FD_ISSET(sockfd, &fds_read))
		{
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if(newsockfd < 0)
			{ 
				error("ERROR on accept");
				continue;
			}	
			if((cli_num = add_socket(newsockfd)) == -1)
			{	
				puts("Socket addition Error!");
			}

			if(setsockopt(newsockfd, SOL_SOCKET, SO_REUSEADDR, &rcv_buf, (int)sizeof(rcv_buf)))
			{	
				error("SET error");
			}

			strcpy(cli_ip, inet_ntoa(cli_addr.sin_addr));

			//chk ����0�̸�cli_ipaddr.txt ���Ͽ� ��ϵ��� ���� IP
			chk = IpChk(cli_ip);      

			//	printf("Permission sign : %d\n", chk); 
			if(chk == 0)
			{
				printf("��ϵ��� ���� IP [ %s ], ����!\n", cli_ip);
				del_socket(newsockfd);
				continue;				
			}
			
			//���ӵ� ������ 3�� �ʰ��ϰ��, �� Ŭ���̾�Ʈ�� 2�� ������  ���� ���
			if(cnt_fd_socket > 3)
			{
				puts("Ŭ���̾�Ʈ ��� ����(2) �ʰ� ����!");
				del_socket(newsockfd);
				continue;
			}
	
			cli_num = cli_num -1;
			printf("Accept : add socket (%d)  Client number : (%d) IP: (%s)\n\n\n", newsockfd, cli_num, cli_ip); //cnt_fd_socket Ȯ�ΰ���
			client_n[cli_num-1] = cli_num-1;
			strcpy(cli_ipaddr[cli_num-1], cli_ip);
			continue;    //���ο� ������ �߰��Ǿ����Ƿ� �ٽ� select ȣ��
		}

		for(i=1; i<cnt_fd_socket; i++)	//fd_socket[1]���� �Ϲ� ������ �б� ���� �̺�Ʈ
		{	
			if(FD_ISSET(fd_socket[i],&fds_read))
			{
				memset(buffer, 0x00, sizeof(buffer));

				n = recv(fd_socket[i],buffer,sizeof(buffer),0);
				//				accept(fd_socket[i],(struct sockaddr *) &cli_addr, &clilen);
				//				strcpy(cli_ip, inet_ntoa(cli_addr.sin_addr));
				if (n < 0)
					error("ERROR reading from socket");
//				else if(n > 1448)
//					recv(fd_socket[i],buffer,sizeof(buffer),0);
				else if(n == 0) 
				{
					printf("%s Session closed\n\n\n", cli_ipaddr[i-1]);
					del_socket(fd_socket[i]); //fd_socket �迭���� ����
					//������ Ŭ���̾�Ʈ ��ȣ ������ Ŭ���̾�Ʈ ������ �ѹ�ȣ�� �����ش�
					j = i;
					while((j)<(cnt_fd_socket+1))
					{
						strcpy(cli_ipaddr[j-1], cli_ipaddr[j]);
						j++;
					}
					continue;
				}

				//���� ���۷� ���� ����ü ������ ��´�.
				memcpy(process, buffer, sizeof(buffer));

				//Ŭ���̾�Ʈ ���μ����� 0�ΰ�� ���� �޸��� ���μ��� ������ �ʱ�ȭ
				if( process[0].prc_num == 0)
					memset(&rcv_process[0], 0x00, sizeof(iProcess));
//			
				//���� ����ü���� Ŭ���̾�Ʈ fd �� ip�ּҿ� �� ����ü��  ���  �޽��� ť�� ����
				rcv_process[i-1].fd = client_n[i-1];
				strcpy(rcv_process[i-1].cli_ip, cli_ipaddr[i-1]);
				memcpy(&rcv_process[i-1].prc, process, (process[0].prc_num)*sizeof(Process));
		//		printf("(%d) processes (%d) bytes from Client [%s] client_fd(%d)\n",rcv_process[i-1].prc[0].prc_num, n, \
												cli_ipaddr[i-1],rcv_process[i-1].fd);

				// GetPrcInfo(process);  // TCP�κ��� ���� Ŭ���̾�Ʈ ���μ��� ���� ���
				memset(&msgq.msgtext, 0x00, msgq_size);
//				msgq_size = sizeof(msgq) - sizeof(msgq.msgtype);
//				msgq.msgtype = MSGQ_TYPE;
				memcpy(&msgq.msgtext, &rcv_process[i-1], msgq_size);

				if( msgsnd(msg_que_id, &msgq, msgq_size, 0) == -1)
				{
					puts("Message Send Fail");
				}
			}
		}

		//	printf("prc_old: %d  prc_new: %d\n",process_old[0].prc_num, process[0].prc_num);

		//�޼���ť�� ���� ����MGR �������  Ŭ���̾�Ʈ ���μ��� ���� ����



		/* ���μ��� ��ȭ �� �κ�


		//���μ����� ���� ���μ��� Ŀ�ǵ带 ���Ͽ� ���º�ȭ�� ����͸�
		prc_status = MntPrcChg(process, process_old);

		//���º�ȭ������� ȭ�鿡 ����ϰ� process_old�� ���ο� ���� �Է��ϰ� �޼��� ť�� ���� �ٸ� ������  ����
		if(prc_status == PROCESS_CHANGED)
		{
		puts("");
		puts("***************************************************");
		printf("\n%s Client's processes status changed!\n\n", cli_ip);
		puts("***************************************************");

		memcpy(process_old, process, process[0].prc_num*sizeof(Process));

		}

*/

		//		fprintf(stdout,"%d bytes from %s Client\n\n", n, cli_ip);
		//	n = write(newsockfd,"Client to Sever Success",23);
		//	if (n < 0) error("ERROR writing to socket");
	}

	close(newsockfd);
	close(sockfd);
	return 0; 
}
