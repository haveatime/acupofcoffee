#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/if_link.h>
#include <linux/if_addr.h>
#include <linux/neighbour.h>
#include <linux/netconf.h>
#include <arpa/inet.h>

#include <stdlib.h>

#define NLMSG_TAIL(nmsg) ((struct rtattr *) (((void *) (nmsg)) + NLMSG_ALIGN((nmsg)->nlmsg_len)))

int parsemsg( char *msgbuf, int msglen );

/*******************************
x/32xb addr
*******************************/

void displaymem( unsigned char *addr, int len )
{
	char *dispenv = getenv("DISP_NLFRAME");

	if( NULL == dispenv )
		return;

	int index = 0;
	for( index=0; index<len; index++ )
	{
		printf(" %02X",addr[index]);

		if( (index+1)==len )
			printf("\n");
		else if( (index%16)==15 )
			printf("\n");
	}
}

int appendrtaddr( struct nlmsghdr *n, int maxlen, int type, const void *data, int alen )
{
	int len = RTA_LENGTH(alen);
	struct rtattr *rta;

	if (NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len) > maxlen)
	{
		fprintf(stderr, "appendrtaddr ERROR: message exceeded bound of %d\n", maxlen);
		return -1;
	}

	rta = NLMSG_TAIL(n);
	rta->rta_type = type;
	rta->rta_len = len;

	if (alen)
		memcpy(RTA_DATA(rta), data, alen);

	n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len);

	return 0;
}

int makentsock( )
{
	int ret;

	socklen_t addr_len;
	int sndbuf = 32768;
	int rcvbuf = 1024 * 1024;
	int one = 1;
	struct sockaddr_nl socklocal;

	int sockfd = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
	if (sockfd < 0)
	{
		perror("makentsock open netlink socket error\n");
		return -1;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf)) < 0)
	{
		perror("makentsock setsockopt SO_SNDBUF error\n");
		return -1;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF,
		       &rcvbuf, sizeof(rcvbuf)) < 0)
	{
		perror("makentsock setsockopt SO_RCVBUF error\n");
		return -1;
	}

	/* Older kernels may no support extended ACK reporting */
#if 0
	setsockopt(sockfd, SOL_NETLINK, NETLINK_EXT_ACK,
		   &one, sizeof(one));
#endif

	memset(&socklocal, 0, sizeof(socklocal));
	socklocal.nl_family = AF_NETLINK;
	socklocal.nl_groups = 0;

	if (bind(sockfd, (struct sockaddr *)&socklocal, sizeof(socklocal)) < 0)
	{
		perror("makentsock bind netlink socket error\n");
		return -1;
	}

	addr_len = sizeof(socklocal);
	if (getsockname(sockfd, (struct sockaddr *)&socklocal, &addr_len) < 0)
	{
		perror("makentsock getsockname error\n");
		return -1;
	}

	if (addr_len != sizeof(socklocal))
	{
		fprintf(stderr, "makentsock wrong address length %d\n", addr_len);
		return -1;
	}
	if (socklocal.nl_family != AF_NETLINK)
	{
		fprintf(stderr, "makentsock wrong address family %d\n",
			socklocal.nl_family);
		return -1;
	}

	return sockfd;
}

int sendntlinkmsg( int sockfd, int *seq )
{
	int ret;

	struct
	{
		struct nlmsghdr nlh;
		struct ifinfomsg ifm;
		char buf[1024];
	} req = {};

	req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
	req.nlh.nlmsg_type = RTM_GETLINK;
	req.nlh.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
	req.nlh.nlmsg_seq = ++(*seq);
	req.ifm.ifi_family = AF_UNSPEC;

	__u32 data = RTEXT_FILTER_VF;
	ret = appendrtaddr(&req.nlh, sizeof(req), IFLA_EXT_MASK, &data, sizeof(__u32));
	if (ret != 0)
	{
		fprintf(stderr,"sendntlinkmsg appendrtaddr error\n");
		return -1;
	}

	int reqlen = req.nlh.nlmsg_len;
	printf("sendntlinkmsg: %d bytes\n",reqlen);
	displaymem( (unsigned char*)&req, reqlen );

	int finished = parsemsg( (char*)&req, reqlen );

	ret = send(sockfd, &req, reqlen, 0);
	if (ret < 0)
	{
		fprintf(stderr,"sendntlinkmsg send error\n");
		return -1;
	}

	return ret;
}

