#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "net.h"
#include "md5.h"
#include "printdata.h"
#include "codes.h"

#include <string>

struct reply {
	std::string daemonStamp;
} cmd_reply;

static GSList* cmd_list = NULL;

static const char separator = 0x01;

static void arg_escape(std::string &earg, const char *arg)
{
	earg.clear();
	while (*arg) {
		if (*arg=='\\') {
			earg+="\\\\";
		} else if (*arg==' ') {
			earg+="\\ ";
		} else if (*arg=='\n') {
			earg+="\\n";
		} else {
			earg+=*arg;
		}
		arg++;
	}
}

struct cmd *make_command( int command, ... )
{
	struct cmd *c = (struct cmd *)g_malloc( sizeof( struct cmd ) );
	c->command = command;
	va_list    ap;
	va_start( ap, command );
	switch (command) {
	case CMD_CLIENT:
	{
		const char *protocol_version = va_arg( ap, const char * );
		const char *client_name = va_arg( ap, const char * );
		std::string earg1, earg2;
		arg_escape(earg1, protocol_version);
		arg_escape(earg2, client_name);
		c->data = g_strdup_printf("client %s %s\n", earg1.c_str(), earg2.c_str());
		break;
	}
	case CMD_REGISTER:
	{
		int need_md5 = va_arg( ap, int );
		const char *user = va_arg( ap, const char * );
		const char *passwd = va_arg( ap, const char * );
		const char *email = va_arg( ap, const char * );
		char hex[33];
		if (need_md5) {
			struct MD5Context ctx;
			MD5Init(&ctx);
			MD5Update(&ctx, (const unsigned char*)passwd, strlen(passwd));
			unsigned char digest[16];
			MD5Final(digest, &ctx);
			for (int i = 0; i < 16; i++)
				snprintf( hex+2*i, 3, "%02x", digest[i] );
			hex[32] = '\0';
		}
		std::string earg1, earg2, earg3;
		arg_escape(earg1, user);
		if (need_md5)
			arg_escape(earg2, hex);
		else
			arg_escape(earg2, passwd);
		arg_escape(earg3, email);
		c->data = g_strdup_printf("register %s %s %s\n", earg1.c_str(), earg2.c_str(), earg3.c_str());
		break;
	}
	case CMD_CHANGE_PASSWD:
	{
		int need_md5 = va_arg( ap, int );
		const char *user = va_arg( ap, const char * );
		const char *old_passwd = va_arg( ap, const char * );
		const char *new_passwd = va_arg( ap, const char * );
		char hex[33];
		if (need_md5) {
			struct MD5Context ctx;
			MD5Init(&ctx);
			MD5Update(&ctx, (const unsigned char*)new_passwd, strlen(new_passwd));
			unsigned char digest[16];
			MD5Final(digest, &ctx);
			for (int i = 0; i < 16; i++)
				snprintf( hex+2*i, 3, "%02x", digest[i] );
			hex[32] = '\0';
		}
		std::string earg1, earg2, earg3;
		arg_escape(earg1, user);
		arg_escape(earg2, old_passwd);
		if (need_md5)
			arg_escape(earg3, hex);
		else
			arg_escape(earg3, new_passwd);
		c->data = g_strdup_printf("change_password %s %s %s\n", earg1.c_str(), earg2.c_str(), earg3.c_str());
		break;
	}
	case CMD_AUTH:
		c->auth.need_md5 = va_arg( ap, int );
		c->auth.user = va_arg( ap, const char * );
		c->auth.passwd = va_arg( ap, const char * );
		break;
	case CMD_LOOKUP:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("lookup %s\n", earg.c_str());
		break;
	}
	case CMD_PREVIOUS:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("previous %s\n", earg.c_str());
		break;
	}
	case CMD_NEXT:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("next %s\n", earg.c_str());
		break;
	}
	case CMD_QUERY:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("query %s\n", earg.c_str());
		break;
	}
	case CMD_SELECT_QUERY:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("selectquery %s\n", earg.c_str());
		break;
	}
	case CMD_DEFINE:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("define %s\n", earg.c_str());
		break;
	}
	case CMD_SET_DICT_MASK:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("setdictmask %s\n", earg.c_str());
		break;
	}
	case CMD_GET_DICT_MASK:
		c->data = g_strdup("getdictmask\n");
		break;
	case CMD_SET_COLLATE_FUNC:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("setcollatefunc %s\n", earg.c_str());
		break;
	}
	case CMD_GET_COLLATE_FUNC:
		c->data = g_strdup("getcollatefunc\n");
		break;
	case CMD_SET_LANGUAGE:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("setlanguage %s\n", earg.c_str());
		break;
	}
	case CMD_GET_LANGUAGE:
		c->data = g_strdup("getlanguage\n");
		break;
	case CMD_SET_EMAIL:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("setemail %s\n", earg.c_str());
		break;
	}
	case CMD_GET_EMAIL:
		c->data = g_strdup("getemail\n");
		break;
	case CMD_GET_USER_LEVEL:
		c->data = g_strdup("getuserlevel\n");
		break;
	case CMD_MAX_DICT_COUNT:
		c->data = g_strdup("maxdictcount\n");
		break;
	case CMD_DIR_INFO:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("dirinfo %s\n", earg.c_str());
		c->dirinfo_show_adddict = va_arg( ap, int );
		break;
	}
	case CMD_DICT_INFO:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("dictinfo %s\n", earg.c_str());
		break;
	}
	case CMD_DICTS_LIST:
	{
		std::string earg;
		arg_escape(earg, va_arg( ap, const char * ));
		c->data = g_strdup_printf("dictslist %s\n", earg.c_str());
		break;
	}
	case CMD_USER_LEVEL:
	{
		std::string earg1, earg2, earg3;
		arg_escape(earg1, va_arg( ap, const char * ));
		arg_escape(earg2, va_arg( ap, const char * ));
		arg_escape(earg3, va_arg( ap, const char * ));
		c->data = g_strdup_printf("userlevel %s %s %s\n", earg1.c_str(), earg2.c_str(), earg3.c_str());
		break;
	}
	case CMD_FROMTO:
	{
		c->data = g_strdup("fromto\n");
		break;
	}
	case CMD_COOKIE:
	{
		std::string earg;
		arg_escape(earg, va_arg(ap, const char *));
		c->data = g_strdup_printf("cookie %s\n", earg.c_str());
		break;
	}
	case CMD_QUIT:
		c->data = g_strdup("quit\n");
		break;
	}
	va_end( ap );
	return c;
}

