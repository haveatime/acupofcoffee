#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#endif

#include <QCoreApplication>
#include <QSocketNotifier>
#include <QStringList>
#include <QDebug>

class DaeSocketNotifier : public QSocketNotifier
{
public:
  DaeSocketNotifier (int fd) : QSocketNotifier (fd, QSocketNotifier::Read) {};

protected:
  bool event (QEvent *e)
  {
		if ((e->type() == QEvent::SockAct) /*|| (e->type() == QEvent::SockClose)*/)
		{
			int recvnum;
			int ret;
#ifdef WIN32
			ret = ioctlsocket(socket(), FIONREAD, (unsigned long *)&recvnum);
#else
			ret = ioctl(socket(), FIONREAD, &recvnum);
#endif

			if (ret != -1)
			{
				QVector<char> recvbuf(recvnum+1);
				sockaddr_in remote_addr;
#ifdef WIN32
				int	remote_len;
#else
				socklen_t	remote_len;
#endif
				remote_len = sizeof(remote_addr);

				recvnum = recvfrom( socket(), recvbuf.data(), recvbuf.size()-1, 0, (sockaddr *)&remote_addr, &remote_len );
				if( recvnum > 0 )
				{
					recvbuf.last() = 0;
					qDebug()<<"DaeSocketNotifier SockAct recvnum="<<recvbuf.size()-1<<"remote="<<inet_ntoa(remote_addr.sin_addr);
					qDebug()<<"Message="<<recvbuf.data();
					qDebug()<<"";
				}
			}

			return TRUE;
		}
		else
			return QSocketNotifier::event(e);
  };
};

int main(int argc, char **argv)
{
#ifdef WIN32
	WSAData wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);
#endif

	QCoreApplication qtapp(argc, argv);

	QStringList qtargs = qtapp.arguments();
	if( qtargs.size() <= 1 )
	{
		qDebug()<<"Usage: socketnotifer portno";
		return 1;
	}

	DaeSocketNotifier *sn;
    int devfd = socket( AF_INET, SOCK_DGRAM, 0 );

	struct sockaddr_in server_addr;
	memset( &server_addr, 0, sizeof(server_addr) );
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int portno = qtargs.at(1).toInt();
	qDebug()<<"portno="<<portno;
	server_addr.sin_port = htons( portno );

    int ret = bind( devfd, (sockaddr *)&server_addr, sizeof(server_addr) );
	if( 0 == ret )
		sn = new DaeSocketNotifier(devfd);

	qtapp.exec();

    return 0;
}
