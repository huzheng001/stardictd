#include "stardictdmain.h"
#include "net.h"
#include "daemon.h"

#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

StardictdMain stardictdMain;

StardictdMain::StardictdMain() : oLibs(NULL, true, 2, 0)
{
	_is_daemon = false;
	conf = NULL;
	logger = NULL;
	servercollatefunc = 0;
	UserLevel = 0;
}

StardictdMain::~StardictdMain()
{
	delete conf;
	delete logger;
}

static void setsig( int sig, void (*f)(int), int sa_flags )
{
	struct sigaction   sa;
	sa.sa_handler = f;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = sa_flags;
	sigaction(sig, &sa, NULL);
}

static void reaper( int dummy )
{
	int status;
	pid_t pid;
	while ((pid = wait3(&status, WNOHANG, NULL)) > 0) {
		printf("Child exit!\n");
		++stardictdMain._dict_reaps;
	}
}

static const char * signal2name (int sig)
{
   static char name [50];

   switch (sig) {
   case SIGHUP:
      return "SIGHUP";
   case SIGINT:
      return "SIGINT";
   case SIGQUIT:
      return "SIGQUIT";
   case SIGILL:
      return "SIGILL";
   case SIGTRAP:
      return "SIGTRAP";
   case SIGTERM:
      return "SIGTERM";
   case SIGPIPE:
      return "SIGPIPE";
   case SIGALRM:
      return "SIGALRM";
   default:
      snprintf (name, sizeof (name), "Signal %d", sig);
      return name;
   }
}

static void handler( int sig )
{
   const char *name;
   //time_t     t;

   name = signal2name (sig);

   if (stardictdMain._is_daemon) {
      daemon_terminate( sig, name );
   } else {
      //tim_stop( "dictd" );
      switch (sig){
      case SIGALRM:
         /*if (_dict_markTime > 0){
            time(&t);
            log_info( ":T: %24.24s; %d/%d %sr %su %ss\n",
                      ctime(&t),
                      _dict_forks - _dict_reaps,
                      _dict_forks,
                      dict_format_time( tim_get_real( "dictd" ) ),
                      dict_format_time( tim_get_user( "dictd" ) ),
                      dict_format_time( tim_get_system( "dictd" ) ) );
            alarm(_dict_markTime);
            return;
         }*/

         break;
      }

      //log_sig_info (sig);
   }
   exit(sig+128);
}

bool StardictdMain::init(int argc, char *argv[])
{
	time(&startTime);
	setsig(SIGCHLD, reaper, SA_RESTART);
	setsig(SIGINT,  handler, 0);
	setsig(SIGQUIT, handler, 0);
	setsig(SIGILL,  handler, 0);
	setsig(SIGTRAP, handler, 0);
	setsig(SIGTERM, handler, 0);
	setsig(SIGPIPE, handler, 0);
	setsig(SIGALRM, handler, SA_RESTART);

	oLibs.LoadFromXML();
	conf = new Conf();
	logger = new Logger((LOG_LEVEL)(conf->get_int("server/log_level")));
	return true;
}

int StardictdMain::start_daemon()
{
	pid_t pid;

	++_dict_forks;
	switch ((pid = fork())) {
	case 0:
		_is_daemon = true;
		break;
	case -1:
		break;
	default:
		break;
	}
	return pid;
}

#define DICT_QUEUE_DEPTH         10

void StardictdMain::run()
{
	int masterSocket;
	struct sockaddr_in csin;

	socklen_t alen = sizeof(csin);
	const char *bind_to = NULL;
	const int depth = DICT_QUEUE_DEPTH;
	const int port = conf->get_int("server/port");
	const int _dict_daemon_limit = conf->get_int("server/process_limit");
	const int delay_time = conf->get_int("client/delay_time");
	const int max_cost_time = conf->get_int("client/max_cost_time");
	int dict_daemon_count;
	
	masterSocket = net_open_tcp( bind_to, port, depth );
	for (;;) {
		if ((childSocket = accept(masterSocket, (struct sockaddr *)&csin, &alen)) < 0) {
			if (errno == EINTR){
			}
			continue;
		}
		dict_daemon_count = _dict_forks - _dict_reaps;
		if (dict_daemon_count < _dict_daemon_limit) {
			if (!start_daemon()) { /* child */
				if (dict_daemon_count < (_dict_daemon_limit * 0.9)) {
					dict_daemon(childSocket, delay_time, max_cost_time, 0);
				} else {
					dict_daemon(childSocket, delay_time, max_cost_time, 1);
				}
				exit(0); // Prevent super software ability!
			} else { /* parent */
				close(childSocket);
			}
		} else {
			dict_daemon(childSocket, delay_time, max_cost_time, 2);
			exit(0); // Prevent super software ability!
		}
	}
}