void append_command(bool append_sep, struct cmd *c )
{
	c->append_sep = append_sep;
	cmd_list = g_slist_append(cmd_list, c);
}

void clean_command()
{
	struct cmd *c;
	GSList *list = cmd_list;
	while (list) {
		c = (struct cmd *)(list->data);
		switch (c->command) {
			case CMD_AUTH:
				break;
			default:
				g_free(c->data);
				break;
		}
		g_free(c);
		list = list->next;
	}
	g_slist_free(cmd_list);
}

bool request_command(struct cmd *c)
{
	switch (c->command) {
		case CMD_AUTH:
		{
			struct MD5Context ctx;
			unsigned char digest[16];
			char hex[33];
			int i;
			if (c->auth.need_md5) {
				MD5Init(&ctx);
				MD5Update(&ctx, (const unsigned char*)c->auth.passwd, strlen(c->auth.passwd));
				MD5Final(digest, &ctx );
				for (i = 0; i < 16; i++)
					sprintf( hex+2*i, "%02x", digest[i] );
				hex[32] = '\0';
			}
			MD5Init(&ctx);
			MD5Update(&ctx, (const unsigned char*)cmd_reply.daemonStamp.c_str(), cmd_reply.daemonStamp.length());
			if (c->auth.need_md5)
				MD5Update(&ctx, (const unsigned char*)hex, 32);
			else
				MD5Update(&ctx, (const unsigned char*)(c->auth.passwd), strlen(c->auth.passwd));
			MD5Final(digest, &ctx );
			for (i = 0; i < 16; i++)
				sprintf( hex+2*i, "%02x", digest[i] );
			hex[32] = '\0';
			std::string earg1, earg2;
			arg_escape(earg1, c->auth.user);
			arg_escape(earg2, hex);
			char *data = g_strdup_printf("auth %s %s\n", earg1.c_str(), earg2.c_str());
			if (net_write_str(data)) {
				g_free(data);
				return true;
			}
			g_free(data);
			break;
		}
		default:
			if (net_write_str(c->data))
				return true;
			break;
	}
	return false;
}

static bool process_banner()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_HELLO) {
		printf("process_banner:\n");
		if (status == CODE_TEMPORARILY_UNAVAILABLE) {
			printf("Server temporarily unavailable!\n");
		} else {
			printf("Unexpected status code %d\n", status);
		}
		return true;
	}
	const char *p;
	p = strrchr(buf.c_str(), ' ');
	if (p) {
		p++;
		cmd_reply.daemonStamp = p;
	}
	return false;
}

static bool process_command_register()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Register failed: %s\n", buf.c_str());
		return true;
	}
	return false;
}

