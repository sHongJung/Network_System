extern int IpChk(char *ipaddr);
extern int GetPrcInfo(Process *process);
extern int MntPrcChg(Process *process, Process *process_old);
extern int add_socket(int sockfd);
extern int del_socket(int sockfd);
extern int mk_fds(fd_set *fds, int *a_fd_socket);
