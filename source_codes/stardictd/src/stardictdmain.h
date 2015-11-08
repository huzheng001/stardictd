#ifndef _STARDICTD_MAIN_H
#define _STARDICTD_MAIN_H

#include "servicemain.h"
#include "db.h"
#include "lib/lib.h"
#include "conf.h"
#include "log.h"

enum TextWinQueryResult
{
        TEXT_WIN_FOUND,
        TEXT_WIN_NOT_FOUND,
        TEXT_WIN_SHOW_FIRST,
        TEXT_WIN_FUZZY_NOT_FOUND,
        TEXT_WIN_PATTERN_NOT_FOUND,
        TEXT_WIN_TIPS, // when the word entry is empty,it will show tip
        TEXT_WIN_INFO,
        TEXT_WIN_TREEDICT,
};

enum NotFoundReason
{
	REASON_NOT_FOUND,
	REASON_FUZZY_NOT_FOUND,
	REASON_PATTERN_NOT_FOUND,
	REASON_FULLTEXT_NOT_FOUND,
};

class StardictdMain : public ServiceMain {
public:
	Libs oLibs;
	MySQLConn conn;
	Logger *logger;
	Conf *conf;
	bool _is_daemon;
	int _dict_reaps;
	int _dict_forks;
	std::vector<InstantDictIndex> query_dictmask;
	std::vector<InstantDictIndex> scan_dictmask;

	StardictdMain();
	~StardictdMain();

	virtual bool init(int argc, char *argv[]);
	virtual void run();

	void lookup(const gchar* sWord, int wordcount);
	void query(const gchar* sWord);
	void selectquery(const gchar* sWord);
	bool smartquery(const gchar* sWord, int BeginPos);
	void define(const gchar* sWord);
	void previous(const gchar* sWord, int wordcount);
	void next(const gchar* sWord, int wordcount);
	void SetDictMask(const gchar *dicts, int max, int userLevel);
	void SetServerCollateFunc(int servercollatefunc);
	void SetUserLang(const char *lang);
private:
	int childSocket;
	time_t startTime;
	int servercollatefunc;
	std::string UserLang;

	int start_daemon();

	void BuildResultData(std::vector<InstantDictIndex> &dictmask, const char* sWord, CurrentIndex *iIndex, const gchar *piIndexValidStr, int iLib, gchar ***pppWord, gchar ****ppppWordData, bool &bFound, gint Method);
	void FreeResultData(std::vector<InstantDictIndex> &dictmask, gchar ***pppWord, gchar ****ppppWordData);
	bool SimpleLookupToTextWin(const gchar* sWord, CurrentIndex* piIndex, const gchar *piIndexValidStr = NULL, bool bTryMoreIfNotFound = false, bool bShowNotfound = true, bool isShowFirst = false);
	void ListWords(CurrentIndex* iCurrent, int maxwordcount);
	void LookupWithRuleToMainWin(const gchar* word);
	void LookupWithRegexToMainWin(const gchar* word);
	void LookupWithFuzzyToMainWin(const gchar* word);
	void LookupDataToMainWin(const gchar *sWord);
	bool SimpleLookupToFloat(const char* sWord, bool bShowIfNotFound);
	void LookupWithFuzzyToFloatWin(const gchar *sWord);
	void SimpleLookup(const char* sWord);
	void ShowData(std::vector<InstantDictIndex> &dictmask, gchar ***pppWord, gchar ****ppppWordData, const char* sWord);
	void ShowNotFound(const char* sWord, NotFoundReason reason);
	void ShowADs();
};

extern StardictdMain stardictdMain;

#endif
