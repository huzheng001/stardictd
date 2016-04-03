#include "daemon.h"
#include "net.h"
#include "md5.h"
#include "stardictdmain.h"

#include <glib.h>

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <alloca.h>
#include "codes.h"

#define BUFFERSIZE 10240

static char         daemonStamp[256] = "";
static std::string auth_user;
static std::string userID;
static int userLevel = 0;

static void daemon_printf( const char *format, ... )
{
	va_list ap;
	char buf[BUFFERSIZE];

	va_start( ap, format );
	vsnprintf( buf, sizeof (buf), format, ap );
	va_end( ap );
	net_write(buf, strlen(buf));
}

static void daemon_ok( int code, const char *string)
{
	daemon_printf("%d %s\n", code, string);
}

static void AppendMysql(std::string &sql, const char *str)
{
	sql += '\'';
	int len = strlen(str);
	char *buf = (char *)g_malloc(2*len+1);
	stardictdMain.conn.escapeString(buf, str, len);
	sql += buf;
	g_free(buf);
	sql += '\'';
}

static bool init_database()
{
	const char *db_host = stardictdMain.conf->get_str("database/host").c_str();
	const int db_port = stardictdMain.conf->get_int("database/port");
	const char *db_user = stardictdMain.conf->get_str("database/user").c_str();
	const char *db_password = stardictdMain.conf->get_str("database/password").c_str();
	const char *db_dbname = stardictdMain.conf->get_str("database/dbname").c_str();
	if (stardictdMain.conn.init(db_host, db_user, db_password, db_dbname, db_port))
		return true;
	return false;
}

static void daemon_auth(std::string &user, std::string &key)
{
	if (user == "guest") {
		daemon_printf( "%d\n", CODE_OK );
		return;
	}
	char *buf;
	struct MD5Context ctx;
	unsigned char digest[16];
	char hex[33];
	if (user == "root") {
		std::string &root_password = stardictdMain.conf->get_str("root-user/password");
		if (root_password.empty()) {
			daemon_printf( "%d auth denied\n", CODE_DENIED );
			return;
		}
		MD5Init(&ctx);
		MD5Update(&ctx,  (const unsigned char*)"StarDict", 8); //StarDict-Protocol 0.4, add md5 salt.
		MD5Update(&ctx, (const unsigned char*)root_password.c_str(), root_password.length());
		MD5Final(digest, &ctx);
		for (int i = 0; i < 16; i++)
			snprintf( hex+2*i, 3, "%02x", digest[i] );
		hex[32] = '\0';
		buf = g_strdup_printf("%s%s", daemonStamp, hex);
	} else {
		if (init_database()) {
			daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
			return;
		}
		std::string sql;
		sql = "SELECT user_id, user_md5saltpassword, level FROM stardict_users WHERE username=";
		AppendMysql(sql, user.c_str());
		if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
			daemon_printf( "%d Query failed!\n", CODE_DENIED );
			return;
		}
		MySQLResult *res = stardictdMain.conn.getResult();
		if (!res) {
			daemon_printf( "%d Get result failed!\n", CODE_DENIED );
			return;
		}
		DB_ROW row = res->fetchRow();
		if (!row) {
			res->destroy();
			daemon_printf( "%d User doesn't exist!\n", CODE_DENIED );
			return;
		}
		userID = row[0];
		userLevel = atoi(row[2]);
		stardictdMain.SetUserLevel(userLevel);
		buf = g_strdup_printf("%s%s", daemonStamp, row[1]);
		res->destroy();
	}
	MD5Init(&ctx);
	MD5Update(&ctx, (const unsigned char*)buf, strlen(buf));
	MD5Final(digest, &ctx);
	for (int i = 0; i < 16; i++)
		snprintf( hex+2*i, 3, "%02x", digest[i] );
	hex[32] = '\0';
	g_free(buf);
	if (key != hex) {
		daemon_printf( "%d auth denied\n", CODE_DENIED );
		return;
	} else {
		daemon_printf( "%d authenticated\n", CODE_OK );
		auth_user=user;
		if (auth_user == "root") {
			stardictdMain.SetDictMask("", 0, 0);
		} else {
			std::string sql;
			sql = "SELECT dictmask, collatefunc, language FROM user_basic WHERE id=";
			sql+= userID;
			if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
				daemon_printf( "%d Query failed!\n", CODE_DENIED );
				return;
			}
			MySQLResult *res = stardictdMain.conn.getResult();
			if (!res) {
				daemon_printf( "%d Get result failed!\n", CODE_DENIED );
				return;
			}
			DB_ROW row = res->fetchRow();
			if (row) {
				char *str = g_strdup_printf("level-%d-user/max_dict_count", userLevel);
				const int max_dict_count = stardictdMain.conf->get_int(str);
				g_free(str);
				stardictdMain.SetDictMask(row[0], max_dict_count, userLevel);
				bool enable_collate;
				if (userLevel == 0) {
					enable_collate = stardictdMain.conf->get_bool("level-0-user/enable_collate");
				} else {
					enable_collate = true;
				}
				if (enable_collate) {
					stardictdMain.SetServerCollateFunc(atoi(row[1]));
				} else {
					stardictdMain.SetServerCollateFunc(0);
				}
				stardictdMain.SetUserLang(row[2]);
			} else {
				stardictdMain.SetDictMask("", 0, 0);
				stardictdMain.SetServerCollateFunc(0);
			}
			res->destroy();
		}
	}
}

