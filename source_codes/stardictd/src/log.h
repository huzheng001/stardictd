#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>

enum LOG_LEVEL {
	LOG_NONE,
	LOG_ACCOUNT,
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFORMATION,
	LOG_DEBUG
};

class Logger {
public:
	Logger(LOG_LEVEL level);
	~Logger();
	void log(LOG_LEVEL level, const char *text);
private:
	FILE *logFile;
	FILE *accountFile;
	LOG_LEVEL logLevel;
};

#endif
