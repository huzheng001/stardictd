#include "printdata.h"

#include <glib.h>

#include "string.h"
#include <stdio.h>
#include <stdlib.h>

static gchar* toUtfPhonetic(const gchar *text, gsize len)
{
	std::string p;
	gsize i;
	for (i=0;i<len;i++) {
		switch (text[i]) {
			case 'A':
				p+="æ"; break;
			case 'B':
				p+="ɑ"; break;
			case 'C':
				p+="ɔ"; break;
			case 'Q':
				p+="ʌ"; break;
			case 'E':
				p+="ә"; break;
			case 'Z':
				p+="є"; break;
			case 'N':
				p+="ŋ"; break;
			case 'W':
				p+="θ"; break;
			case 'T':
				p+="ð"; break;
			case 'F':
				p+="ʃ"; break;
			case 'V':
				p+="ʒ"; break;
			case 'L':
				p+="ɚ"; break;
			case 'I':
				p+="i"; break;
			case '^':
				p+="ɡ"; break;
			case '9':
				p+="ˏ"; break;
			case '5':
				p+="'"; break;
			default:
				p+=text[i];
				break;
		}
	}
	return stardict_markup_escape_text(p.c_str(), -1);
}

static gchar *powerword_markup_escape_text(const gchar *text, gssize length)
{
	const gchar *p;
	const gchar *end;
	p = text;
	end = text + length;

	GString *str;
	str = g_string_sized_new (length);

	const gchar *n;
	bool find;
	bool previous_islink = false;
	std::string marktags;
	guint currentmarktag = 0;
	const char *prev_char = NULL;
	while (p != end) {
		const gchar *next;
		next = g_utf8_next_char (p);
		switch (*p) {
			case '}':
				if (currentmarktag==0) {
					g_string_append (str, "}");
					previous_islink = false;
				}
				else {
					currentmarktag--;
					switch (marktags[currentmarktag]) {
						case 'b':
						case 'B':
							g_string_append (str, "</b>");
							previous_islink = false;
							break;
						case 'I':
							g_string_append (str, "</i>");
							previous_islink = false;
							break;
						case '+':
							g_string_append (str, "</sup>");
							previous_islink = false;
							break;
						case '-':
							g_string_append (str, "</sub>");
							previous_islink = false;
							break;
						case 'x':
							g_string_append (str, "</u></font>");
							previous_islink = false;
							break;
						case 'l':
						case 'D':
						case 'L':
						case 'U':
							g_string_append (str, "</u></font>");
							previous_islink = true;
							break;
						default:
							previous_islink = false;
							break;
					}
				}
				break;
			case '&':
				find = false;
				if (next!=end) {
					n = g_utf8_next_char(next);
					if (n!=end && *n == '{') {
						find=true;
						currentmarktag++;
						if (marktags.length()<currentmarktag)
							marktags+=*next;
						else
							marktags[currentmarktag-1]=*next;
						switch (*next) {
							case 'b':
							case 'B':
								g_string_append (str, "<b>");
								next = n+1;
								break;
							case 'I':
								g_string_append (str, "<i>");
								next = n+1;
								break;
							case '+':
								g_string_append (str, "<sup>");
								next = n+1;
								break;
							case '-':
								g_string_append (str, "<sub>");
								next = n+1;
								break;
							case 'x':
								g_string_append (str, "<font color=\"blue\"><u>");
								next = n+1;
								break;
							case 'X':
								{
								const gchar *tag_end = n+1;
								while (tag_end!=end) {
									if (*tag_end=='}')
										break;
									else
										tag_end++;
								}
								g_string_append (str, "<font color=\"blue\">");
								gchar *tag_str = toUtfPhonetic(n+1, tag_end - (n+1));
								g_string_append (str, tag_str);
								g_free(tag_str);
								g_string_append (str, "</font>");
								currentmarktag--;
								if (tag_end!=end)
									next = tag_end+1;
								else
									next = end;
								previous_islink = false;
								break;
								}
							case 'l':
							case 'D':
								if (previous_islink)
									g_string_append (str, "\t");
								g_string_append (str, "<font color=\"blue\"><u>");
								next = n+1;
								break;
							case 'L':
							case 'U':
								if (previous_islink)
									g_string_append (str, "\t");
								g_string_append (str, "<font color=\"#008080\"><u>");
								next = n+1;
								break;
							case '2':
								// Phonetic. Need more work...
								next = n+1;
								break;
							/*case ' ':
							case '9':
							case 'S':*/
							default:
								next = n+1;
								break;
						}
					}
				}
				if (!find) {
					previous_islink = false;
					g_string_append (str, "&amp;");
				}
				break;
			case '<':
				previous_islink = false;
				g_string_append (str, "&lt;");
				break;
			case '>':
				previous_islink = false;
				g_string_append (str, "&gt;");
				break;
			case '\'':
				previous_islink = false;
				g_string_append (str, "&#39;"); // "&apos;" can't show correctly in IE.
				break;
			case '"':
				previous_islink = false;
				g_string_append (str, "&quot;");
				break;
			case '\n':
				previous_islink = false;
				g_string_append (str, "<br>");
				break;
			case ' ':
				previous_islink = false;
				if (!prev_char || (*prev_char)==' ' || (*prev_char)=='\n')
					g_string_append (str, "&nbsp;");
				else
					g_string_append (str, " ");
				break;
			default:
				previous_islink = false;
				g_string_append_len (str, p, next - p);
				break;
		}
		prev_char = p;
		p = next;
	}
	if (currentmarktag>0) {
		do {
			currentmarktag--;
			switch (marktags[currentmarktag]) {
				case 'b':
				case 'B':
					g_string_append (str, "</b>");
					break;
				case 'I':
					g_string_append (str, "</i>");
					break;
				case '+':
					g_string_append (str, "</sup>");
					break;
				case '-':
					g_string_append (str, "</sub>");
					break;
				case 'x':
				case 'l':
				case 'D':
				case 'L':
				case 'U':
					g_string_append (str, "</u></font>");
					break;
				default:
					break;
			}
		} while (currentmarktag>0);
	}
	return g_string_free (str, FALSE);
}

