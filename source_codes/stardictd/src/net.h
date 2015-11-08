#ifndef _NET_H
#define _NET_H

#include <string>

extern int net_open_tcp (const char *address, int port, int queueLength);
extern void net_detach();
extern char *net_get_hostname();

extern void net_set_fd(int fd);
extern void net_close_fd();
extern bool net_read_line(std::string &buf);
extern bool net_write(const char *buf, int len);
extern bool net_write_str(const char *str);
extern bool net_write_data(const char *data);

#endif
