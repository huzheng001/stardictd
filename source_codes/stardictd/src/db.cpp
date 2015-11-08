#include "db.h"
#include "string.h"
#include "stdio.h"
#include <glib.h>

MySQLResult::MySQLResult()
{
	result = NULL;
}

void MySQLResult::destroy()
{
	mysql_free_result(result);
	result = NULL;
}

void MySQLResult::set(MYSQL_RES *res)
{
	result = res;
}

DB_ROW MySQLResult::fetchRow()
{
	return mysql_fetch_row(result);
}

unsigned long *MySQLResult::fetchLengths()
{
	return mysql_fetch_lengths(result);
}

MySQLConn::MySQLConn()
{
	mysql = NULL;
}

MySQLConn::~MySQLConn()
{
	destroy();
}

bool MySQLConn::init(const char *host, const char *user, const char *passwd,
			const char *db, int port)
{
	static bool inited = false;
	if (inited)
		return false;
	inited = true;

	mysql_library_init(0, NULL, NULL);
	mysql = mysql_init(NULL);
	if (!mysql) {
		return true;
	}
	if (!mysql_real_connect(mysql, host, user, passwd, db, port, NULL, 0)) {
		return true;
	}
	return false;
}

void MySQLConn::destroy()
{
	if (mysql) {
		mysql_close(mysql);
		mysql_library_end();
	}
}

unsigned int MySQLConn::escapeString(char *to, const char *from, int len)
{
	return mysql_real_escape_string(mysql, to, from, len);
}

bool MySQLConn::query(const char *sql, int n)
{
	if (mysql_real_query(mysql, sql, n) != 0) {
		return true;
	}
	return false;
}

MySQLResult *MySQLConn::getResult()
{
	MYSQL_RES *res = mysql_store_result(mysql);
	if (!res)
		return NULL;
	result.set(res);
	return &result;
}

void MySQLConn::lastInsertID(std::string &userID)
{
	gchar *id = g_strdup_printf("%u", (unsigned int)mysql_insert_id(mysql));
	userID = id;
	g_free(id);
}