typedef struct _PwUserData {
	std::string *res;
	const gchar *oword;
	bool first_jbcy;
} PwUserData;

static void func_parse_passthrough(GMarkupParseContext *context, const gchar *passthrough_text, gsize text_len, gpointer user_data, GError **error)
{
	if (!g_str_has_prefix(passthrough_text, "<![CDATA["))
		return;
	const gchar *element = g_markup_parse_context_get_element(context);
	if (!element)
		return;
	const gchar *text = passthrough_text+9;
	gsize len = text_len-9-3;
	while (g_ascii_isspace(*text)) {
		text++;
		len--;
	}
	while (len>0 && g_ascii_isspace(*(text+len-1))) {
		len--;
	}
	if (len==0)
		return;
	std::string *res = ((PwUserData*)user_data)->res;
	if (strcmp(element, "词典音标")==0) {
		if (!res->empty())
			*res+="<br>";
		*res+="[<font color=\"blue\">";
		gchar *str = toUtfPhonetic(text, len);
		*res+=str;
		g_free(str);
		*res+="</font>]";
	} else if (strcmp(element, "单词原型")==0) {
		const gchar *oword = ((PwUserData*)user_data)->oword;
		if (strncmp(oword, text, len)) {
			if (!res->empty())
				*res+="<br>";
			*res+="<b>";
			gchar *str = stardict_markup_escape_text(text, len);
			res->append(str);
			g_free(str);
			*res+="</b>";
		}
	} else if (strcmp(element, "单词词性")==0) {
		if (!res->empty())
			*res+="<br>";
		*res+="<i>";
		gchar *str = powerword_markup_escape_text(text, len);
		res->append(str);
		g_free(str);
		*res+="</i>";
	} else if (strcmp(element, "汉语拼音")==0) {
		if (!res->empty())
			*res+="<br>";
		*res+="<font color=\"blue\"><u>";
		gchar *str = powerword_markup_escape_text(text, len);
		res->append(str);
		g_free(str);
		*res+="</u></font>";
	} else if (strcmp(element, "例句原型")==0) {
		if (!res->empty())
			*res+="<br>";
		*res+="<font color=\"#008080\">";
		gchar *str = powerword_markup_escape_text(text, len);
		res->append(str);
		g_free(str);
		*res+="</font>";
	} else if (strcmp(element, "例句解释")==0) {
		if (!res->empty())
			*res+="<br>";
		*res+="<font color=\"#01259A\">";
		gchar *str = powerword_markup_escape_text(text, len);
		res->append(str);
		g_free(str);
		*res+="</font>";
	/*} else if (strcmp(element, "相关词")==0) {
		if (!res->empty())
			*res+="<br>";
		std::string tabstr;
		tabstr+=text[0];
		for (gsize i=1;i<len;i++) {
			if (text[i]=='&')
				tabstr+="\t&";
			else
				tabstr+=text[i];
		}
		gchar *str = powerword_markup_escape_text(tabstr.c_str(), tabstr.length());
		res->append(str);
		g_free(str);*/
	} else
	/*} else if (
	strcmp(element, "解释项")==0 ||
	strcmp(element, "跟随解释")==0 ||
	strcmp(element, "相关词")==0 ||
	strcmp(element, "预解释")==0 ||
	strcmp(element, "繁体写法")==0 ||
	strcmp(element, "台湾音标")==0 ||
	strcmp(element, "图片名称")==0 ||
	strcmp(element, "跟随注释")==0 ||
	strcmp(element, "音节分段")==0 ||
	strcmp(element, "AHD音标")==0 ||
	strcmp(element, "国际音标")==0 ||
	strcmp(element, "美国音标")==0 ||
	strcmp(element, "子解释项")==0 ||
	strcmp(element, "同义词")==0 ||
	strcmp(element, "日文发音")==0 ||
	strcmp(element, "惯用型原型")==0 ||
	strcmp(element, "惯用型解释")==0 ||
	strcmp(element, "另见")==0
	) {*/
	{
		if (!res->empty())
			*res+="<br>";
		gchar *str = powerword_markup_escape_text(text, len);
		res->append(str);
		g_free(str);
	}
}