static bool process_command_change_password()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Change password failed: %s\n", buf.c_str());
		return true;
	}
	return false;
}

static bool process_command_client()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Process command \"client\" failed: %s\n", buf.c_str());
		return true;
	}
	return false;
}

static bool process_command_auth()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Auth denied: %s\n", buf.c_str());
		return true;
	}
	return false;
}

static bool process_command_cookie()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Cookie denied: %s\n", buf.c_str());
		return true;
	}
	return false;
}

static bool print_dict_result(bool is_floating = false)
{
	std::string str;
	if (net_read_line(str))
		return true;
	int status;
	status = atoi(str.c_str());
	if (status != CODE_OK) {
		if (status == CODE_DICTMASK_NOTSET) {
			printf("You haven't choose your custom dictionries. <a href=\"dictmanage.php\">Choose</a> now!<br>");
		} else {
			printf("Query failed: %s\n", str.c_str());
		}
		return true;
	}
	std::string oword;
	if (net_read_str(oword))
		return true;
	std::string etext;
	if (is_floating) {
		escape_text(etext, oword.c_str());
		printf("<big><b>%s</b></big><br>", etext.c_str());
	}
	char *data;
	bool first = true;
	while (true) {
		if (net_read_str(str))
			return true;
		if (str.empty()) {
			if (first) {
				printf("<b>Not Found!</b><br>");
			}
			break;
		}
		if (first)
			first = false;
		escape_text(etext, str.c_str());
		printf("<font color=\"blue\">&lt;--- %s ---&gt;</font><br>", etext.c_str());
		while (true) {
			if (net_read_str(str))
				return true;
			if (str.empty())
				break;
			escape_text(etext, str.c_str());
			if (is_floating) {
				if (str!=oword)
					printf("<font color=\"purple\">%s</font><br>", etext.c_str());
			} else {
				printf("<big><b>%s</b></big><br>", etext.c_str());
			}
			if (net_read_data(&data))
				return true;
			print_data(data, str.c_str());
			g_free(data);
			printf("<br>");
			while (true) {
				if (net_read_data(&data))
					return true;
				if (*reinterpret_cast<guint32 *>(data) == 0)
					break;
				printf("<hr>");
				print_data(data, str.c_str());
				g_free(data);
				printf("<br>");
			}
		}
	}
	return false;
}

static bool process_command_lookup()
{
	if (print_dict_result())
		return true;
	fwrite(&separator, 1, 1, stdout);
	std::string str;
	if (net_read_str(str))
		return true;
	printf("%s", str.c_str());
	std::string etext;
	if (str == "d") {
		while (true) {
			if (net_read_str(str))
				return true;
			if (str.empty())
				break;
			escape_text(etext, str.c_str());
			printf("\n%s", etext.c_str());
			while (true) {
				if (net_read_str(str))
					return true;
				if (str.empty())
					break;
				printf("\t%s", str.c_str());
			}
		}
	} else {
		// str == "l" or "r" or "g" or "f".
		while (true) {
			if (net_read_str(str))
				return true;
			if (str.empty())
				break;
			printf("\n%s", str.c_str());
		}
	}
	return false;
}

static bool print_word_list()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		return true;
	}
	bool isfirst = true;
	while (true) {
		if (net_read_str(buf))
			return true;
		if (buf.empty())
			break;
		if (isfirst) {
			printf("%s", buf.c_str());
			isfirst = false;
		} else {
			printf("\n%s", buf.c_str());
		}
	}
	return false;
}

static bool process_command_previous()
{
	return print_word_list();
}

static bool process_command_next()
{
	return print_word_list();
}

static bool process_command_query()
{
	return print_dict_result();
}

static bool process_command_selectquery()
{
	return print_dict_result(true);
}

static bool process_command_define()
{
	return print_dict_result();
}

static bool process_command_setdictmask()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Set dictmask failed: %s\n", buf.c_str());
		return true;
	}
	return false;
}

static bool process_command_setcollatefunc()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Set collate func failed: %s\n", buf.c_str());
		return true;
	}
	printf("Update OK!");
	return false;
}

static bool process_command_setlanguage()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Set language failed: %s\n", buf.c_str());
		return true;
	}
	return false;
}

static bool process_command_setemail()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Set email failed: %s\n", buf.c_str());
		return true;
	}
	printf("Update OK!");
	return false;
}

struct dictmask_ParseUserData {
	std::string *html_str;
	std::string uid;
	std::string bookname;
	std::string wordcount;
};

