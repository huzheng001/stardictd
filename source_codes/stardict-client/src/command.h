#ifndef _COMMAND_H_
#define _COMMAND_H_

enum {
	CMD_CLIENT,
	CMD_AUTH,
	CMD_LOOKUP,
	CMD_PREVIOUS,
	CMD_NEXT,
	CMD_QUERY,
	CMD_SELECT_QUERY,
	CMD_DEFINE,
	CMD_REGISTER,
	CMD_CHANGE_PASSWD,
	CMD_SET_DICT_MASK,
	CMD_GET_DICT_MASK,
	CMD_SET_COLLATE_FUNC,
	CMD_GET_COLLATE_FUNC,
	CMD_SET_LANGUAGE,
	CMD_GET_LANGUAGE,
	CMD_SET_EMAIL,
	CMD_GET_EMAIL,
	CMD_MAX_DICT_COUNT,
	CMD_DIR_INFO,
	CMD_DICT_INFO,
	CMD_USER_LEVEL,
	CMD_GET_USER_LEVEL,
	CMD_DICTS_LIST,
	CMD_FROMTO,
	CMD_COOKIE,
	CMD_QUIT,
};

struct cmd {
	int command;
	bool append_sep;
	union {
		char *data;
		struct {
			const char *user;
			const char *passwd;
			int need_md5;
		} auth;
	};
	union {
		bool dirinfo_show_adddict;
	};
};

extern struct cmd *make_command( int command, ... );
extern void append_command(bool append_sep, struct cmd *c );
extern bool request_command(struct cmd *c);
extern void process_command();
extern void clean_command();

#endif