void StardictdMain::SetDictMask(const char *dicts, int max, int userLevel)
{
	oLibs.SetServerDictMask(query_dictmask, dicts, max, userLevel);
	scan_dictmask = query_dictmask;
}

void StardictdMain::SetServerCollateFunc(int _servercollatefunc)
{
	servercollatefunc = _servercollatefunc;
	if (servercollatefunc!=0)
		oLibs.LoadCollateFile(query_dictmask, (CollateFunctions)(servercollatefunc-1));
}

void StardictdMain::SetUserLang(const char *lang)
{
	UserLang = lang;
}

void StardictdMain::SetUserLevel(int level)
{
	UserLevel = level;
}

void StardictdMain::BuildResultData(std::vector<InstantDictIndex> &dictmask, const char* sWord, CurrentIndex *iIndex, const gchar *piIndexValidStr, int iLib, gchar ***pppWord, gchar ****ppppWordData, bool &bFound, gint Method)
{
	if (dictmask[iLib].type != InstantDictType_LOCAL)
		return;

	int iRealLib = dictmask[iLib].index;
	gint i, j;
	gint count=0, syncount;
	bool bLookupWord, bLookupSynonymWord;
	gint nWord;
	glong iWordIdx;
	if (piIndexValidStr) {
		if (iIndex[iLib].idx != INVALID_INDEX) {
			bLookupWord = !strcmp(oLibs.poGetWord(iIndex[iLib].idx, iRealLib, servercollatefunc), piIndexValidStr);
		} else {
			bLookupWord = false;
		}
		if (iIndex[iLib].synidx != UNSET_INDEX && iIndex[iLib].synidx != INVALID_INDEX) {
			bLookupSynonymWord = !strcmp(oLibs.poGetSynonymWord(iIndex[iLib].synidx, iRealLib, servercollatefunc), piIndexValidStr);
		} else {
			bLookupSynonymWord = false;
		}
	} else {
		if (Method==0) {
			bLookupWord = oLibs.LookupWord(sWord, iIndex[iLib].idx, iIndex[iLib].idx_suggest, iRealLib, servercollatefunc);
			bLookupSynonymWord = oLibs.LookupSynonymWord(sWord, iIndex[iLib].synidx, iIndex[iLib].synidx_suggest, iRealLib, servercollatefunc);
		} else if (Method==1) {
			bLookupWord = oLibs.LookupSimilarWord(sWord, iIndex[iLib].idx, iIndex[iLib].idx_suggest, iRealLib, servercollatefunc);
			bLookupSynonymWord = oLibs.LookupSynonymSimilarWord(sWord, iIndex[iLib].synidx, iIndex[iLib].synidx_suggest, iRealLib, servercollatefunc);
		} else {
			bLookupWord = oLibs.SimpleLookupWord(sWord, iIndex[iLib].idx, iIndex[iLib].idx_suggest, iRealLib, servercollatefunc);
			bLookupSynonymWord = oLibs.SimpleLookupSynonymWord(sWord, iIndex[iLib].synidx, iIndex[iLib].synidx_suggest, iRealLib, servercollatefunc);
		}
	}
	if (bLookupWord || bLookupSynonymWord) {
		glong orig_idx, orig_synidx;
		orig_idx = oLibs.CltIndexToOrig(iIndex[iLib].idx, iRealLib, servercollatefunc);
		orig_synidx = oLibs.CltSynIndexToOrig(iIndex[iLib].synidx, iRealLib, servercollatefunc);
		nWord=0;
		if (bLookupWord)
			nWord++;
		if (bLookupSynonymWord) {
			syncount = oLibs.GetOrigWordCount(orig_synidx, iRealLib, false);
			nWord+=syncount;
		}
		pppWord[iLib] = (gchar **)g_malloc(sizeof(gchar *)*(nWord+1));
		ppppWordData[iLib] = (gchar ***)g_malloc(sizeof(gchar **)*(nWord));
		if (bLookupWord) {
			pppWord[iLib][0] = g_strdup(oLibs.poGetOrigWord(orig_idx, iRealLib));
			count = oLibs.GetOrigWordCount(orig_idx, iRealLib, true);
			ppppWordData[iLib][0] = (gchar **)g_malloc(sizeof(gchar *)*(count+1));
			for (i=0;i<count;i++) {
				ppppWordData[iLib][0][i] = stardict_datadup(oLibs.poGetOrigWordData(orig_idx+i, iRealLib));
			}
			ppppWordData[iLib][0][count] = NULL;
			i=1;
		} else {
			i=0;
		}
		for (j=0;i<nWord;i++,j++) {
			iWordIdx = oLibs.poGetOrigSynonymWordIdx(orig_synidx+j, iRealLib);
			if (bLookupWord) {
				if (iWordIdx>=orig_idx && (iWordIdx<orig_idx+count)) {
					nWord--;
					i--;
					continue;
				}
			}
			pppWord[iLib][i] = g_strdup(oLibs.poGetOrigWord(iWordIdx, iRealLib));
			ppppWordData[iLib][i] = (gchar **)g_malloc(sizeof(gchar *)*2);
			ppppWordData[iLib][i][0] = stardict_datadup(oLibs.poGetOrigWordData(iWordIdx, iRealLib));
			ppppWordData[iLib][i][1] = NULL;
		}
		pppWord[iLib][nWord] = NULL;
		bFound = true;
	} else {
		pppWord[iLib] = NULL;
	}
}