static void dictmask_parse_start_element(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer user_data, GError **error)
{
	if (strcmp(element_name, "dict")==0) {
		dictmask_ParseUserData *Data = (dictmask_ParseUserData *)user_data;
		Data->uid.clear();
		Data->bookname.clear();
		Data->wordcount.clear();
	}
}

static void dictmask_parse_end_element(GMarkupParseContext *context, const gchar *element_name, gpointer user_data, GError **error)
{
	if (strcmp(element_name, "dict")==0) {
		dictmask_ParseUserData *Data = (dictmask_ParseUserData *)user_data;
		Data->html_str->append("<option id=\"");
		Data->html_str->append(Data->uid);
		Data->html_str->append("\" value=\"");
		Data->html_str->append(Data->uid);
		Data->html_str->append("\">");
		std::string etext;
		escape_text(etext, Data->bookname.c_str());
		Data->html_str->append(etext);
		Data->html_str->append(" ");
		Data->html_str->append(Data->wordcount);
		Data->html_str->append("</option>");
	}
}

static void dictmask_parse_text(GMarkupParseContext *context, const gchar *text, gsize text_len, gpointer user_data, GError **error)
{
	const gchar *element = g_markup_parse_context_get_element(context);
	if (!element)
		return;
	dictmask_ParseUserData *Data = (dictmask_ParseUserData *)user_data;
	if (strcmp(element, "uid")==0) {
		Data->uid.assign(text, text_len);
	} else if (strcmp(element, "bookname")==0) {
		Data->bookname.assign(text, text_len);
	} else if (strcmp(element, "wordcount")==0) {
		Data->wordcount.assign(text, text_len);
	}
}

static bool process_command_getdictmask()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Get dictmask failed: %s\n", buf.c_str());
		return true;
	}
	if (net_read_str(buf))
		return true;
	std::string html_str;
	dictmask_ParseUserData Data;
	Data.html_str = &html_str;
	GMarkupParser parser;
	parser.start_element = dictmask_parse_start_element;
	parser.end_element = dictmask_parse_end_element;
	parser.text = dictmask_parse_text;
	parser.passthrough = NULL;
	parser.error = NULL;
	GMarkupParseContext* context = g_markup_parse_context_new(&parser, (GMarkupParseFlags)0, &Data, NULL);
	g_markup_parse_context_parse(context, buf.c_str(), buf.length(), NULL);
	g_markup_parse_context_end_parse(context, NULL);
	g_markup_parse_context_free(context);
	printf("%s", html_str.c_str());
	return false;
}

static bool process_command_maxdictcount()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Get max dict count failed: %s\n", buf.c_str());
		return true;
	}
	if (net_read_str(buf))
		return true;
	printf("%s", buf.c_str());
	return false;
}

static bool process_command_getcollatefunc()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		return true;
	}
	if (net_read_str(buf))
		return true;
	printf("%s", buf.c_str());
	return false;
}

static bool process_command_getlanguage()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		return true;
	}
	if (net_read_str(buf))
		return true;
	printf("%s", buf.c_str());
	return false;
}

static bool process_command_getemail()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		return true;
	}
	if (net_read_str(buf))
		return true;
	printf("%s", buf.c_str());
	return false;
}

static bool process_command_getuserlevel()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		return true;
	}
	if (net_read_str(buf))
		return true;
	printf("%s", buf.c_str());
	return false;
}

struct dirinfo_ParseUserData {
	bool dirinfo_show_adddict;
	std::string *html_str;
	std::string parent;
	unsigned int userlevel;
	bool in_dir;
	std::string dir_name;
	std::string dir_dirname;
	std::string dir_dictcount;
	bool in_dict;
	std::string dict_islink;
	std::string dict_level;
	std::string dict_uid;
	std::string dict_bookname;
	std::string dict_wordcount;
};

static void dirinfo_parse_start_element(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer user_data, GError **error)
{
	if (strcmp(element_name, "dir")==0) {
		dirinfo_ParseUserData *Data = (dirinfo_ParseUserData *)user_data;
		Data->in_dir = true;
		Data->dir_name.clear();
		Data->dir_dirname.clear();
		Data->dir_dictcount.clear();
	} else if (strcmp(element_name, "dict")==0) {
		dirinfo_ParseUserData *Data = (dirinfo_ParseUserData *)user_data;
		Data->in_dict = true;
		Data->dict_islink.clear();
		Data->dict_level.clear();
		Data->dict_uid.clear();
		Data->dict_bookname.clear();
		Data->dict_wordcount.clear();
	}
}

