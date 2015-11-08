#ifndef _CONF_H_
#define _CONF_H_

#include <string>
#include <map>

class Conf {
public:
	Conf();
	bool get_bool(const char *key);
	int get_int(const char *key);
	std::string& get_str(const char *key);
private:
	std::map<std::string, bool> bool_map;
	std::map<std::string, int> int_map;
	std::map<std::string, std::string> str_map;
};

#endif
