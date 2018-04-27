# include <sys/types.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/ioctl.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/epoll.h>
# include <net/if.h>
# include <linux/sockios.h>

//static bool linux_getMyIP(int nFamily)
int main(int nFamily)
{
	int             sock;
	int             nRet;

	size_t          nNIC;
	const size_t    nMaxNIC = 256;

	struct ifconf   ifc;
	struct ifreq    ifr[nMaxNIC];

	struct sockaddr* pAddr(NULL);
	struct sockaddr_in addr4;
	struct sockaddr_in6 addr6;

	nFamily = AF_INET;
	sock = socket(nFamily, SOCK_STREAM, 0);
	if ( sock == -1 ) return false;

	ifc.ifc_len = sizeof(ifr);
	ifc.ifc_ifcu.ifcu_req = ifr;

	nRet = ioctl(sock, SIOCGIFCONF, &ifc);
	if ( nRet == -1) return false;

	close(sock);

	nNIC = ifc.ifc_len / sizeof(struct ifreq);

	addr_buf.nIPVersion = nFamily;

	for ( size_t i = 0 ; i < nNIC; i ++ )
	{
		if ( nFamily == ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr.sa_family )
		{
			// todo convert binary to string...
			// if nFamily is AF_INET then pAddr is struct sockaddr_in*.
			// else if nFamily is AF_INET6 then pAddr is struct sockaddr_in6*.
			pAddr = (&ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr);
		}
	}

	return 0;
}