static void dirinfo_parse_end_element(GMarkupParseContext *context, const gchar *element_name, gpointer user_data, GError **error)
{
	if (strcmp(element_name, "dir")==0) {
		dirinfo_ParseUserData *Data = (dirinfo_ParseUserData *)user_data;
		Data->in_dir = false;
		Data->html_str->append("<li id=\"");
		Data->html_str->append(Data->parent);
		Data->html_str->append(Data->dir_name);
		Data->html_str->append(G_DIR_SEPARATOR_S);
		Data->html_str->append("\"><img src=\"images/plus.gif\" OnClick=\"getSubTree('");
		Data->html_str->append(Data->parent);
		Data->html_str->append(Data->dir_name);
		Data->html_str->append(G_DIR_SEPARATOR_S);
		Data->html_str->append("')\"><img src=\"images/folder.gif\"><a href=\"#\" onClick=\"getSubTree('");
		Data->html_str->append(Data->parent);
		Data->html_str->append(Data->dir_name);
		Data->html_str->append(G_DIR_SEPARATOR_S);
		Data->html_str->append("')\">");
		Data->html_str->append(Data->dir_dirname);
		Data->html_str->append(" (");
		Data->html_str->append(Data->dir_dictcount);
		Data->html_str->append(")");
		Data->html_str->append("</a></li>");
	} else if (strcmp(element_name, "dict")==0) {
		dirinfo_ParseUserData *Data = (dirinfo_ParseUserData *)user_data;
		std::string ebookname;
		std::string eebookname;
		escape_text(ebookname, Data->dict_bookname.c_str());
		escape_text(eebookname, ebookname.c_str());
		Data->in_dict = false;
		Data->html_str->append("<li><img src=\"images/blank.gif\">");
		if (Data->dirinfo_show_adddict) {
			unsigned int dict_level;
			if (Data->dict_level.empty())
				dict_level = 0;
			else
				dict_level = atoi(Data->dict_level.c_str());
			if (Data->userlevel < dict_level) {
				Data->html_str->append("<input type=\"button\" value=\"$\" onClick=\"showNeedLevel()\">");
			} else {
				Data->html_str->append("<input type=\"button\" value=\"+\" onClick=\"addDict('");
				Data->html_str->append(Data->dict_uid);
				Data->html_str->append("','");
				Data->html_str->append(eebookname);
				Data->html_str->append("','");
				Data->html_str->append(Data->dict_wordcount);
				Data->html_str->append("')\">");
			}
		}
		if (Data->dict_islink == "1")
			Data->html_str->append("<img src=\"images/linkdict.png\"><a href=\"#\" onClick=\"showDictInfo('");
		else
			Data->html_str->append("<img src=\"images/sheet.gif\"><a href=\"#\" onClick=\"showDictInfo('");
		Data->html_str->append(Data->dict_uid);
		Data->html_str->append("')\">");
		Data->html_str->append(ebookname);
		Data->html_str->append("</a> ");
		Data->html_str->append(Data->dict_wordcount);
		Data->html_str->append("</li>");
	}
}

static void dirinfo_parse_text(GMarkupParseContext *context, const gchar *text, gsize text_len, gpointer user_data, GError **error)
{
	const gchar *element = g_markup_parse_context_get_element(context);
	if (!element)
		return;
	dirinfo_ParseUserData *Data = (dirinfo_ParseUserData *)user_data;
	if (strcmp(element, "parent")==0) {
		Data->parent.assign(text, text_len);
	} else if (strcmp(element, "userlevel")==0) {
		std::string userlevel(text, text_len);
		Data->userlevel = atoi(userlevel.c_str());
	} else if (strcmp(element, "name")==0) {
		if (Data->in_dir)
			Data->dir_name.assign(text, text_len);
	} else if (strcmp(element, "dirname")==0) {
		if (Data->in_dir)
			Data->dir_dirname.assign(text, text_len);
	} else if (strcmp(element, "dictcount")==0) {
		if (Data->in_dir)
			Data->dir_dictcount.assign(text, text_len);
	} else if (strcmp(element, "islink")==0) {
		if (Data->in_dict)
			Data->dict_islink.assign(text, text_len);
	} else if (strcmp(element, "level")==0) {
		if (Data->in_dict)
			Data->dict_level.assign(text, text_len);
	} else if (strcmp(element, "uid")==0) {
		if (Data->in_dict)
			Data->dict_uid.assign(text, text_len);
	} else if (strcmp(element, "bookname")==0) {
		if (Data->in_dict)
			Data->dict_bookname.assign(text, text_len);
	} else if (strcmp(element, "wordcount")==0) {
		if (Data->in_dict)
			Data->dict_wordcount.assign(text, text_len);
	}
}

