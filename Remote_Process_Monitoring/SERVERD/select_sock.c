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
	//������ ���鼭 ������ ���ϱ���� ��ȣ�� ã�´�.
	for(i=0; i<cnt_fd_socket; i++)
	{
		if(fd_socket[i] ==fd)
		{
			if(i != (cnt_fd_socket-1))
			 fd_socket[i] = fd_socket[cnt_fd_socket-1];
	fd_socket[cnt_fd_socket-1] = -1;
	flag = 1; // ã�� ���
	break;
		}
	}
	if(flag == 0)
		return -1;  //��ã�� ���
	--cnt_fd_socket;
	return i;
}

int mk_fds(fd_set *fds, int *a_fd_socket){
	int i, fd_max;
	FD_ZERO(fds);   //���ϱ���� ��Ʈ �ʱ�ȭ
	for(i=0, fd_max=-1;i<cnt_fd_socket;i++)
	{
		fd_max = MAX(fd_max, a_fd_socket[i]);
		FD_SET(a_fd_socket[i], fds);
	}
	return fd_max; //���ϱ���� �߿� ���� ū ��ȣ�� ����
}
