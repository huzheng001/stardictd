#include "net.h"

#include <unistd.h>
#include <string.h>
#include <glib.h>

static int socket_fd;

void net_set_fd(int fd)
{
	socket_fd = fd;
}

void net_close_fd()
{
	 close(socket_fd);
}

static bool net_write(const char *buf, int len)
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
	return net_write(str, strlen(str));
}

bool net_read_line(std::string &buf)
{
	buf.clear();
	int n;
	char c;
	while (true) {
		n = read(socket_fd, &c, 1);
		if (n <= 0)
			break;
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

bool net_read_str(std::string &str)
{
	str.clear();
	int n;
	char c;
	while (true) {
		n = read(socket_fd, &c, 1);
		if (n <= 0)
			break;
		switch (c) {
			case '\0':
				return false;
			default:
				str += c;
				break;
		}
	}
	return true;
}

static bool net_read(char *data, int len)
{
	int left_byte = len;
	int count;
	while (left_byte) {
		count = read(socket_fd, data+(len - left_byte), left_byte);
		if (count <= 0)
			return true;
		left_byte -= count;
	}
	return false;
}

bool net_read_data(char **data)
{
	guint32 nsize;
	if (net_read((char *)(&nsize), 4))
		return true;
	guint32 data_size = g_ntohl(nsize);
	*data = (char *)g_malloc(data_size + 4);
	*reinterpret_cast<guint32 *>(*data) = data_size;
	if (net_read(*data+4, data_size)) {
		g_free(*data);
		return true;
	} else {
		return false;
	}
}