static bool process_command_dirinfo(bool dirinfo_show_adddict)
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Get dirinfo failed: %s\n", buf.c_str());
		return true;
	}
	if (net_read_str(buf))
		return true;
	std::string html_str;
	dirinfo_ParseUserData Data;
	Data.dirinfo_show_adddict = dirinfo_show_adddict;
	Data.html_str = &html_str;
	Data.userlevel = 0;
	Data.in_dir = false;
	Data.in_dict = false;
	GMarkupParser parser;
	parser.start_element = dirinfo_parse_start_element;
	parser.end_element = dirinfo_parse_end_element;
	parser.text = dirinfo_parse_text;
	parser.passthrough = NULL;
	parser.error = NULL;
	GMarkupParseContext* context = g_markup_parse_context_new(&parser, (GMarkupParseFlags)0, &Data, NULL);
	g_markup_parse_context_parse(context, buf.c_str(), buf.length(), NULL);
	g_markup_parse_context_end_parse(context, NULL);
	g_markup_parse_context_free(context);
	printf("%s", html_str.c_str());
	return false;
}

/*struct DictInfo_Data {
		std::string uid;
		std::string bookname;
};
struct ToInfo_Data {
		std::string to;
		std::list<DictInfo_Data> dictData;
};
struct From_Data {
		std::string from;
		std::list<ToInfo_Data> toData;
};
struct FromToDict{
	struct From_Data from_data;
	struct ToInfo_Data toinfo_data;
	struct DictInfo_Data dictinfo_data;
	std::list<From_Data> list_fromto;
};


static void fromtoinfo_parse_start_element(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer user_data, GError **error)
{
	FromToDict *fromtodict = (FromToDict *)user_data;
	if (strcmp(element_name, "from")==0) {
		fromtodict->from_data.from.clear();
		fromtodict->from_data.toData.clear();
		int i = 0;
		while (attribute_names[i]) {
			if (strcmp(attribute_names[i], "lang")==0) {
				fromtodict->from_data.from = attribute_values[i];
				break;
			}
			i++;
		}
	} else if(strcmp(element_name, "to")==0) {
		fromtodict->toinfo_data.to.clear();
		fromtodict->toinfo_data.dictData.clear();
		int i = 0;
		while (attribute_names[i]) {
			if (strcmp(attribute_names[i], "lang")==0) {
				fromtodict->toinfo_data.to = attribute_values[i];
				break;
			}
			i++;
		}
	} else if (strcmp(element_name, "dict")==0) {
		fromtodict->dictinfo_data.uid.clear();
		fromtodict->dictinfo_data.bookname.clear();
	}
}

static void fromtoinfo_parse_end_element(GMarkupParseContext *context, const gchar *element_name, gpointer user_data, GError **error)
{
	FromToDict *fromtodict = (FromToDict *)user_data;
	if (strcmp(element_name, "from")==0) {
		fromtodict->list_fromto.push_back(fromtodict->from_data);
	} else if(strcmp(element_name, "to")==0) {
		fromtodict->from_data.toData.push_back(fromtodict->toinfo_data);
	} else if(strcmp(element_name, "dict")==0) {
		fromtodict->toinfo_data.dictData.push_back(fromtodict->dictinfo_data);
	}
}

static void fromtoinfo_parse_text(GMarkupParseContext *context, const gchar *text, gsize text_len, gpointer user_data, GError **error)
{
	const gchar *element = g_markup_parse_context_get_element(context);
	if (!element)
		return;
	FromToDict *fromtodict = (FromToDict *)user_data;
	if (strcmp(element, "uid")==0) {
		fromtodict->dictinfo_data.uid.assign(text, text_len);
	} else if (strcmp(element, "bookname")==0) {
		fromtodict->dictinfo_data.bookname.assign(text, text_len);
	}	
}*/

static bool process_command_fromtoinfo()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Get fromtoinfo failed: %s\n", buf.c_str());
		return true;
	}
	if (net_read_str(buf))
		return true;
	printf("%s", buf.c_str());
	/*std::string html_str;
	FromToDict Data;
	GMarkupParser parser;
	parser.start_element = fromtoinfo_parse_start_element;
	parser.end_element = fromtoinfo_parse_end_element;
	parser.text = fromtoinfo_parse_text;
	parser.passthrough = NULL;
	parser.error = NULL;
	GMarkupParseContext* context = g_markup_parse_context_new(&parser, (GMarkupParseFlags)0, &Data, NULL);
	g_markup_parse_context_parse(context, buf.c_str(), buf.length(), NULL);
	g_markup_parse_context_end_parse(context, NULL);
	g_markup_parse_context_free(context);
	for(std::list<From_Data>::iterator i= Data.list_fromto.begin(); i != Data.list_fromto.end(); ++i){
		html_str += i->from;
		std::list<ToInfo_Data> &toData = i->toData;
		for(std::list<ToInfo_Data>::iterator j = toData.begin(); j!=toData.end(); j++){
			html_str += j->to;
			std::list<DictInfo_Data> &dictData = j->dictData;
			for(std::list<DictInfo_Data>::iterator l = dictData.begin(); l != dictData.end(); ++l){
				html_str += l->uid;
				html_str += l->bookname;
			}
		}
	}
	printf("%s", html_str.c_str());*/
	return false;
}

