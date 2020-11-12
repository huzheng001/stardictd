
#include "conf.h"
#include <stdio.h>
#include <glib.h>

Conf::Conf()
{
	int_map["server/process_limit"] = 300;
	int_map["server/port"] = 2629;
	int_map["server/log_level"] = 4; // LOG_INFORMATION
	str_map["database/host"] = "localhost";
	int_map["database/port"] = 3306;
	str_map["database/user"] = "stardict";
	str_map["database/password"] = ""; //Must set in stardictd.conf
	str_map["database/dbname"] = "stardict";
	int_map["client/delay_time"] = 7;
	int_map["client/max_cost_time"] = 30;
	str_map["info/ad_info"] = "<title>Upgrade Now!</title><url>http://www.stardict.org/finance.php</url>";
	str_map["guest-user/dictmask"] = ""; //Must set in stardictd.conf
	bool_map["level-0-user/enable_collate"] = false;
	int_map["level-0-user/max_dict_count"] = 8;
	int_map["level-1-user/max_dict_count"] = 15;
	int_map["level-2-user/max_dict_count"] = 25;
	int_map["level-3-user/max_dict_count"] = 35;
	int_map["level-4-user/max_dict_count"] = 45;
	int_map["level-5-user/max_dict_count"] = 55;
	str_map["root-user/password"] = ""; //Must set in stardictd.conf

	GKeyFile *keyfile = g_key_file_new();
	std::string path = g_get_home_dir();
	path += G_DIR_SEPARATOR_S "stardictd.conf";
	if (!g_key_file_load_from_file(keyfile, path.c_str(), G_KEY_FILE_NONE, NULL)) {
		printf("Load %s failed!\n", path.c_str());
		g_key_file_free(keyfile);
		return;
	}
	GError *err;
	int n;
	err = NULL;
	n = g_key_file_get_integer(keyfile, "server", "process_limit", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["server/process_limit"] = n;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "server", "port", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["server/port"] = n;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "server", "log_level", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["server/log_level"] = n;
	}
	char *str;
	str = g_key_file_get_string(keyfile, "database", "host", NULL);
	if (str) {
		str_map["database/host"] = str;
		g_free(str);
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "database", "port", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["database/port"] = n;
	}
	str = g_key_file_get_string(keyfile, "database", "user", NULL);
	if (str) {
		str_map["database/user"] = str;
		g_free(str);
	}
	str = g_key_file_get_string(keyfile, "database", "password", NULL);
	if (str) {
		str_map["database/password"] = str;
		g_free(str);
	}
	str = g_key_file_get_string(keyfile, "database", "dbname", NULL);
	if (str) {
		str_map["database/dbname"] = str;
		g_free(str);
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "client", "delay_time", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["client/delay_time"] = n;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "client", "max_cost_time", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["client/max_cost_time"] = n;
	}
	str = g_key_file_get_string(keyfile, "info", "ad_info", NULL);
	if (str) {
		str_map["info/ad_info"] = str;
		g_free(str);
	}
	str = g_key_file_get_string(keyfile, "guest-user", "dictmask", NULL);
	if (str) {
		str_map["guest-user/dictmask"] = str;
		g_free(str);
	}
	gboolean b;
	err = NULL;
	b = g_key_file_get_boolean(keyfile, "level-0-user", "enable_collate", &err);
	if (err) {
		g_error_free (err);
	} else {
		bool_map["level-0-user/enable_collate"] = b;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "level-0-user", "max_dict_count", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["level-0-user/max_dict_count"] = n;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "level-1-user", "max_dict_count", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["level-1-user/max_dict_count"] = n;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "level-2-user", "max_dict_count", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["level-2-user/max_dict_count"] = n;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "level-3-user", "max_dict_count", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["level-3-user/max_dict_count"] = n;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "level-4-user", "max_dict_count", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["level-4-user/max_dict_count"] = n;
	}
	err = NULL;
	n = g_key_file_get_integer(keyfile, "level-5-user", "max_dict_count", &err);
	if (err) {
		g_error_free (err);
	} else {
		int_map["level-5-user/max_dict_count"] = n;
	}
	str = g_key_file_get_string(keyfile, "root-user", "password", NULL);
	if (str) {
		str_map["root-user/password"] = str;
		g_free(str);
	}
	g_key_file_free(keyfile);
}

bool Conf::get_bool(const char *key)
{
	return bool_map[key];
}

int Conf::get_int(const char *key)
{
	return int_map[key];
}

std::string& Conf::get_str(const char *key)
{
	return str_map[key];
}