static void func_parse_start_element(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer user_data, GError **error)
{
	std::string *res = ((PwUserData*)user_data)->res;
	if (strcmp(element_name, "基本词义")==0) {
		if (((PwUserData*)user_data)->first_jbcy) {
			((PwUserData*)user_data)->first_jbcy = false;
		} else {
			*res+="<br><font color=\"blue\">&lt;基本词义&gt;</font>";
		}
	} else if (strcmp(element_name, "继承用法")==0) {
		*res+="<br><font color=\"blue\">&lt;继承用法&gt;</font>";
	} else if (strcmp(element_name, "习惯用语")==0) {
		*res+="<br><font color=\"blue\">&lt;习惯用语&gt;</font>";
	} else if (strcmp(element_name, "词性变化")==0) {
		*res+="<br><font color=\"blue\">&lt;词性变化&gt;</font>";
	} else if (strcmp(element_name, "特殊用法")==0) {
		*res+="<br><font color=\"blue\">&lt;特殊用法&gt;</font>";
	} else if (strcmp(element_name, "参考词汇")==0) {
		*res+="<br><font color=\"blue\">&lt;参考词汇&gt;</font>";
	} else if (strcmp(element_name, "常用词组")==0) {
		*res+="<br><font color=\"blue\">&lt;常用词组&gt;</font>";
	} else if (strcmp(element_name, "语源")==0) {
		*res+="<br><font color=\"blue\">&lt;语源&gt;</font>";
	} else if (strcmp(element_name, "派生")==0) {
		*res+="<br><font color=\"blue\">&lt;派生&gt;</font>";
	} else if (strcmp(element_name, "用法")==0) {
		*res+="<br><font color=\"blue\">&lt;用法&gt;</font>";
	} else if (strcmp(element_name, "注释")==0) {
		*res+="<br><font color=\"blue\">&lt;注释&gt;</font>";
	}
}

