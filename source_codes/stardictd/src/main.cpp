#include "net.h"
#include "servicemain.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include "rsa.h"

#include "stardictdmain.h"
#include "daemon.h"

extern ServiceMain *serviceMain;



#ifndef UID_NOBODY
#define UID_NOBODY  65534
#endif
#ifndef GID_NOGROUP
#define GID_NOGROUP 65534
#endif

static void release_root_privileges()
{
	if (geteuid() == 0) {
		struct passwd *pwd;
		if ((pwd = getpwnam("stardictd"))) {
			int success;
			success = setgid(pwd->pw_gid);
			if (success != 0) {
				printf("setgid stardictd failed!\n");
			}
			initgroups("stardictd",pwd->pw_gid);
			success = setuid(pwd->pw_uid);
			if (success != 0) {
				printf("setuid stardictd failed!\n");
			}
		} else {
			printf("User stardictd doesn't exist!\n");
			exit(0);
		}
	}
}

int main(int argc, char *argv[])
{
	bool daemon_mode = false;
	for (int i=1; i < argc; i++) {
		if (strcmp(argv[i], "-d")==0)
			daemon_mode = true;
	}
	if (daemon_mode)
		net_detach();
	release_root_privileges();
	setlocale (LC_ALL,"");

	rsa_init();
	rsa_gen_key(RSA_Public_Key_d, RSA_Public_Key_n);


	if (serviceMain->init(argc, argv))
		serviceMain->run();
	return 0;
}