void StardictdMain::FreeResultData(std::vector<InstantDictIndex> &dictmask, gchar ***pppWord, gchar ****ppppWordData)
{
	if (!pppWord)
		return;
	int j, k;
	size_t i;
	for (i=0; i<dictmask.size(); i++) {
		if (pppWord[i]) {
			j=0;
			while (pppWord[i][j]) {
				k=0;
				while (ppppWordData[i][j][k]) {
					g_free(ppppWordData[i][j][k]);
					k++;
				}
				g_free(pppWord[i][j]);
				g_free(ppppWordData[i][j]);
				j++;
			}
			g_free(pppWord[i]);
			g_free(ppppWordData[i]);
		}
	}
	g_free(pppWord);
	g_free(ppppWordData);
}

static gchar* GetHeadWord(gchar *str)
{
        while (g_ascii_isspace(*str))
                str++;
        glong len = g_utf8_strlen(str, -1);
        if (len) {
                gchar *last_str = g_utf8_offset_to_pointer(str, len-1);
                gunichar last = g_utf8_get_char(last_str);
                while ((g_unichar_isspace(last) || g_unichar_ispunct(last)) || g_unichar_isdigit(last)) {
                        *last_str = '\0';
                        last_str = g_utf8_find_prev_char(str, last_str);
                        if (!last_str)
                                break;
                        last = g_utf8_get_char(last_str);
                }
        }
        return str;
}