static void daemon_change_password(std::string &user, std::string &old_passwd, std::string &new_passwd)
{
	if (new_passwd.length()!=32) {
		daemon_printf( "%d wrong new password\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	if (auth_user != "root") {
		sql = "SELECT user_md5saltpassword FROM stardict_users WHERE username=";
		AppendMysql(sql, user.c_str());
		if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
			daemon_printf( "%d Query failed!\n", CODE_DENIED );
			return;
		}
		MySQLResult *res = stardictdMain.conn.getResult();
		if (!res) {
			daemon_printf( "%d Get result failed!\n", CODE_DENIED );
			return;
		}
		DB_ROW row = res->fetchRow();
		if (!row) {
			res->destroy();
			daemon_printf( "%d user doesn't exist\n", CODE_DENIED );
			return;
		}
		if (old_passwd != row[0]) {
			res->destroy();
			daemon_printf( "%d old password is wrong\n", CODE_DENIED );
			return;
		}
		res->destroy();
	}
	sql = "UPDATE stardict_users SET user_md5saltpassword=";
	AppendMysql(sql, new_passwd.c_str());
	sql += " WHERE username=";
	AppendMysql(sql, user.c_str());
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d Query failed!\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
}

static void daemon_register(std::string &user, std::string &password, std::string &email)
{
	if (user.length()>16 || password.length()!=32 || strchr(email.c_str(), '@')==NULL) {
		daemon_printf( "%d invalid parameter\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	// Original insert sql code.
	sql = "INSERT INTO stardict_users (username, user_md5saltpassword) VALUES(";
	AppendMysql(sql, user.c_str());
	sql += ",";
	AppendMysql(sql, password.c_str());
	sql += ")";
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d user already exists\n", CODE_DENIED );
		return;
	}
	stardictdMain.conn.lastInsertID(userID);
	sql = "INSERT INTO user_basic (id, register_time, email) VALUES(";
	sql += userID;
	sql += ",";
	gchar *register_time = g_strdup_printf("%u", (unsigned int)time(NULL));
	sql += register_time;
	g_free(register_time);
	sql += ",";
	AppendMysql(sql, email.c_str());
	sql += ")";
	std::string text;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		text = "Init user_basic table failed: ";
		text += sql;
		stardictdMain.logger->log(LOG_ERROR, text.c_str());
	}
	// Code to cooperate with phpBB3 forum.
	// It works in PROTOCOL_VERSION_0.3 ago, but don't works in PROTOCOL_VERSION_0.4 now. As change "user_password" to "user_md5saltpassword".
	//
	//
	/*gchar *register_time = g_strdup_printf("%u", (unsigned int)time(NULL));
	sql = "INSERT INTO `stardict_users`(`user_type`, `group_id`, `user_permissions`, `user_perm_from`, `user_ip`, `user_regdate`, `username`, `username_clean`, `user_password`, `user_passchg`, `user_pass_convert`, `user_email`, `user_email_hash`, `user_birthday`, `user_lastvisit`, `user_lastmark`, `user_lastpost_time`) VALUES(";
	sql += "0 ,2, '', 0, ''";
	sql += ",";
	sql += register_time;
	sql += ",";
	AppendMysql(sql, user.c_str());
	sql += ",";
	AppendMysql(sql, user.c_str());
	sql += ",";
	AppendMysql(sql, password.c_str());
	sql += ",";
	sql += register_time;
	sql += ",";
	sql += "0";
	sql += ",";
	AppendMysql(sql, email.c_str());
	sql += ",";
	sql += "'0' ,''";
	sql += ",";
	sql += register_time;
	sql += ",";
	sql += register_time;
	sql += ",";
	sql += "'0'"; 
	sql += ")";
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d user already exists\n", CODE_DENIED );
		g_free(register_time);
		return;
	}
	stardictdMain.conn.lastInsertID(userID);
	
	sql = "INSERT INTO user_basic (id, register_time, email) VALUES(";
	sql += userID;
	sql += ",";
	sql += register_time;
	g_free(register_time);
	sql += ",";
	AppendMysql(sql, email.c_str());
	sql += ")";

	std::string text;	
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		text = "Init user_basic table failed: ";
		text += sql;
		stardictdMain.logger->log(LOG_ERROR, text.c_str());
	}
	
	sql = "INSERT INTO `stardict_user_group` (`user_id`, `group_id`,`group_leader`, `user_pending`) VALUES(";
	sql += userID;
	sql += ",";
	sql += "2";
	sql += ",";
	sql += "0";
	sql += ",";
	sql += "0";
	sql += ")";
	
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		text = "Init stardict_user_group table failed: ";
		text += sql;
		stardictdMain.logger->log(LOG_ERROR, text.c_str());
	}*/
	text = "Register user: ";
	text += user;
	text += ' ';
	text += password;
	text += ' ';
	text += email;
	stardictdMain.logger->log(LOG_ACCOUNT, text.c_str());

	daemon_printf( "%d registered\n", CODE_OK );
}

