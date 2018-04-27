#include "prcserver.h"
#include "prcserver.p"

int add_socket(int fd){
	if(cnt_fd_socket < MAX_FD_SOCKET)
	{
		fd_socket[cnt_fd_socket] =fd;
		return ++cnt_fd_socket;
	}
	else
		return -1;	
}

int del_socket(int fd){
	int i, flag;
	flag = 0; // 1: found, 0: not found
	close(fd);
	//루프를 돌면서 제거할 파일기술자 번호를 찾는다.
	for(i=0; i<cnt_fd_socket; i++)
	{
		if(fd_socket[i] ==fd)
		{
			if(i != (cnt_fd_socket-1))
			 fd_socket[i] = fd_socket[cnt_fd_socket-1];
	fd_socket[cnt_fd_socket-1] = -1;
	flag = 1; // 찾은 경우
	break;
		}
	}
	if(flag == 0)
		return -1;  //못찾은 경우
	--cnt_fd_socket;
	return i;
}

int mk_fds(fd_set *fds, int *a_fd_socket){
	int i, fd_max;
	FD_ZERO(fds);   //파일기술자 세트 초기화
	for(i=0, fd_max=-1;i<cnt_fd_socket;i++)
	{
		fd_max = MAX(fd_max, a_fd_socket[i]);
		FD_SET(a_fd_socket[i], fds);
	}
	return fd_max; //파일기술자 중에 가장 큰 번호를 리턴
}
