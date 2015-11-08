#ifndef _NET_H
#define _NET_H

#include <string>

extern void net_set_fd(int fd);
extern void net_close_fd();
extern bool net_write_str(const char *str);
extern bool net_read_line(std::string &buf);
extern bool net_read_str(std::string &str);
extern bool net_read_data(char **data);

#endif