static std::string powerword2html(const char *p, guint32 sec_size, const gchar *oword)
{
	std::string res;
	PwUserData Data;
	Data.res = &res;
	Data.oword = oword;
	Data.first_jbcy = true;

	GMarkupParser parser;
	parser.start_element = func_parse_start_element;
	parser.end_element = NULL;
	parser.text = NULL;
	parser.passthrough = func_parse_passthrough;
	parser.error = NULL;
	GMarkupParseContext* context = g_markup_parse_context_new(&parser, (GMarkupParseFlags)0, &Data, NULL);
	g_markup_parse_context_parse(context, p, sec_size, NULL);
	g_markup_parse_context_end_parse(context, NULL);
	g_markup_parse_context_free(context);
	return res;
}

typedef struct _XdxfParseUserData {
	std::string *res;
	bool firstk;
} XdxfParseUserData;

static void xdxf_parse_start_element(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer user_data, GError **error)
{
	XdxfParseUserData *Data = (XdxfParseUserData *)user_data;
	if (strcmp(element_name, "abr")==0) {
		Data->res->append("<font color=\"green\"><i>");
	} else if (strcmp(element_name, "kref")==0) {
		Data->res->append("<a href=\"javascript:stardict_query(&quot;");
	} else if (strcmp(element_name, "b")==0) {
		Data->res->append("<b>");
	} else if (strcmp(element_name, "i")==0) {
		Data->res->append("<i>");
	} else if (strcmp(element_name, "sup")==0) {
		Data->res->append("<sup>");
	} else if (strcmp(element_name, "sub")==0) {
		Data->res->append("<sub>");
	} else if (strcmp(element_name, "tt")==0) {
		Data->res->append("<tt>");
	} else if (strcmp(element_name, "big")==0) {
		Data->res->append("<big>");
	} else if (strcmp(element_name, "small")==0) {
		Data->res->append("<small>");
	} else if (strcmp(element_name, "blockquote")==0) {
		Data->res->append("<blockquote>");
	} else if (strcmp(element_name, "tr")==0) {
		Data->res->append("<b>[");
	} else if (strcmp(element_name, "ex")==0) {
		Data->res->append("<font color=\"violet\">");
	} else if (strcmp(element_name, "c")==0) {
		Data->res->append("<font");
		bool have_color = false;
		int i = 0;
		while (attribute_names[i]) {
			if (strcmp(attribute_names[i], "c")==0) {
				Data->res->append(" color=\"");
				Data->res->append(attribute_values[i]);
				Data->res->append("\"");
				have_color = true;
				break;
			}
			i++;
		}
		if (!have_color)
			Data->res->append(" color=\"blue\"");
		Data->res->append(">");
	}
}

static void xdxf_parse_end_element(GMarkupParseContext *context, const gchar *element_name, gpointer user_data, GError **error)
{
	XdxfParseUserData *Data = (XdxfParseUserData *)user_data;
	if (strcmp(element_name, "abr")==0) {
		Data->res->append("</i></font>");
	} else if (strcmp(element_name, "kref")==0) {
		Data->res->append("</a>");
	} else if (strcmp(element_name, "b")==0) {
		Data->res->append("</b>");
	} else if (strcmp(element_name, "i")==0) {
		Data->res->append("</i>");
	} else if (strcmp(element_name, "sup")==0) {
		Data->res->append("</sup>");
	} else if (strcmp(element_name, "sub")==0) {
		Data->res->append("</sub>");
	} else if (strcmp(element_name, "tt")==0) {
		Data->res->append("</tt>");
	} else if (strcmp(element_name, "big")==0) {
		Data->res->append("</big>");
	} else if (strcmp(element_name, "small")==0) {
		Data->res->append("</small>");
	} else if (strcmp(element_name, "blockquote")==0) {
		Data->res->append("</blockquote>");
	} else if (strcmp(element_name, "tr")==0) {
		Data->res->append("]</b>");
	} else if (strcmp(element_name, "ex")==0) {
		Data->res->append("</font>");
	} else if (strcmp(element_name, "c")==0) {
		Data->res->append("</font>");
	}
}