static void daemon_setdictmask(std::string &dictmask)
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "UPDATE user_basic SET dictmask=";
	AppendMysql(sql, dictmask.c_str());
	sql += " WHERE id=";
	sql += userID;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
}

static void daemon_setcollatefunc(std::string &func)
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_DENIED );
		return;
	}
	if (userLevel == 0) {
		bool enable_collate = stardictdMain.conf->get_bool("level-0-user/enable_collate");
		if (!enable_collate) {
			daemon_printf( "%d level 0 user can't set collate function.\n", CODE_DENIED );
			return;
		}
	}
	int i = atoi(func.c_str());
	if (func.empty() || i<0 || i-1 >= COLLATE_FUNC_NUMS) {
		daemon_printf( "%d wrong collate func\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "UPDATE user_basic SET collatefunc=";
	gchar *str = g_strdup_printf("%d", i); // func may contain other string, which will cause security problem.
	sql += str;
	g_free(str);
	sql += " WHERE id=";
	sql += userID;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	stardictdMain.SetServerCollateFunc(i);
	daemon_printf( "%d\n", CODE_OK );
}

static void daemon_setlanguage(std::string &lang)
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "UPDATE user_basic SET language=";
	AppendMysql(sql, lang.c_str());
	sql += " WHERE id=";
	sql += userID;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
}

