#include "net.h"

#include <glib.h>

#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>


void net_detach()
{
	if (daemon(0, 0) != 0) {
		fprintf(stderr, "Create daemon error!\n");
		exit(1);
	}
#if 0
	switch (fork()) {
	case -1:
		fprintf(stderr, "Cannot fork\n");
		exit(1);
		break;
	case 0:
		break; /* child */
	default:
		exit(0); /* parent */
	}
	int i;
	for (i=getdtablesize()-1; i >= 0; --i)
		close(i); /* close everything */
	int fd;
	if ((fd = open("/dev/tty", O_RDWR)) >= 0) {
		/* detach from controlling tty */
		ioctl(fd, TIOCNOTTY, 0);
		close(fd);
	}
	chdir("/");	/* cd to safe directory */
	umask(0);	/* set safe umask */
	setpgid(0,getpid());	/* Get process group */
	fd = open("/dev/null", O_RDWR);    /* stdin */
	dup(fd);                           /* stdout */
	dup(fd);                           /* stderr */
#endif
}

int net_open_tcp (const char *address, int port, int queueLength)
{
	struct protoent *protocolEntry;
	struct sockaddr_in ssin;
	int s;
	const int one = 1;

	memset( &ssin, 0, sizeof(ssin) );
	ssin.sin_family = AF_INET;
	ssin.sin_addr.s_addr = address ? inet_addr(address) : htonl(INADDR_ANY);
	if (!(ssin.sin_port = htons(port)))
		exit(1);
	if (!(protocolEntry = getprotobyname("tcp")))
		exit(1);
	if ((s = socket(PF_INET, SOCK_STREAM, protocolEntry->p_proto)) < 0)
		exit(1);
	setsockopt( s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one) );
	if (bind(s, (struct sockaddr *)&ssin, sizeof(ssin)) < 0)
		exit(1);
	if (listen( s, queueLength ) < 0)
		exit(1);
	return s;

	/*int s;
	s = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(2628);
	bind(s, (struct sockaddr *)&addr, sizeof(addr));
	listen(s, 10);
	return s;*/
}

#define MAXHOSTNAMELEN 64

char *net_get_hostname()
{
	char netHostname[MAXHOSTNAMELEN];
	memset( netHostname, 0, sizeof(netHostname) );
	gethostname( netHostname, sizeof(netHostname)-1 );
	struct hostent *hostEntry;
	if ((hostEntry = gethostbyname(netHostname)))
		return g_strdup(hostEntry->h_name);
	else
		return g_strdup(netHostname);
}

static int socket_fd;

void net_set_fd(int fd)
{
	socket_fd = fd;
}

void net_close_fd()
{
	close(socket_fd);
}

bool net_read_line(std::string &buf)
{
	buf.clear();
	int n;
	char c;
	int read_len = 0;
	while (true) {
		n = read(socket_fd, &c, 1);
		if (n <= 0)
			break;
		read_len++;
		if (read_len > 10240)
			return true;
		switch (c) {
			case '\n':
				return false;
			case '\r':
				break;
			default:
				buf += c;
				break;
		}
	}
	return true;
}

bool net_write(const char *buf, int len)
{
	int left_byte = len;
	int count;
	while (left_byte) {
		count = write(socket_fd, buf+(len - left_byte), left_byte);
		if (count <= 0 )
			return true;
		left_byte -= count;
	}
	return false;
}

bool net_write_str(const char *str)
{
	return net_write(str, strlen(str)+1);
}

bool net_write_data(const char *data)
{
	guint32 data_size = *reinterpret_cast<const guint32 *>(data);
	guint32 nsize = g_htonl(data_size);
	if (net_write((const char *)(&nsize), 4))
		return true;
	return net_write(data+4, data_size);
}