static void xdxf_parse_text(GMarkupParseContext *context, const gchar *text, gsize text_len, gpointer user_data, GError **error)
{
	const gchar *element = g_markup_parse_context_get_element(context);
	if (!element)
		return;
	XdxfParseUserData *Data = (XdxfParseUserData *)user_data;
	if (strcmp(element, "k")==0) {
		if (Data->firstk) {
			Data->firstk = false;
			return; // Omit it.
		}
	} else if (strcmp(element, "kref")==0) {
		char *estr = stardict_markup_escape_text(text, text_len);
		Data->res->append(estr);
		Data->res->append("&quot;)\">");
		Data->res->append(estr);
		g_free(estr);
		return;
	}
	if (Data->res->empty() && text[0] == '\n') { // Omit the first new line after <k></k>
		text++;
		text_len--;
		if (text_len == 0)
			return;
	}
	char *estr = stardict_markup_escape_text(text, text_len);
	Data->res->append(estr);
	g_free(estr);
}

static std::string xdxf2html(const char *p, guint32 sec_size)
{
	std::string res;
	XdxfParseUserData Data;
	Data.res = &res;
	Data.firstk = true;

	GMarkupParser parser;
	parser.start_element = xdxf_parse_start_element;
	parser.end_element = xdxf_parse_end_element;
	parser.text =  xdxf_parse_text;
	parser.passthrough = NULL;
	parser.error = NULL;
	GMarkupParseContext* context = g_markup_parse_context_new(&parser, (GMarkupParseFlags)0, &Data, NULL);
	g_markup_parse_context_parse(context, "<ar>", -1, NULL);
	g_markup_parse_context_parse(context, p, sec_size, NULL);
	g_markup_parse_context_parse(context, "</ar>", -1, NULL);
	g_markup_parse_context_end_parse(context, NULL);
	g_markup_parse_context_free(context);
	return res;
}

typedef struct _PangoParseUserData {
	std::string *res;
	std::string span_elements;
} PangoParseUserData;

static void pango_parse_start_element(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer user_data, GError **error)
{
	PangoParseUserData *Data = (PangoParseUserData *)user_data;
	if (strcmp(element_name, "markup")==0) {
	} else if (strcmp(element_name, "span")==0) {
		std::string res;
		Data->res->append("<span");
		int i = 0;
		while (attribute_names[i]) {
			if (strcmp(attribute_names[i], "face")==0 || strcmp(attribute_names[i], "font_family")==0) {
				Data->res->append(" face=\"");
				Data->res->append(attribute_values[i]);
				Data->res->append("\"");
			} else if (strcmp(attribute_names[i], "size")==0) {
				Data->res->append(" size=\"");
				if (strcmp(attribute_values[i], "xx-small")==0)
					Data->res->append("1");
				else if (strcmp(attribute_values[i], "x-small")==0)
					Data->res->append("2");
				else if (strcmp(attribute_values[i], "small")==0)
					Data->res->append("3");
				else if (strcmp(attribute_values[i], "medium")==0)
					Data->res->append("4");
				else if (strcmp(attribute_values[i], "large")==0)
					Data->res->append("5");
				else if (strcmp(attribute_values[i], "x-large")==0)
					Data->res->append("6");
				else if (strcmp(attribute_values[i], "xx-large")==0)
					Data->res->append("7");
				else if (strcmp(attribute_values[i], "smaller")==0)
					Data->res->append("-1");
				else if (strcmp(attribute_values[i], "larger")==0)
					Data->res->append("+1");
				Data->res->append("\"");
			} else if (strcmp(attribute_names[i], "style")==0) {
				if (strcmp(attribute_values[i], "italic")==0) {
					res+="<i>";
					Data->span_elements= "</i>" + Data->span_elements;
				}
			} else if (strcmp(attribute_names[i], "weight")==0) {
				if (strcmp(attribute_values[i], "bold")==0) {
					res+="<b>";
					Data->span_elements= "</b>" + Data->span_elements;
				}
			} else if (strcmp(attribute_names[i], "underline")==0) {
				if (strcmp(attribute_values[i], "single")==0) {
					res+="<u>";
					Data->span_elements= "</u>" + Data->span_elements;
				}
			} else if (strcmp(attribute_names[i], "strikethrough")==0) {
				if (strcmp(attribute_values[i], "true")==0) {
					res+="<s>";
					Data->span_elements= "</s>" + Data->span_elements;
				}
			} else if (strcmp(attribute_names[i], "rise")==0) {
				int s = atoi(attribute_values[i]);
				if (s>0) {
					res+="<sup>";
					Data->span_elements= "</sup>" + Data->span_elements;
				} else if (s<0) {
					res+="<sub>";
					Data->span_elements= "</sub>" + Data->span_elements;
				}
			} else if (strcmp(attribute_names[i], "foreground")==0) {
				Data->res->append(" color=\"");
				Data->res->append(attribute_values[i]);
				Data->res->append("\"");
			} else if (strcmp(attribute_names[i], "background")==0) {
				Data->res->append(" bgcolor=\"");
				Data->res->append(attribute_values[i]);
				Data->res->append("\"");
			} else if (strcmp(attribute_names[i], "lang")==0) {
				Data->res->append(" lang=\"");
				Data->res->append(attribute_values[i]);
				Data->res->append("\"");
			}
			i++;
		}
		Data->res->append(">");
		Data->res->append(res);
	} else {
		Data->res->append("<");
		Data->res->append(element_name);
		Data->res->append(">");
	}
}

