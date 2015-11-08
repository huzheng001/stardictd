#ifndef _DB_H_
#define _DB_H_

#include <mysql.h>

#include <string>

typedef char **DB_ROW;

class MySQLResult {
public:
	MySQLResult();
	void set(MYSQL_RES *res);
	void destroy();
	DB_ROW fetchRow();
	unsigned long *fetchLengths();
private:
	MYSQL_RES *result;
};

class MySQLConn {
public:
	MySQLConn();
	~MySQLConn();

	bool init(const char *host, const char *user, const char *passwd,
			const char *db, int port);
	void destroy();
	unsigned int escapeString(char *to, const char *from, int len);
	bool query(const char *sql, int n);
	MySQLResult *getResult();
	void lastInsertID(std::string &userID);
private:
	MYSQL *mysql;
	// So that we need not to new a result every query
	MySQLResult result;
};

#endif
