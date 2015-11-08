#ifndef _PRINT_DATA_H_
#define _PRINT_DATA_H_

#include <string>

extern char* stardict_markup_escape_text (const char *text, int length);
extern void escape_text(std::string &etext, const char *text);
extern void print_data(char *data, const char *oword);

#endif