static void pango_parse_end_element(GMarkupParseContext *context, const gchar *element_name, gpointer user_data, GError **error)
{
	PangoParseUserData *Data = (PangoParseUserData *)user_data;
	if (strcmp(element_name, "markup")==0) {
	} else if (strcmp(element_name, "span")==0) {
		Data->res->append(Data->span_elements);
		Data->res->append("</span>");
		Data->span_elements.clear();
	} else {
		Data->res->append("</");
		Data->res->append(element_name);
		Data->res->append(">");
	}
}

static void pango_parse_text(GMarkupParseContext *context, const gchar *text, gsize text_len, gpointer user_data, GError **error)
{
	PangoParseUserData *Data = (PangoParseUserData *)user_data;
	char *estr = stardict_markup_escape_text(text, text_len);
	Data->res->append(estr);
	g_free(estr);
}

static std::string pango2html(const char *p, guint32 sec_size)
{
	std::string res;
	PangoParseUserData Data;
	Data.res = &res;

	GMarkupParser parser;
	parser.start_element = pango_parse_start_element;
	parser.end_element = pango_parse_end_element;
	parser.text =  pango_parse_text;
	parser.passthrough = NULL;
	parser.error = NULL;
	GMarkupParseContext* context = g_markup_parse_context_new(&parser, (GMarkupParseFlags)0, &Data, NULL);
	bool needs_root;
	if (strncmp (p, "<markup>", 8) == 0)
		needs_root = false;
	else
		needs_root = true;
	if (needs_root)
		g_markup_parse_context_parse(context, "<markup>", -1, NULL);
	g_markup_parse_context_parse(context, p, sec_size, NULL);
	if (needs_root)
		g_markup_parse_context_parse(context, "</markup>", -1, NULL);
	g_markup_parse_context_end_parse(context, NULL);
	g_markup_parse_context_free(context);
	return res;
}

