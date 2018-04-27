extern void GetIPinfo(void);
extern void PrcFileGen(void);
extern int IpCount(void);
extern int PrcCount(void);
extern void PrcInfoGet(IPaddr *ipaddr, int portno, Process *process, int ip_num, int prc_num, int rcv_size);
extern void PrcZeroSet(Process *process, int prc_num, int rcv_size);
extern void ServerIpChk(char *str);
extern void PrintPrcInfo(Process *process, int prc_num);