bool StardictdMain::SimpleLookupToTextWin(const char* sWord, CurrentIndex *piIndex, const gchar *piIndexValidStr, bool bTryMoreIfNotFound, bool bShowNotfound, bool isShowFirst)
{
	bool bFound = false;
	gchar ***pppWord = (gchar ***)g_malloc(sizeof(gchar **) * query_dictmask.size());
	gchar ****ppppWordData = (gchar ****)g_malloc(sizeof(gchar ***) * query_dictmask.size());
	CurrentIndex *iIndex;
	if (!piIndex)
		iIndex = (CurrentIndex *)g_malloc(sizeof(CurrentIndex) * query_dictmask.size());
	else
		iIndex = piIndex;

	for (unsigned int iLib = 0; iLib < query_dictmask.size(); iLib++)
		BuildResultData(query_dictmask, sWord, iIndex, piIndexValidStr, iLib, pppWord, ppppWordData, bFound, 0);
	if (!bFound && !piIndexValidStr) {
		for (unsigned int iLib = 0; iLib < query_dictmask.size(); iLib++)
			BuildResultData(query_dictmask, sWord, iIndex, NULL, iLib, pppWord, ppppWordData, bFound, 1);
	}
	if (bFound) {
		ShowData(query_dictmask, pppWord, ppppWordData, sWord);
	} else {
		if (bTryMoreIfNotFound) {
			gchar *word = g_strdup(sWord);
			gchar *hword;
			hword = GetHeadWord(word);
			if (*hword) {
				if (!strcmp(hword,sWord)) {
					if (bShowNotfound)
						ShowNotFound(sWord, REASON_NOT_FOUND);
				} else {
					for (unsigned int iLib = 0; iLib < query_dictmask.size(); iLib++)
						BuildResultData(query_dictmask, hword, iIndex, NULL, iLib, pppWord, ppppWordData, bFound, 0);
					if (!bFound) {
						for (unsigned int iLib = 0; iLib < query_dictmask.size(); iLib++)
							BuildResultData(query_dictmask, hword, iIndex, NULL, iLib, pppWord, ppppWordData, bFound, 1);
					}
					if (bFound) {
						ShowData(query_dictmask, pppWord, ppppWordData, sWord);
					} else {
						if (bShowNotfound)
							ShowNotFound(sWord, REASON_NOT_FOUND);
					}
				}
			} else {
				if (bShowNotfound)
					ShowNotFound(sWord, REASON_NOT_FOUND);
			}
			g_free(word);
		} else {
			if (bShowNotfound)
				ShowNotFound(sWord, REASON_NOT_FOUND);
		}
	}

	if (!piIndex)
		g_free(iIndex);

	FreeResultData(query_dictmask, pppWord, ppppWordData);
	return bFound;
}

void StardictdMain::ShowData(std::vector<InstantDictIndex> &dictmask, gchar ***Word, gchar ****WordData, const gchar * sOriginWord)
{
	net_write_str(sOriginWord);
	const guint32 zero = 0;
        unsigned int i, j,k;
	for (i = 0; i < dictmask.size(); i++) {
                if (Word[i]) {
			net_write_str(oLibs.dict_name(dictmask[i].index).c_str());
                        j=0;
                        do {
				net_write_str(Word[i][j]);
				net_write_data(WordData[i][j][0]);
                                k=1;
                                while (WordData[i][j][k]) {
					net_write_data(WordData[i][j][k]);
                                        k++;
                                }
				net_write_data((const char *)&zero);
                                j++;
                        } while (Word[i][j]);
			net_write_str("");
                }
        }
	if (UserLevel == 0) {
		ShowADs();
	}
	net_write_str("");
}

void StardictdMain::ShowADs()
{
	// Need to move these codes to setting!

	const char *bookname;
	const char *word;
	const char *definition;
	if (UserLang == "zh_CN") {
		bookname = "链接";
		word = "译心译意网";
		definition = "找翻译？当翻译？上<a href=\"http://www.1x1y.com.cn\" target=\"_blank\">译心译意网</a>！";
	} else {
		bookname = "Links";
		word = "www.1x1y.com.cn";
		definition = "Find Translator? Become a Translator? Visit <a href=\"http://www.1x1y.com.cn\" target=\"_blank\">1x1y website</a> Now!";
	}
	net_write_str(bookname);
	net_write_str(word);
	guint32 size;
	size = strlen(definition) + 1 + 1;
	size = g_htonl(size);
	net_write((const char *)&size, sizeof(guint32));
	net_write("h", 1);
	net_write_str(definition);
	size = 0;
	net_write_data((const char *)&size);
	net_write_str("");
}

void StardictdMain::ShowNotFound(const char* sOriginWord, NotFoundReason reason)
{
	net_write_str(sOriginWord);
	net_write_str("");
}