int sendntaddrmsg( int sockfd, int *seq )
{
	int ret;

	struct {
		struct nlmsghdr nlh;
		struct ifaddrmsg ifm;
		char buf[128];
	} req = {};

	req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg));
	req.nlh.nlmsg_type = RTM_GETADDR;
	req.nlh.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
	req.nlh.nlmsg_seq = ++(*seq);
	req.ifm.ifa_family = AF_UNSPEC;

	int reqlen = sizeof(req);
	printf("sendntaddrmsg: %d bytes\n",reqlen);
	displaymem( (unsigned char*)&req, reqlen );

	int finished = parsemsg( (char*)&req, reqlen );

	ret = send(sockfd, &req, reqlen, 0);
	if (ret < 0)
	{
		fprintf(stderr,"sendntaddrmsg send error\n");
		return -1;
	}

	return ret;
}

int recvntmsg( int sockfd, char **recvbuf )
{
	int ret;

	int len;
	struct sockaddr_nl nladdr;
	struct iovec iov;
	struct msghdr msg = {};

	msg.msg_name = &nladdr;
	msg.msg_namelen = sizeof(nladdr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	int flags = MSG_PEEK | MSG_TRUNC;

	char *buf;

	iov.iov_base = NULL;
	iov.iov_len = 0;

	do
	{
		len = recvmsg(sockfd, &msg, flags);
	} while (len < 0 && (errno == EINTR || errno == EAGAIN));

	if (len < 0)
	{
		fprintf(stderr, "recvntmsg netlink receive error %s (%d)\n", strerror(errno), errno);
		return -errno;
	}

	if (len == 0)
	{
		fprintf(stderr, "recvntmsg EOF on netlink\n");
		return -ENODATA;
	}

	if (len < 32768)
		len = 32768;
	buf = (char *)malloc(len);
	if (!buf)
	{
		fprintf(stderr, "recvntmsg malloc error: not enough buffer\n");
		return -ENOMEM;
	}

	iov.iov_base = buf;
	iov.iov_len = len;

	do
	{
		len = recvmsg(sockfd, &msg, 0);
	} while (len < 0 && (errno == EINTR || errno == EAGAIN));

	printf("recvntmsg: %d bytes\n",len);
	displaymem( (unsigned char*)iov.iov_base, len );

	*recvbuf = buf;
	return len;
}

int parsemsg( char *msgbuf, int msglen )
{
	int finished = 0;

	struct nlmsghdr *h = (struct nlmsghdr *)msgbuf;

	while( NLMSG_OK(h,msglen) )
	{
		printf("parsemsg nlmsg_len=%d nlmsg_type=%d nlmsg_flags=%d nlmsg_seq=%d nlmsg_pid=%d\n",h->nlmsg_len,h->nlmsg_type,h->nlmsg_flags,h->nlmsg_seq,h->nlmsg_pid);

		if (h->nlmsg_type == NLMSG_DONE)
		{
			finished = 1;
			break;
		}

		h = NLMSG_NEXT(h, msglen);
	}

	return finished;
}

int main( int argc, char **argv )
{
	int myseq = time(NULL);
	int ret;
	int index;

	int myfd = makentsock( );
	if (myfd < 0)
	{
		fprintf(stderr,"main makentsock error\n");
		return -1;
	}

	printf("\n#####################################################################################\n");
	ret = sendntlinkmsg( myfd, &myseq );
	if (ret < 0)
	{
		fprintf(stderr,"main sendntlinkmsg error\n");
		return -1;
	}

	printf("\n#####################################################################################\n");
	while( 1 )
	{
		char *recvbuf = NULL;

		ret = recvntmsg( myfd, &recvbuf );
		if (ret < 0)
		{
			fprintf(stderr,"main recvntmsg error\n");
			return -1;
		}

		int finished = parsemsg( recvbuf, ret );
		free( recvbuf );

		if( 0 != finished )
			break;
	}

	printf("\n#####################################################################################\n");
	ret = sendntaddrmsg( myfd, &myseq );
	if (ret < 0)
	{
		fprintf(stderr,"main sendntaddrmsg error\n");
		return -1;
	}

	printf("\n#####################################################################################\n");
	while( 1 )
	{
		char *recvbuf = NULL;

		ret = recvntmsg( myfd, &recvbuf );
		if (ret < 0)
		{
			fprintf(stderr,"main recvntmsg error\n");
			return -1;
		}

		int finished = parsemsg( recvbuf, ret );
		free( recvbuf );

		if( 0 != finished )
			break;
	}

	return 0;
}