struct dictinfo_ParseUserData {
	std::string *html_str;
	std::string dictinfo_bookname;
	std::string dictinfo_wordcount;
	std::string dictinfo_synwordcount;
	std::string dictinfo_author;
	std::string dictinfo_email;
	std::string dictinfo_website;
	std::string dictinfo_description;
	std::string dictinfo_date;
	std::string dictinfo_download;
};

static void dictinfo_parse_start_element(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer user_data, GError **error)
{
	if (strcmp(element_name, "dictinfo")==0) {
		dictinfo_ParseUserData *Data = (dictinfo_ParseUserData *)user_data;
		Data->dictinfo_bookname.clear();
		Data->dictinfo_wordcount.clear();
		Data->dictinfo_synwordcount.clear();
		Data->dictinfo_author.clear();
		Data->dictinfo_email.clear();
		Data->dictinfo_website.clear();
		Data->dictinfo_description.clear();
		Data->dictinfo_date.clear();
		Data->dictinfo_download.clear();
	}
}

static void dictinfo_parse_end_element(GMarkupParseContext *context, const gchar *element_name, gpointer user_data, GError **error)
{
	if (strcmp(element_name, "dictinfo")==0) {
		dictinfo_ParseUserData *Data = (dictinfo_ParseUserData *)user_data;
		std::string etext;
		Data->html_str->append("<b>Book Name: </b>");
		escape_text(etext, Data->dictinfo_bookname.c_str());
		Data->html_str->append(etext);
		Data->html_str->append("<br><b>Word Count: </b>");
		Data->html_str->append(Data->dictinfo_wordcount);
		if (!Data->dictinfo_synwordcount.empty()) {
			Data->html_str->append("<br><b>Synonym count: </b>");
			Data->html_str->append(Data->dictinfo_synwordcount);
		}
		Data->html_str->append("<br><b>Author: </b>");
		escape_text(etext, Data->dictinfo_author.c_str());
		Data->html_str->append(etext);
		Data->html_str->append("<br><b>Email: </b>");
		escape_text(etext, Data->dictinfo_email.c_str());
		Data->html_str->append(etext);
		Data->html_str->append("<br><b>Website: </b>");
		escape_text(etext, Data->dictinfo_website.c_str());
		Data->html_str->append(etext);
		Data->html_str->append("<br><b>Description: </b>");
		escape_text(etext, Data->dictinfo_description.c_str());
		Data->html_str->append(etext);
		Data->html_str->append("<br><b>Date: </b>");
		escape_text(etext, Data->dictinfo_date.c_str());
		Data->html_str->append(etext);
		if (!Data->dictinfo_download.empty()) {
			Data->html_str->append("<br><br><a href=\"");
			escape_text(etext, Data->dictinfo_download.c_str());
			Data->html_str->append(etext);
			Data->html_str->append("\" target=\"_blank\">Download Now!</a>");
		}
	}
}

static void dictinfo_parse_text(GMarkupParseContext *context, const gchar *text, gsize text_len, gpointer user_data, GError **error)
{
	const gchar *element = g_markup_parse_context_get_element(context);
	if (!element)
		return;
	dictinfo_ParseUserData *Data = (dictinfo_ParseUserData *)user_data;
	if (strcmp(element, "bookname")==0) {
		Data->dictinfo_bookname.assign(text, text_len);
	} else if (strcmp(element, "wordcount")==0) {
		Data->dictinfo_wordcount.assign(text, text_len);
	} else if (strcmp(element, "synwordcount")==0) {
		Data->dictinfo_synwordcount.assign(text, text_len);
	} else if (strcmp(element, "author")==0) {
		Data->dictinfo_author.assign(text, text_len);
	} else if (strcmp(element, "email")==0) {
		Data->dictinfo_email.assign(text, text_len);
	} else if (strcmp(element, "website")==0) {
		Data->dictinfo_website.assign(text, text_len);
	} else if (strcmp(element, "description")==0) {
		Data->dictinfo_description.assign(text, text_len);
	} else if (strcmp(element, "date")==0) {
		Data->dictinfo_date.assign(text, text_len);
	} else if (strcmp(element, "download")==0) {
		Data->dictinfo_download.assign(text, text_len);
	}
}