void StardictdMain::LookupWithRuleToMainWin(const gchar *word)
{
	gchar **ppMatchWord = (gchar **)g_malloc(sizeof(gchar *) * (MAX_MATCH_ITEM_PER_LIB*2) * query_dictmask.size());
	gint iMatchCount=oLibs.LookupWithRule(word, ppMatchWord, query_dictmask);
	if (iMatchCount) {
		SimpleLookupToTextWin(ppMatchWord[0], NULL, NULL);
		net_write_str("r");
		for (gint i=0; i<iMatchCount; i++) {
			net_write_str(ppMatchWord[i]);
			g_free(ppMatchWord[i]);
		}
	} else {
		ShowNotFound(word, REASON_NOT_FOUND);
		net_write_str("r");
	}
	net_write_str("");
	g_free(ppMatchWord);
}

void StardictdMain::LookupWithRegexToMainWin(const gchar *word)
{
	gchar **ppMatchWord = (gchar **)g_malloc(sizeof(gchar *) * (MAX_MATCH_ITEM_PER_LIB*2) * query_dictmask.size());
	gint iMatchCount=oLibs.LookupWithRegex(word, ppMatchWord, query_dictmask);
	if (iMatchCount) {
		SimpleLookupToTextWin(ppMatchWord[0], NULL, NULL);
		net_write_str("g");
		for (gint i=0; i<iMatchCount; i++) {
			net_write_str(ppMatchWord[i]);
			g_free(ppMatchWord[i]);
		}
	} else {
		ShowNotFound(word, REASON_NOT_FOUND);
		net_write_str("g");
	}
	net_write_str("");
	g_free(ppMatchWord);
}

void StardictdMain::LookupWithFuzzyToMainWin(const gchar *sWord)
{
	const int MAX_FUZZY_MATCH_ITEM=100;
	gchar *fuzzy_reslist[MAX_FUZZY_MATCH_ITEM];
	bool Found= oLibs.LookupWithFuzzy(sWord, fuzzy_reslist, MAX_FUZZY_MATCH_ITEM, query_dictmask);
	if (Found) {
		SimpleLookupToTextWin(fuzzy_reslist[0], NULL, NULL);
		net_write_str("f");
		for (int i=0; i<MAX_FUZZY_MATCH_ITEM && fuzzy_reslist[i]; i++) {
			net_write_str(fuzzy_reslist[i]);
			g_free(fuzzy_reslist[i]);
		}
	} else {
		ShowNotFound(sWord, REASON_NOT_FOUND);
		net_write_str("f");
	}
	net_write_str("");
}

void StardictdMain::LookupDataToMainWin(const gchar *sWord)
{
	std::vector<gchar *> reslist[query_dictmask.size()];
	if (oLibs.LookupData(sWord, reslist, NULL, NULL, NULL, query_dictmask)) {
		std::vector<Dict *>::size_type i;
		for (i=0; i<query_dictmask.size(); i++) {
			if (!reslist[i].empty()) {
				SimpleLookupToTextWin(reslist[i][0], NULL, NULL);
				net_write_str("d");
				break;
			}
		}
		for (i=0; i<query_dictmask.size(); i++) {
			if (!reslist[i].empty()) {
				net_write_str(oLibs.dict_name(query_dictmask[i].index).c_str());
				for (std::vector<gchar *>::iterator p=reslist[i].begin(); p!=reslist[i].end(); ++p) {
					net_write_str(*p);
					g_free(*p);
				}
				net_write_str("");
			}
		}
	} else {
		ShowNotFound(sWord, REASON_NOT_FOUND);
		net_write_str("d");
	}
	net_write_str("");
}

void StardictdMain::ListWords(CurrentIndex* iCurrent, int maxwordcount)
{
	int iWordCount=0;
	const gchar * poCurrentWord=oLibs.poGetCurrentWord(iCurrent, query_dictmask, servercollatefunc);
	net_write_str("l");
	if (poCurrentWord) {
		net_write_str(poCurrentWord);
		iWordCount++;
		while (iWordCount<maxwordcount &&
				(poCurrentWord=oLibs.poGetNextWord(NULL,iCurrent, query_dictmask, servercollatefunc))) {
			net_write_str(poCurrentWord);
			iWordCount++;
		}
	}
	net_write_str("");
}