static void daemon_setemail(std::string &email)
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_DENIED );
		return;
	}
	if (strchr(email.c_str(), '@') == NULL) {
		daemon_printf( "%d invalid email\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "UPDATE user_basic SET email=";
	AppendMysql(sql, email.c_str());
	sql += " WHERE id=";
	sql += userID;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
}

static void daemon_getdictmask()
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_USER_NOT_REGISTER );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "SELECT dictmask FROM user_basic WHERE id=";
	sql += userID;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	MySQLResult *res = stardictdMain.conn.getResult();
	if (!res) {
		daemon_printf( "%d get result failed\n", CODE_DENIED );
		return;
	}
	DB_ROW row = res->fetchRow();
	if (!row) {
		res->destroy();
		daemon_printf( "%d fetch row failed\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	const char *dictmask = row[0];

	char *str = g_strdup_printf("level-%d-user/max_dict_count", userLevel);
	const int max_dict_count = stardictdMain.conf->get_int(str);
	g_free(str);
	std::string dictmask_str = stardictdMain.oLibs.get_dicts_list(dictmask, max_dict_count, userLevel);
	net_write_str(dictmask_str.c_str());
	res->destroy();
}

static void daemon_getcollatefunc()
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "SELECT collatefunc FROM user_basic WHERE id=";
	sql += userID;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	MySQLResult *res = stardictdMain.conn.getResult();
	if (!res) {
		daemon_printf( "%d get result failed\n", CODE_DENIED );
		return;
	}
	DB_ROW row = res->fetchRow();
	if (!row) {
		res->destroy();
		daemon_printf( "%d fetch row failed\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	net_write_str(row[0]);
	res->destroy();
}

static void daemon_getlanguage()
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "SELECT language FROM user_basic WHERE id=";
	sql += userID;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	MySQLResult *res = stardictdMain.conn.getResult();
	if (!res) {
		daemon_printf( "%d get result failed\n", CODE_DENIED );
		return;
	}
	DB_ROW row = res->fetchRow();
	if (!row) {
		res->destroy();
		daemon_printf( "%d fetch row failed\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	net_write_str(row[0]);
	res->destroy();
}

static void daemon_getemail()
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "SELECT email FROM user_basic WHERE id=";
	sql += userID;
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	MySQLResult *res = stardictdMain.conn.getResult();
	if (!res) {
		daemon_printf( "%d get result failed\n", CODE_DENIED );
		return;
	}
	DB_ROW row = res->fetchRow();
	if (!row) {
		res->destroy();
		daemon_printf( "%d fetch row failed\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	net_write_str(row[0]);
	res->destroy();
}

static void daemon_getuserlevel()
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_DENIED );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	gchar *level = g_strdup_printf("%d", userLevel);
	net_write_str(level);
	g_free(level);
}

static void daemon_maxdictcount()
{
	if (userID.empty()) {
		daemon_printf( "%d unknow user id\n", CODE_USER_NOT_REGISTER );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	char *str = g_strdup_printf("level-%d-user/max_dict_count", userLevel);
	const int max_dict_count = stardictdMain.conf->get_int(str);
	g_free(str);
	str = g_strdup_printf("%d", max_dict_count);
	net_write_str(str);
	g_free(str);
}

static void daemon_userlevel(std::string &user, std::string &fromlevel, std::string &tolevel)
{
	if (auth_user != "root") {
		daemon_printf( "%d no permission\n", CODE_DENIED );
		return;
	}
	int from = atoi(fromlevel.c_str());
	if (fromlevel.empty() || from<0 || from>5) {
		daemon_printf( "%d wrong from-level\n", CODE_DENIED );
		return;
	}
	int to = atoi(tolevel.c_str());
	if (tolevel.empty() || to<0 || to>5 || to==from) {
		daemon_printf( "%d wrong to-level\n", CODE_DENIED );
		return;
	}
	if (init_database()) {
		daemon_printf( "%d Connect to MySQL failed!\n", CODE_DENIED );
		return;
	}
	std::string sql;
	sql = "SELECT level FROM stardict_users WHERE username=";
	AppendMysql(sql, user.c_str());
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query user level failed\n", CODE_DENIED );
		return;
	}
	MySQLResult *res = stardictdMain.conn.getResult();
	if (!res) {
		daemon_printf( "%d Get result failed!\n", CODE_DENIED );
		return;
	}
	DB_ROW row = res->fetchRow();
	if (!row) {
		res->destroy();
		daemon_printf( "%d user doesn't exist.\n", CODE_DENIED );
		return;
	}
	int clevel = atoi(row[0]);
	res->destroy();
	if (clevel != from) {
		daemon_printf( "%d current level is %d.\n", CODE_DENIED, clevel );
		return;
	}
	sql = "UPDATE stardict_users SET level=";
	gchar *str = g_strdup_printf("%d", to);
	sql += str;
	g_free(str);
	sql += " WHERE username=";
	AppendMysql(sql, user.c_str());
	if (stardictdMain.conn.query(sql.c_str(), sql.length())) {
		daemon_printf( "%d query failed\n", CODE_DENIED );
		return;
	}
	std::string text;
	text = "Set user level: ";
	text += user;
	text += ' ';
	text += fromlevel;
	text += ' ';
	text += tolevel;
	stardictdMain.logger->log(LOG_ACCOUNT, text.c_str());
	daemon_printf( "%d\n", CODE_OK );
}

static void daemon_dirinfo(std::string &path)
{
	const std::string *info_string = stardictdMain.oLibs.get_dir_info(path.c_str());
	if (info_string) {
		daemon_printf( "%d\n", CODE_OK );
		gchar *str = g_strdup_printf("<userlevel>%d</userlevel>", userLevel);
		net_write(str, strlen(str));
		g_free(str);
		net_write_str(info_string->c_str());
	} else {
		daemon_printf( "%d wrong path\n", CODE_DENIED );
	}
}

static void daemon_fromto()
{
	const std::string &info_string = stardictdMain.oLibs.get_fromto_info();
	daemon_printf("%d\n", CODE_OK);
	net_write_str(info_string.c_str());
}

static void daemon_dictinfo(std::string &uid)
{
	const std::string *info_string = stardictdMain.oLibs.get_dict_info(uid.c_str(), false);
	if (info_string) {
		daemon_printf( "%d\n", CODE_OK );
		net_write_str(info_string->c_str());
	} else {
		daemon_printf( "%d unknow dict uid\n", CODE_DENIED );
	}
}

static void daemon_dictslist(std::string &dicts)
{
	const int max_dict_count = stardictdMain.conf->get_int("level-0-user/max_dict_count");
	std::string info_string = stardictdMain.oLibs.get_dicts_list(dicts.c_str(), max_dict_count, 0);
	daemon_printf( "%d\n", CODE_OK );
	net_write_str(info_string.c_str());
}

static void daemon_getadinfo()
{
	daemon_printf( "%d\n", CODE_OK );
	const char *ad_info = stardictdMain.conf->get_str("info/ad_info").c_str();
	net_write_str(ad_info);
}

static void daemon_LookupinTmpdictmask(std::string &Tmpdictmask)
{
	daemon_printf( "%d\n", CODE_OK );
	auth_user = "guest";
	const int max_dict_count = stardictdMain.conf->get_int("level-0-user/max_dict_count");
	stardictdMain.SetDictMask(Tmpdictmask.c_str(), max_dict_count, 0);
	stardictdMain.SetServerCollateFunc(0);
}

static bool check_dictmask()
{
	if (auth_user.empty()) {
		// Didn't authorized.
		auth_user = "guest";
		stardictdMain.SetDictMask(stardictdMain.conf->get_str("guest-user/dictmask").c_str(), -1, -1);
		stardictdMain.SetServerCollateFunc(0);
	} else {
		if (stardictdMain.query_dictmask.size()==0)
			return true;
	}
	return false;
}

static void daemon_lookup(std::string &word, int wordcount)
{
	if (check_dictmask()) {
		daemon_printf( "%d\n", CODE_DICTMASK_NOTSET );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	stardictdMain.lookup(word.c_str(), wordcount);
}

static void daemon_query(std::string &word)
{
	if (check_dictmask()) {
		daemon_printf( "%d\n", CODE_DICTMASK_NOTSET );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	stardictdMain.query(word.c_str());
}

static void daemon_selectquery(std::string &word)
{
	if (check_dictmask()) {
		daemon_printf( "%d\n", CODE_DICTMASK_NOTSET );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	stardictdMain.selectquery(word.c_str());
}

static void daemon_smartquery(std::string &word, std::string &BeginPos)
{
	if (check_dictmask()) {
		daemon_printf( "%d\n", CODE_DICTMASK_NOTSET );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	stardictdMain.smartquery(word.c_str(), atoi(BeginPos.c_str()));
}

static void daemon_define(std::string &word)
{
	if (check_dictmask()) {
		daemon_printf( "%d\n", CODE_DICTMASK_NOTSET );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	stardictdMain.define(word.c_str());
}

static void daemon_previous(std::string &word, int wordcount)
{
	if (check_dictmask()) {
		daemon_printf( "%d\n", CODE_DICTMASK_NOTSET );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	stardictdMain.previous(word.c_str(), wordcount);
}

static void daemon_next(std::string &word, int wordcount)
{
	if (check_dictmask()) {
		daemon_printf( "%d\n", CODE_DICTMASK_NOTSET );
		return;
	}
	daemon_printf( "%d\n", CODE_OK );
	stardictdMain.next(word.c_str(), wordcount);
}

static void daemon_client(std::string &protocol_name, std::string &client_name)
{
	double version;
	version = atof(protocol_name.c_str());
	if (version == 0.4) {
		daemon_ok( CODE_OK, "OK");
	} else {
		if (version > 0.4) {
			daemon_ok(CODE_DENIED, "The protocol version is not the same! You need to change to the newest stardictd server!");
		} else {
			daemon_ok(CODE_DENIED, "The protocol version is not the same! You need to update the client!");
		}
	}
}

static void daemon_banner()
{
	gchar *hostname = net_get_hostname();
	time_t t;
	time(&t);
	snprintf( daemonStamp, sizeof (daemonStamp), "<%d.%d.%lu@%s>",
		stardictdMain._dict_forks,
		(int) getpid(),
		(long unsigned)t,
		hostname );
	daemon_printf( "%d %s %s <auth> %s\n",
			CODE_HELLO,
			hostname,
			"stardictd-0.5",
			daemonStamp);
	g_free(hostname);
}

static void daemon_quit()
{
	daemon_ok( CODE_GOODBYE, "bye");
	daemon_terminate( 0, "quit" );
}

static std::vector<std::string> *args_new(const char *buf)
{
	std::vector<std::string> *args = new std::vector<std::string>;
	std::string str;
	const char *p = buf;
	while (*p) {
		if (*p=='\\') {
			p++;
			switch (*p) {
			case ' ':
				str+=' ';
				break;
			case '\\':
				str+='\\';
				break;
			case 'n':
				str+='\n';
				break;
			default:
				str+=*p;
				break;
			}
		} else if (*p == ' ') {
			args->push_back(str);
			str.clear();
		} else {
			str+=*p;
		}
		p++;
	}
	args->push_back(str);
	return args;
}

static int _handleconn (int delay_time, int max_cost_time, int error)
{
	if (error) {
		if (error == 1) {
			daemon_printf( "%d server temporarily unavailable\n", CODE_TEMPORARILY_UNAVAILABLE );
			printf("daemon busy: temporarily unavailable\n");
		} else {
			printf("daemon too busy: drop connection\n");
		}
		net_close_fd();
		return 0;
	}

	daemon_banner();

	std::string buf;
	std::vector<std::string> *args;
	alarm(delay_time);
	while (true) {
		if (net_read_line(buf))
			break;
		alarm(max_cost_time);
		printf("%s\n", buf.c_str());
		args = args_new(buf.c_str());
		if (args->size() == 0) {
			daemon_printf( "%d no command\n", CODE_SYNTAX_ERROR );
		} else if ((*args)[0] == "client" && args->size()>=3) {
			daemon_client((*args)[1], (*args)[2]);
		} else if ((*args)[0] == "auth" && args->size()>=3) {
			daemon_auth((*args)[1], (*args)[2]);
		} else if ((*args)[0] == "lookup" && args->size()>=2) {
			if (args->size() >=3) {
				int wordcount = atoi((*args)[2].c_str());
				if (wordcount < 1) {
					wordcount = 1;
				} else if (wordcount > 100) {
					wordcount = 100;
				}
				daemon_lookup((*args)[1], wordcount);
			} else {
				daemon_lookup((*args)[1], 30);
			}
		} else if ((*args)[0] == "query" && args->size()>=2) {
			daemon_query((*args)[1]);
		} else if ((*args)[0] == "selectquery" && args->size()>=2) {
			daemon_selectquery((*args)[1]);
		} else if ((*args)[0] == "smartquery" && args->size()>=3) {
			daemon_smartquery((*args)[1], (*args)[2]);
		} else if ((*args)[0] == "define" && args->size()>=2) {
			daemon_define((*args)[1]);
		} else if ((*args)[0] == "previous" && args->size()>=2) {
			if (args->size() >=3) {
				int wordcount = atoi((*args)[2].c_str());
				if (wordcount < 1) {
					wordcount = 1;
				} else if (wordcount > 50) {
					wordcount = 50;
				}
				daemon_previous((*args)[1], wordcount);
			} else {
				daemon_previous((*args)[1], 15);
			}
		} else if ((*args)[0] == "next" && args->size()>=2) {
			if (args->size() >=3) {
				int wordcount = atoi((*args)[2].c_str());
				if (wordcount < 1) {
					wordcount = 1;
				} else if (wordcount > 100) {
					wordcount = 100;
				}
				daemon_next((*args)[1], wordcount);
			} else {
				daemon_next((*args)[1], 30);
			}
		} else if ((*args)[0] == "quit" && args->size()>=1) {
			daemon_quit();
		} else if ((*args)[0] == "register" && args->size()>=4) {
			daemon_register((*args)[1], (*args)[2], (*args)[3]);
		} else if ((*args)[0] == "change_password" && args->size()>=4) {
			daemon_change_password((*args)[1], (*args)[2], (*args)[3]);
		} else if ((*args)[0] == "setdictmask" && args->size()>=2) {
			daemon_setdictmask((*args)[1]);
		} else if ((*args)[0] == "getdictmask" && args->size()>=1) {
			daemon_getdictmask();
		} else if ((*args)[0] == "setcollatefunc" && args->size()>=2) {
			daemon_setcollatefunc((*args)[1]);
		} else if ((*args)[0] == "getcollatefunc" && args->size()>=1) {
			daemon_getcollatefunc();
		} else if ((*args)[0] == "setlanguage" && args->size()>=2) {
			daemon_setlanguage((*args)[1]);
		} else if ((*args)[0] == "getlanguage" && args->size()>=1) {
			daemon_getlanguage();
		} else if ((*args)[0] == "setemail" && args->size()>=2) {
			daemon_setemail((*args)[1]);
		} else if ((*args)[0] == "getemail" && args->size()>=1) {
			daemon_getemail();
		} else if ((*args)[0] == "getuserlevel" && args->size()>=1) {
			daemon_getuserlevel();
		} else if ((*args)[0] == "maxdictcount" && args->size()>=1) {
			daemon_maxdictcount();
		} else if ((*args)[0] == "dirinfo" && args->size()>=2) {
			daemon_dirinfo((*args)[1]);
		} else if ((*args)[0] == "dictinfo" && args->size()>=2) {
			daemon_dictinfo((*args)[1]);
		} else if ((*args)[0] == "userlevel" && args->size()>=4) {
			daemon_userlevel((*args)[1], (*args)[2], (*args)[3]);
		} else if ((*args)[0] == "fromto" && args->size() >= 1) {
			daemon_fromto();
		} else if ((*args)[0] == "dictslist" && args->size()>=2) {
			daemon_dictslist((*args)[1]);
		} else if ((*args)[0] == "tmpdictmask" && args->size() >= 2) {
			daemon_LookupinTmpdictmask((*args)[1]);
		} else if ((*args)[0] == "getadinfo" && args->size()>=1) {
			daemon_getadinfo();
		} else {
			daemon_printf( "%d unknown command\n", CODE_SYNTAX_ERROR );
		}
		delete args;
		alarm(delay_time);
	}
	daemon_terminate( 0, "close" );
	return 0;
}

int dict_daemon( int s, int delay_time, int max_cost_time, int error)
{
	net_set_fd(s);
	return _handleconn(delay_time, max_cost_time, error);
}

void daemon_terminate( int sig, const char *name )
{
	alarm(0);
	printf("daemon_terminate: %s\n", name);

	net_close_fd();
	exit(0);
}
