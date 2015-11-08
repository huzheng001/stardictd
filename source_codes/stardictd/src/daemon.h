#ifndef _DAEMON_H
#define _DAEMON_H

extern int dict_daemon( int s, int delay_time, int max_cost_time, int error);
extern void daemon_terminate( int sig, const char *name );

#endif