void StardictdMain::previous(const gchar* sWord, int maxwordcount)
{
	CurrentIndex *iPreIndex = (CurrentIndex *)g_malloc(sizeof(CurrentIndex) * query_dictmask.size());
	const gchar * preword=oLibs.poGetPreWord(sWord, iPreIndex, query_dictmask, servercollatefunc);
	if (preword) {
		int iWordCount=1;
		net_write_str(preword);
		while (iWordCount<maxwordcount && (preword=oLibs.poGetPreWord(NULL,iPreIndex, query_dictmask, servercollatefunc))) {
			net_write_str(preword);
			iWordCount++;
		}
	}
	g_free(iPreIndex);
	net_write_str("");
}

void StardictdMain::next(const gchar* sWord, int maxwordcount)
{
	CurrentIndex *iNextIndex = (CurrentIndex *)g_malloc(sizeof(CurrentIndex) * query_dictmask.size());
	const gchar * nextword=oLibs.poGetNextWord(sWord, iNextIndex, query_dictmask, servercollatefunc);
	if (nextword) {
		int iWordCount=1;
		net_write_str(nextword);
		while (iWordCount<maxwordcount && (nextword=oLibs.poGetNextWord(NULL,iNextIndex, query_dictmask, servercollatefunc))) {
			net_write_str(nextword);
			iWordCount++;
		}
	}
	g_free(iNextIndex);
	net_write_str("");
}

void StardictdMain::lookup(const gchar* sWord, int wordcount)
{
	std::string res;
	switch (analyse_query(sWord, res)) {
	case qtPATTERN:
		LookupWithRuleToMainWin(res.c_str());
		break;
	case qtREGEX:
		LookupWithRegexToMainWin(res.c_str());
		break;
	case qtFUZZY:
		LookupWithFuzzyToMainWin(res.c_str());
		break;
	case qtDATA:
		LookupDataToMainWin(res.c_str());
		break;
	default:
		CurrentIndex *iCurrentIndex = (CurrentIndex*)g_malloc(sizeof(CurrentIndex) * query_dictmask.size());
		bool find = SimpleLookupToTextWin(res.c_str(), iCurrentIndex, NULL, true, false, false);
		if (!find) {
			const gchar *sug_word = oLibs.GetSuggestWord(res.c_str(), iCurrentIndex, query_dictmask, servercollatefunc);
			if (sug_word) {
				gchar *suit_word = g_strdup(sug_word);
				SimpleLookupToTextWin(suit_word, iCurrentIndex, NULL, false, true, true);
				g_free(suit_word);
			} else {
				ShowNotFound(res.c_str(), REASON_NOT_FOUND);
			}
		}
		ListWords(iCurrentIndex, wordcount);
	}
}

void StardictdMain::define(const gchar* sWord)
{
	SimpleLookupToTextWin(sWord, NULL);
}

void StardictdMain::LookupWithFuzzyToFloatWin(const gchar *sWord)
{
	
}

void StardictdMain::SimpleLookup(const char* sWord)
{
	gchar ***pppWord = (gchar ***)g_malloc(sizeof(gchar **) * query_dictmask.size());
	gchar ****ppppWordData = (gchar ****)g_malloc(sizeof(gchar ***) * query_dictmask.size());
	CurrentIndex *iIndex = (CurrentIndex *)g_malloc(sizeof(CurrentIndex) * query_dictmask.size());
	bool bFound = false;
	for (unsigned int iLib=0;iLib<query_dictmask.size();iLib++)
		BuildResultData(query_dictmask, sWord, iIndex, NULL, iLib, pppWord, ppppWordData, bFound, 2);
	if (bFound) {
		ShowData(query_dictmask, pppWord, ppppWordData, sWord);
	} else {
		ShowNotFound(sWord, REASON_NOT_FOUND);
	}
	FreeResultData(query_dictmask, pppWord, ppppWordData);
	g_free(iIndex);
}