char* stardict_markup_escape_text (const char *text, int length)
{
	if (length < 0)
		length = strlen (text);
	GString *str;
	str = g_string_sized_new (length);

	const gchar *p;
	const gchar *end;
	p = text;
	end = text + length;
	const char *prev_char = NULL;
	while (p != end) {
		const gchar *next;
		next = g_utf8_next_char (p);
		switch (*p) {
			case '&':
				g_string_append (str, "&amp;");
				break;
			case '<':
				g_string_append (str, "&lt;");
				break;
			case '>':
				g_string_append (str, "&gt;");
				break;
			case '\'':
				g_string_append (str, "&#39;"); // "&apos;" can't show correctly in IE.
				break;
			case '"':
				g_string_append (str, "&quot;");
				break;
			case '\n':
				g_string_append (str, "<br>");
				break;
			case ' ':
				if (!prev_char || (*prev_char)==' ' || (*prev_char)=='\n')
					g_string_append (str, "&nbsp;");
				else
					g_string_append (str, " ");
				break;
			default:
				g_string_append_len (str, p, next - p);
				break;
		}
		prev_char = p;
		p = next;
	}
	return g_string_free (str, FALSE);
}

void escape_text(std::string &etext, const char *text)
{
	etext.clear();
	const char *prev_char = NULL;
	while (*text) {
		switch (*text) {
		case '&':
			etext+="&amp;";
			break;
		case '<':
			etext+="&lt;";
			break;
		case '>':
			etext+="&gt;";
			break;
		case '\'':
			etext+="&#39;"; // "&apos;" can't show correctly in IE.
			break;
		case '"':
			etext+="&quot;";
			break;
		case '\n':
			etext+="<br>";
			break;
		case ' ':
			if (!prev_char || (*prev_char)==' ' || (*prev_char)=='\n')
				etext+="&nbsp;";
			else
				etext+=' ';
			break;
		default:
			etext+=*text;
			break;
		}
		prev_char = text;
		text++;
	}
}

void print_data(char *data, const char *oword)
{
	std::string etext;
	guint32 data_size,sec_size=0;
	data_size=*reinterpret_cast<const guint32 *>(data);
	data+=sizeof(guint32); //Here is a bug fix of 2.4.8, which make (guint32(p - data)<data_size) become correct, when data_size is the following data size, not the whole size as 4 bytes bigger.
	const gchar *p=data;
	bool first_time = true;
	while (guint32(p - data)<data_size) {
		if (first_time)
			first_time=false;
		else
			printf("<br>");
		switch (*p) {
			case 'm':
			case 'l'://need more work...
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					escape_text(etext, p);
					printf("%s", etext.c_str());
				}
				sec_size++;
				break;
			case 'h':
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					printf("%s", p);
				}
				sec_size++;
				break;
			case 'g':
				p++;
				sec_size=strlen(p);
				if (sec_size) {
					std::string res=pango2html(p, sec_size);
					printf("%s", res.c_str());
				}
				sec_size++;
				break;
			case 'x':
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					std::string res=xdxf2html(p, sec_size);
					printf("%s", res.c_str());
				}
				sec_size++;
				break;
			case 'k':
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					std::string res=powerword2html(p, sec_size, oword);
					printf("%s", res.c_str());
				}
				sec_size++;
				break;
			case 'w':
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					printf("%s", p);
					//std::string res=wiki2html(p, sec_size);
				}
				sec_size++;
				break;
			case 't':
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					escape_text(etext, p);
					printf("[<font color=\"blue\">%s</font>]", etext.c_str());
				}
				sec_size++;						
				break;
			case 'y':
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					escape_text(etext, p);
					printf("[<font color=\"red\">%s</font>]", etext.c_str());
				}
				sec_size++;
				break;
			case 'W':
				p++;
				sec_size=g_ntohl(*reinterpret_cast<const guint32 *>(p));
				//enbale sound button.
				sec_size += sizeof(guint32);
				break;
			case 'P':
				p++;
				sec_size=g_ntohl(*reinterpret_cast<const guint32 *>(p));
				//show this picture.
				sec_size += sizeof(guint32);
				break;
			default:
				if (g_ascii_isupper(*p)) {
					p++;
					sec_size=g_ntohl(*reinterpret_cast<const guint32 *>(p));
					sec_size += sizeof(guint32);
				} else {
					p++;
					sec_size = strlen(p)+1;
				}
				break;
		}
		p += sec_size;
	}
}
