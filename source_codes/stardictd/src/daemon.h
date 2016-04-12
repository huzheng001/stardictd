#ifndef _DAEMON_H
#define _DAEMON_H


#include "rsa.h"


extern int RSA_Public_Key_d[RSA_MAX];
extern int RSA_Public_Key_n[RSA_MAX];


extern int dict_daemon( int s, int delay_time, int max_cost_time, int error);
extern void daemon_terminate( int sig, const char *name );

#endif