bool StardictdMain::SimpleLookupToFloat(const char* sWord, bool bShowIfNotFound)
{
	char *SearchWord = (char *)g_malloc(strlen(sWord)+1);
	const char *P1;
	char *P2, *EndPointer;
	P1=sWord;
	P2=SearchWord;
	// delete chinese space at the begining 
	while (*P1 && g_unichar_isspace(g_utf8_get_char(P1)))
		P1 = g_utf8_next_char(P1); 
	//format word, delete any spilth blanks.
	while(*P1) {
		if (g_unichar_isspace(g_utf8_get_char(P1))) {
			*P2++=' ';
			P1 = g_utf8_next_char(P1);
			while(g_unichar_isspace(g_utf8_get_char(P1)))
				P1 = g_utf8_next_char(P1);
		} else {
			g_utf8_strncpy(P2,P1,1);
			P1 = g_utf8_next_char(P1);
			P2 = g_utf8_next_char(P2);
		}
	}
	*P2='\0';
	if (SearchWord[0]=='\0') {
		strcpy(SearchWord, sWord);
	}
	EndPointer=SearchWord+strlen(SearchWord);
	gchar ***pppWord = (gchar ***)g_malloc(sizeof(gchar **) * scan_dictmask.size());
	gchar ****ppppWordData = (gchar ****)g_malloc(sizeof(gchar ***) * scan_dictmask.size());
	CurrentIndex *iIndex = (CurrentIndex *)g_malloc(sizeof(CurrentIndex) * scan_dictmask.size());
	//find the word use most biggest length
	while (EndPointer>SearchWord) {
		// delete end spaces
		while (EndPointer>SearchWord && *EndPointer==' ')
			*EndPointer--='\0';
		bool bFound = false;
		for (unsigned int iLib=0;iLib<scan_dictmask.size();iLib++)
			BuildResultData(scan_dictmask, SearchWord, iIndex, NULL, iLib, pppWord, ppppWordData, bFound, 2);
		if (bFound) {
			ShowData(scan_dictmask, pppWord, ppppWordData, SearchWord);
			FreeResultData(scan_dictmask, pppWord, ppppWordData);
			g_free(iIndex);
			g_free(SearchWord);
			return true;
		}
		// delete last word
		if (bIsPureEnglish(SearchWord)) {
			while (EndPointer>=SearchWord && *EndPointer!=' ')
				EndPointer--;
			if (EndPointer>=SearchWord)
				*EndPointer='\0';
		} else {// delete one character per time
			EndPointer = g_utf8_find_prev_char(SearchWord,EndPointer);
			if (EndPointer)
				*EndPointer='\0';
			else
				EndPointer = SearchWord-1; // so < SearchWord
		}
	}
	FreeResultData(scan_dictmask, pppWord, ppppWordData);
	g_free(iIndex);
	// not found
	if (bShowIfNotFound) {
		ShowNotFound(sWord, REASON_NOT_FOUND);
	}
	g_free(SearchWord);
	return false;
}

static gchar* GetPureEnglishAlpha(gchar *str)
{
	while (*str && (!((*str >= 'a' && *str <='z')||(*str >= 'A' && *str <='Z'))))
		str++;
	gchar *p = str;
	while (*p && ((*p >= 'a' && *p <='z')||(*p >= 'A' && *p <='Z')||(*p==' ')))
		p++;
	*p='\0';
	return str;
}

void StardictdMain::query(const gchar* sWord)
{
	SimpleLookup(sWord);
}

void StardictdMain::selectquery(const gchar* sWord)
{
	if (bIsPureEnglish(sWord)) {
		if (SimpleLookupToFloat(sWord,false) )  //found
			return;
		char *sToken = g_strdup(sWord);
		gchar *a = GetPureEnglishAlpha(sToken);
		if (*a) {
			if (strcmp(sWord, a)==0) {
				ShowNotFound(a, REASON_NOT_FOUND);
			} else {
				SimpleLookupToFloat(a,true);
			}
		} else {
			ShowNotFound(sWord, REASON_NOT_FOUND);
		}
		g_free(sToken);
	} else {
		SimpleLookupToFloat(sWord,true);
	}
}

