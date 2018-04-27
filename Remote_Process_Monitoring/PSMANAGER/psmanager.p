extern int MntPrcChg(iProcess *process_new, iProcess *process_old); 
extern void PrintPrc(iProcess *iprocess);                                                                                           
extern void PrintAllCli_Th(void);                                                                                           
extern void *Th_PrintPrc(void *arg);
extern void KeyInputPrint(void);
extern void Print_SigHandler(int signo);

