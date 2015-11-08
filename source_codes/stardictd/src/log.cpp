#include "log.h"
#include <glib.h>

#include <string>

Logger::Logger(LOG_LEVEL level)
{
	std::string path = g_get_home_dir();
	path += G_DIR_SEPARATOR;
	logFile = fopen((path+"stardictd.log").c_str(), "a");
	if (!logFile) {
		logFile = stderr;
		printf("Open log file failed!\n");
	}
	accountFile = fopen((path+"account.log").c_str(), "a");
	if (!accountFile) {
		accountFile = stderr;
		printf("Open account file failed!\n");
	}
	logLevel = level;
}

Logger::~Logger()
{
	if (logFile && logFile != stderr)
		fclose(logFile);
	if (accountFile && accountFile != stderr)
		fclose(accountFile);
}

static const char *level2str(LOG_LEVEL level)
{
	switch (level) {
	case LOG_ACCOUNT:
		return "ACCOUNT";
	case LOG_ERROR:
		return "ERROR";
	case LOG_WARNING:
		return "WARNING";
	case LOG_INFORMATION:
		return "INFORMATION";
	case LOG_DEBUG:
		return "DEBUG";
	default:
		return "UNKNOWN";
	}
}
static const char *logTime(time_t t)
{
	static char buf[128];
	static time_t last_t;

	if (t != last_t) {
		struct tm *tm = localtime(&t);
		strftime(buf, 127, "%Y/%m/%d %H:%M:%S", tm);
		last_t = t;
	}
	return buf;
}

void Logger::log(LOG_LEVEL level, const char *text)
{
	if (level > logLevel)
		return;
	if (level == LOG_ACCOUNT) {
		fprintf(accountFile, "%s| %s: %s\n", logTime(time(NULL)), level2str(level), text);
		fflush(accountFile);
	} else {
		fprintf(logFile, "%s| %s: %s\n", logTime(time(NULL)), level2str(level), text);
		fflush(logFile);
	}
}