bool StardictdMain::smartquery(const gchar* sWord, int BeginPos)
{
	if (sWord==NULL || sWord[0]=='\0') {
		ShowNotFound(sWord, REASON_NOT_FOUND);
		return false;
	}
	char *SearchWord = g_strdup(sWord);
	char *P1 = SearchWord + BeginPos;
	P1 = g_utf8_next_char(P1);
	while (*P1 && !g_unichar_isspace(g_utf8_get_char(P1)))
		P1 = g_utf8_next_char(P1);
	*P1='\0';
	P1 = SearchWord + BeginPos;
	if (BeginPos) {
		if (g_unichar_isspace(g_utf8_get_char(P1)))
			P1 = g_utf8_prev_char(P1);
		while (P1>SearchWord && !g_unichar_isspace(g_utf8_get_char(g_utf8_prev_char(P1))))
			P1 = g_utf8_prev_char(P1);
	}

	gchar ***pppWord = (gchar ***)g_malloc(sizeof(gchar **) * scan_dictmask.size());
	gchar ****ppppWordData = (gchar ****)g_malloc(sizeof(gchar ***) * scan_dictmask.size());
	CurrentIndex *iIndex = (CurrentIndex *)g_malloc(sizeof(CurrentIndex) * scan_dictmask.size());

	int SearchTimes = 2;
	while (SearchTimes) {
		bool bFound = false;
		for (size_t iLib=0;iLib<scan_dictmask.size();iLib++)
			BuildResultData(scan_dictmask, P1, iIndex, NULL, iLib, pppWord, ppppWordData, bFound, 2);
		if (bFound) {
			ShowData(scan_dictmask, pppWord, ppppWordData, P1);
			FreeResultData(scan_dictmask, pppWord, ppppWordData);
			g_free(iIndex);
			g_free(SearchWord);
			return true;
		}
		SearchTimes--;
		if (!SearchTimes)
			break;
		if (bIsPureEnglish(P1)) {
			char *P2 = SearchWord + BeginPos;
			if (g_ascii_isupper(*P2)) {
				char *P3 = SearchWord + BeginPos;
				P2++;
				if (*P2) {
					if (g_ascii_isupper(*P2)) {
						P2++;
						while (*P1 && g_ascii_isupper(*P2))
							P2++;
						while (P3>SearchWord && g_ascii_isupper(*(P3-1)))
							P3--;
					} else if (g_ascii_islower(*P2)){
						P2++;
						while (*P2 && g_ascii_islower(*P2))
							P2++;
					}
					if (*P2) {
						*P2='\0';
					} else {
						if (P3==P1)
							break;
					}
					P1=P3;
				} else {
					while (P3>SearchWord && g_ascii_isupper(*(P3-1)))
						P3--;
					if (P3==P1)
						break;
					P1=P3;
				}
			} else if (g_ascii_islower(*P2)) {
				char *P3 = SearchWord + BeginPos;
				while (P3>SearchWord && g_ascii_islower(*(P3-1)))
					P3--;
				if (P3>SearchWord && g_ascii_isupper(*(P3-1)))
					P3--;
				P2++;
				while (*P2 && g_ascii_islower(*P2))
					P2++;
				if (*P2) {
					*P2='\0';
				} else {
					if (P3==P1)
						break;
				}
				P1=P3;
			} else if (*P2 == '-') {
				*P2=' ';
				SearchTimes = 2;
			} else {
				char *P3 = SearchWord + BeginPos;
				while (P3>SearchWord && g_ascii_isalpha(*(P3-1)))
					P3--;
				if (P3!=P2) {
					*P2='\0';
					P1=P3;
				}
				else
					break;
			}
		} else {
			if (P1==SearchWord + BeginPos) {
				char *EndPointer=P1+strlen(P1);
				EndPointer = g_utf8_prev_char(EndPointer);
				if (EndPointer!=P1) {
					*EndPointer='\0';
					SearchTimes = 2;
				}
				else {
					break;
				}
			} else {
				P1 = SearchWord + BeginPos;
				SearchTimes = 2;
			}
		}
	}
	FreeResultData(scan_dictmask, pppWord, ppppWordData);
	g_free(iIndex);

	// not found
	ShowNotFound(P1, REASON_NOT_FOUND);
	g_free(SearchWord);
	return false;
}