static bool process_command_dictinfo()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Get dictinfo failed: %s\n", buf.c_str());
		return true;
	}
	if (net_read_str(buf))
		return true;
	std::string html_str;
	dictinfo_ParseUserData Data;
	Data.html_str = &html_str;
	GMarkupParser parser;
	parser.start_element = dictinfo_parse_start_element;
	parser.end_element = dictinfo_parse_end_element;
	parser.text = dictinfo_parse_text;
	parser.passthrough = NULL;
	parser.error = NULL;
	GMarkupParseContext* context = g_markup_parse_context_new(&parser, (GMarkupParseFlags)0, &Data, NULL);
	g_markup_parse_context_parse(context, buf.c_str(), buf.length(), NULL);
	g_markup_parse_context_end_parse(context, NULL);
	g_markup_parse_context_free(context);
	printf("%s", html_str.c_str());
	return false;
}

static bool process_command_userlevel()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_OK) {
		printf("Set user level failed: %s\n", buf.c_str());
		return true;
	}
	printf("Set user level success!\n");
	return false;
}

static bool process_command_quit()
{
	std::string buf;
	if (net_read_line(buf))
		return true;
	int status;
	status = atoi(buf.c_str());
	if (status != CODE_GOODBYE) {
		printf("process_command_quit:\n");
		printf("Unexpected status code %d\n", status);
		return true;
	}
	return false;
}

void process_command()
{
	if (process_banner())
		return;
	struct cmd *c;
	GSList *list = cmd_list;
	while (list) {
		c = (struct cmd *)(list->data);
		if (request_command(c)) {
			printf("Sent request command failed!\n");
			return;
		}
		switch (c->command) {
		case CMD_CLIENT:
			if (process_command_client())
				return;
			break;
		case CMD_REGISTER:
			if (process_command_register())
				return;
			break;
		case CMD_COOKIE:
			if(process_command_cookie())
				return;
			break;
		case CMD_CHANGE_PASSWD:
			if (process_command_change_password())
				return;
			break;
		case CMD_SET_DICT_MASK:
			if (process_command_setdictmask())
				return;
			break;
		case CMD_GET_DICT_MASK:
		case CMD_DICTS_LIST:
			if (process_command_getdictmask())
				return;
			break;
		case CMD_SET_COLLATE_FUNC:
			if (process_command_setcollatefunc())
				return;
			break;
		case CMD_GET_COLLATE_FUNC:
			if (process_command_getcollatefunc())
				return;
			break;
		case CMD_SET_LANGUAGE:
			if (process_command_setlanguage())
				return;
			break;
		case CMD_GET_LANGUAGE:
			if (process_command_getlanguage())
				return;
			break;
		case CMD_SET_EMAIL:
			if (process_command_setemail())
				return;
			break;
		case CMD_GET_EMAIL:
			if (process_command_getemail())
				return;
			break;
		case CMD_GET_USER_LEVEL:
			if (process_command_getuserlevel())
				return;
			break;
		case CMD_MAX_DICT_COUNT:
			if (process_command_maxdictcount())
				return;
			break;
		case CMD_DIR_INFO:
			if (process_command_dirinfo(c->dirinfo_show_adddict))
				return;
			break;
		case CMD_FROMTO:
			if (process_command_fromtoinfo())
				return;
			break;
		case CMD_DICT_INFO:
			if (process_command_dictinfo())
				return;
			break;
		case CMD_USER_LEVEL:
			if (process_command_userlevel())
				return;
			break;
		case CMD_AUTH:
			if (process_command_auth())
				return;
			break;
		case CMD_LOOKUP:
			if (process_command_lookup())
				return;
			break;
		case CMD_PREVIOUS:
			if (process_command_previous())
				return;
			break;
		case CMD_NEXT:
			if (process_command_next())
				return;
			break;
		case CMD_QUERY:
			if (process_command_query())
				return;
			break;
		case CMD_SELECT_QUERY:
			if (process_command_selectquery())
				return;
			break;
		case CMD_DEFINE:
			if (process_command_define())
				return;
			break;
		case CMD_QUIT:
			if (process_command_quit())
				return;
			break;
		}
		if (c->append_sep) {
			fwrite(&separator, 1, 1, stdout);
		}
		list = list->next;
	}
}
