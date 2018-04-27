#include "prclient.h"
#include "prclient.p"

void error(const char *msg)
{
	perror(msg);
}

int main(int argc, char *argv[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[5012];
	int rcv_buf=5012, snd_buf=5012;
	socklen_t len;
	char server_ip[30]; // = "192.168.70.239";
	int i = 0;
	int prc_num, ip_num;
	int rcv_size;
	IPaddr *ipaddr;
	Process process[30];

	signal(SIGPIPE, SIG_IGN);
	if (argc < 2) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR opening socket");

	snd_buf = 5012;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &snd_buf,sizeof(snd_buf)))
	{
		error("SET error");
	}

	ServerIpChk(server_ip);
	strncpy(server_ip, server_ip, sizeof(server_ip)-1);
	//	printf("%s\n", server_ip);		
	server = gethostbyname(server_ip);
	//	server = gethostbyname(argv[1]);

	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
		error("ERROR connecting");
		exit(1);
	}

/*	len=sizeof(snd_buf);
	if(getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &snd_buf, &len))
		error("GET_SOCK Error");
	len=sizeof(rcv_buf);
	if(getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &rcv_buf, &len))
		error("GET_SOCK Error");
	printf("RCV Socket buffer size: %d\n", rcv_buf);
	printf("SND Socket buffer size: %d\n", snd_buf);
*/
	while(1)
	{	
		memset(buffer, 0x00, sizeof(buffer));

		//클라이언트 IP와 프로세스 내용을 저장할 파일 생성
		//		GetIPinfo();

		//프로세스 내용을 파일에 추가
		PrcFileGen();

		//ip 주소 갯수 파악
		ip_num = IpCount();

		//실행중인 프로세스 갯수 파악
		prc_num = PrcCount();
		ipaddr = (IPaddr *)malloc(ip_num * sizeof(IPaddr));
		//		process = (Process *)malloc(prc_num * sizeof(Process));
		rcv_size = prc_num*sizeof(Process);

		//클라이언트 주소와 프로세스 구조체에 파일의 내용 입력	
		PrcInfoGet(ipaddr, portno, process, ip_num, prc_num, rcv_size);
		if(prc_num == 0)
		{	
			memset(process, 0x00, sizeof(Process));
			//			PrcZeroSet(process, prc_num, rcv_size);
			memcpy(buffer, (Process *)process, sizeof(Process));
			n = send(sockfd, buffer, sizeof(Process),0);
		}
		else
		{
			memcpy(buffer,(Process *)process, prc_num*sizeof(Process));
			//		PrcInfoGet(ipaddr, portno,&buffer, ip_num, prc_num);
			n = send(sockfd, buffer, prc_num*sizeof(Process),0);
		}
		if(n<0)
		{
			error("ERROR writing to socket");
			i++;
			if(i > 2)
			{	
	//			puts("클라이언트 허용 숫자 초과로 인한 서버 접근실패!");
				break;
			}
			while(1)
			{
				sleep(1);

				portno = atoi(argv[1]);
				sockfd = socket(AF_INET, SOCK_STREAM, 0);

				if (sockfd < 0)
					error("ERROR opening socket");

				snd_buf = 5012;
				if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &snd_buf,sizeof(snd_buf)))
				{
					error("SET error");
				}

				bzero((char *) &serv_addr, sizeof(serv_addr));
				serv_addr.sin_family = AF_INET;
				bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
				serv_addr.sin_port = htons(portno);

				if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
					error("ERROR connecting Server");
				else
					break;	
			}
		}
		else
		{
			PrintPrcInfo(process, prc_num);
			fprintf(stdout,"%d bytes sending\n",n);
			printf("Processes Count: %d\n", process[0].prc_num);
		}
		//		n = read(sockfd,buffer,3072);
		//		if (n < 0) 
		//			error("ERROR reading from socket");
		//		printf("%s\n",buffer);
		free(ipaddr);
		//		free(process);
		sleep(5);
	}
	close(sockfd);
	return 0;
}
