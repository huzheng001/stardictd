#include <glib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <locale.h>

#include "net.h"
#include "command.h"


bool connect_server(const char *host, const char *port)
{
	struct sockaddr_in ssin;
	memset( &ssin, 0, sizeof(ssin) );
	ssin.sin_family = AF_INET;
	if (!(ssin.sin_port = htons(atoi(port))))
		return true;
	struct protoent *protocolEntry;
	if (!(protocolEntry = getprotobyname("tcp")))
		return true;
	int hosts = 0;
	struct hostent *hostEntry;
	if ((hostEntry = gethostbyname(host))) {
		++hosts;
	} else if ((ssin.sin_addr.s_addr = inet_addr(host)) == htonl(INADDR_NONE))
		return true;
	int socket_fd;
	if (hosts) {
		char **current;
		for (current = hostEntry->h_addr_list; *current; current++) {
			memcpy( &ssin.sin_addr.s_addr, *current, hostEntry->h_length );
			if ((socket_fd = socket(PF_INET, SOCK_STREAM, protocolEntry->p_proto)) < 0) {
				printf("Socket error! It should be SELinux's setting problem.\n");
				return true;
			}
			if (connect(socket_fd, (struct sockaddr *)&ssin, sizeof(ssin)) >= 0) {
				net_set_fd(socket_fd);
				return false;
			}
			close(socket_fd);
		}
	} else {
		if ((socket_fd = socket(PF_INET, SOCK_STREAM, protocolEntry->p_proto)) < 0)
			return true;
		if (connect(socket_fd, (struct sockaddr *)&ssin, sizeof(ssin)) >= 0) {
			net_set_fd(socket_fd);
			return false;
		}
		close(socket_fd);
	}
	return true;
}

int main( int argc, char **argv )
{
	setlocale (LC_ALL,"");
	append_command(false, make_command( CMD_CLIENT, "0.4", "stardict-client" ) );

	const char *host = "127.0.0.1";
	const char *port = "2629";
	int i;
	bool append_sep;
	for (i=1; i< argc; i++) {
		if (argv[i][0]=='-') {
			append_sep = false;
		} else if (argv[i][0]=='+') {
			append_sep = true;
		} else {
			continue;
		}
		switch (argv[i][1]) {
		case 'a':
			if (argv[i][2]=='r')
				append_command(append_sep, make_command( CMD_AUTH, 0, argv[i+1], argv[i+2]));
			else
				append_command(append_sep, make_command( CMD_AUTH, 1, argv[i+1], argv[i+2]));
			i+=2;
			break;
		case 'b':
			append_command(append_sep, make_command(CMD_TMP_DICTMASK, argv[i+1]));
			i++;
			break;
		case 'c':
			if (argv[i][2]=='r')
				append_command(append_sep, make_command( CMD_CHANGE_PASSWD, 0, argv[i+1], argv[i+2], argv[i+3]));
			else
				append_command(append_sep, make_command( CMD_CHANGE_PASSWD, 1, argv[i+1], argv[i+2], argv[i+3]));
			i+=3;
			break;
		case 'd':
			append_command(append_sep, make_command( CMD_DEFINE, argv[i+1] ) );
			i++;
			break;
		case 'g':
			if (argv[i][2]=='d')
				append_command(append_sep, make_command( CMD_GET_DICT_MASK ) );
			else if (argv[i][2]=='m')
				append_command(append_sep, make_command( CMD_MAX_DICT_COUNT ) );
			else if (argv[i][2]=='f')
				append_command(append_sep, make_command( CMD_GET_COLLATE_FUNC ) );
			else if (argv[i][2]=='l')
				append_command(append_sep, make_command( CMD_GET_LANGUAGE ) );
			else if (argv[i][2]=='e')
				append_command(append_sep, make_command( CMD_GET_EMAIL ) );
			else if (argv[i][2]=='u')
				append_command(append_sep, make_command( CMD_GET_USER_LEVEL ) );
			break;
		case 'h':
			i++;
			host = argv[i];
			break;
		case 'i':
			if (argv[i][2]=='r') {
				if (argv[i][3]=='d')
					append_command(append_sep, make_command( CMD_DIR_INFO, argv[i+1], 0 ) );
				else
					append_command(append_sep, make_command( CMD_DIR_INFO, argv[i+1], 1 ) );
			} else if (argv[i][2]=='d') {
				if(argv[i][3]=='a')
					append_command(append_sep, make_command( CMD_DICTS_LIST, argv[i+1]) );
				else
					append_command(append_sep, make_command( CMD_DICT_INFO, argv[i+1] ) );
			}
			i++;
			break;
		case 'l':
			if (argv[i][2]=='p')
				append_command(append_sep, make_command( CMD_PREVIOUS, argv[i+1] ) );
			else if (argv[i][2]=='n')
				append_command(append_sep, make_command( CMD_NEXT, argv[i+1] ) );
			else
				append_command(append_sep, make_command( CMD_LOOKUP, argv[i+1] ) );
			i++;
			break;
		case 'p':
			i++;
			port = argv[i];
			break;
		case 'q':
			if (argv[i][2]=='s')
				append_command(append_sep, make_command( CMD_SELECT_QUERY, argv[i+1] ) );
			else
				append_command(append_sep, make_command( CMD_QUERY, argv[i+1] ) );
			i++;
			break;
		case 'r':
			if (argv[i][2]=='r')
				append_command(append_sep, make_command( CMD_REGISTER, 0, argv[i+1], argv[i+2], argv[i+3]));
			else
				append_command(append_sep, make_command( CMD_REGISTER, 1, argv[i+1], argv[i+2], argv[i+3]));
			i+=3;
			break;
		case 's':
			if (argv[i][2]=='d')
				append_command(append_sep, make_command( CMD_SET_DICT_MASK, argv[i+1] ) );
			else if (argv[i][2]=='f')
				append_command(append_sep, make_command( CMD_SET_COLLATE_FUNC, argv[i+1] ) );
			else if (argv[i][2]=='l')
				append_command(append_sep, make_command( CMD_SET_LANGUAGE, argv[i+1] ) );
			else if (argv[i][2]=='e')
				append_command(append_sep, make_command( CMD_SET_EMAIL, argv[i+1] ) );
			i++;
			break;
		case 'u':
			append_command(append_sep, make_command( CMD_USER_LEVEL, argv[i+1], argv[i+2], argv[i+3]));
			i+=3;
			break;
		case 'z':
			append_command(append_sep, make_command(CMD_FROMTO));
			break;
		}
	}
	if (connect_server(host, port)) {
		clean_command();
		printf("Connect to %s:%s failed!\n", host, port);
		exit(1);
	}
	append_command(false, make_command( CMD_QUIT ) );
	process_command();
	clean_command();
	net_close_fd();
	return FALSE;
}
