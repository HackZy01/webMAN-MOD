#define HTML_RECV_SIZE	2048
#define HTML_RECV_LAST	2047

#define ORANGE		0xf90
#define MAGENTA		0xf0f
#define CYAN		0x0ff

static char html_base_path[HTML_RECV_SIZE]; // used as temporary buffer

#define ITEM_CHECKED			" checked=\"checked\""
#define ITEM_SELECTED			" selected=\"selected\""

#define IS_MARKED(key)			(strstr(param, key) != NULL)
#define IS_UNMARKED(key)		(!strstr(param, key))

#define HTML_REFRESH			"<meta http-equiv=\"refresh\" content=\""

#define HTML_URL				"<a href=\"%s\">%s</a>"
#define HTML_URL2				"<a href=\"%s%s\">%s</a>"

#define HTML_URL_STYLE			"color:#ccc;text-decoration:none;"

#define HTML_DIR				"&lt;dir&gt;"
#define HTML_BUTTON_FMT			"%s%s\" %s'%s';\">"
#define HTML_BUTTON_FMT2		"%s%s\" %s'%s%s';\">"
#define HTML_BUTTON				" <input type=\"button\" value=\""
#define HTML_ONCLICK			"onclick=\"location.href="
#define HTML_ONCLICK2			"onclick=\"var req=new XMLHttpRequest();req.open('GET',"
#define HTML_SEND_CMD			"',true);req.send();return'"
#define HTML_INPUT(n, v, m, s)	"<input name=\"" n "\" type=\"text\" value=\"" v "\" maxlength=\"" m "\" size=\"" s "\">"
#define HTML_PASSW(n, v, m, s)	"<input name=\"" n "\" type=\"password\" value=\"" v "\" maxlength=\"" m "\" size=\"" s "\">"
#define HTML_NUMBER(n, v, min, max)	"<input name=\"" n "\" type=\"number\" value=\"" v "\" min=\"" min "\" max=\"" max "\" style=\"width:45px;\">"
#define HTML_PORT(n, v)			"<input name=\"" n "\" type=\"number\" value=\"" v "\" min=\"1\" max=\"65535\" style=\"width:60px;\">"

#define HTML_DISABLED_CHECKBOX	"disabled"

#define HTML_FORM_METHOD_FMT(a)	"<form action=\"" a "%s"

#define HTML_FORM_METHOD		".ps3mapi\" method=\"get\" enctype=\"application/x-www-form-urlencoded\" target=\"_self\">"

#define HTML_ENTRY_DATE			" &nbsp; " \
								"<td>11-Nov-2006 11:11"

#define _BR_					NULL

#define HTML_RESPONSE_FMT		"HTTP/1.1 %i OK\r\n" \
								"X-PS3-Info: [%s]\r\n" \
								"Content-Type: text/html;charset=UTF-8\r\n" \
								"Cache-Control: no-cache\r\n" \
								"\r\n" \
								"%s%s%s"

#define HTML_HEADER				" <!DOCTYPE html>" \
								"<html xmlns=\"http://www.w3.org/1999/xhtml\">" \
								"<meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\">" \
								"<meta http-equiv=\"Cache-Control\" content=\"no-cache\">" \
								"<meta name=\"viewport\" content=\"width=device-width,initial-scale=0.6,maximum-scale=1.0\">"  /* size: 264 */

#define HTML_RESPONSE_TITLE		WM_APP_VERSION EDITION "<hr><h2>"

#define HTML_BODY				"<body bgcolor=\"#101010\" text=\"#c0c0c0\">" \
								"<font face=\"Courier New\">" /* size: 64 */

#define HTML_BODY_END			"</font></body></html>" /* size: 21 */

#define HTML_BLU_SEPARATOR		"<hr color=\"blue\"/>"
#define HTML_RED_SEPARATOR		"<hr color=\"red\"/>"

#define SCRIPT_SRC_FMT			"<script src=\"%s\"></script>"
#define HTML_REDIRECT_TO_URL	"<script>setTimeout(function(){self.location=\"%s\"},%i);</script>"
#define HTML_REDIRECT_TO_BACK	"<script>history.back();</script>"
#define HTML_CLOSE_BROWSER		"<script>window.close(this);</script>"
#define HTML_REDIRECT_WAIT		3000

#define HTML_TOGGLER			HTML_BLU_SEPARATOR "<b><a class=\"tg\" href=\"javascript:tgl("

#define HTML_SHOW_LAST_GAME		"<span style=\"position:absolute;right:8px\"><font size=2>"
#define HTML_SHOW_LAST_GAME_END	"</font></span>"

#define MAX(a, b)		((a) >= (b) ? (a) : (b))
#define MIN(a, b)		((a) <= (b) ? (a) : (b))
#define ABS(a)			(((a) < 0) ? -(a) : (a))
#define RANGE(a, b, c)	((a) <= (b) ? (b) : (a) >= (c) ? (c) : (a))
#define ISDIGIT(a)		('0' <= (unsigned char)(a) && (unsigned char)(a) <= '9')

int extcmp(const char *s1, const char *s2, size_t n);
int extcasecmp(const char *s1, const char *s2, size_t n);
char *strcasestr(const char *s1, const char *s2);

static void open_browser(char *url, int mode)
{
	int is_ingame = View_Find("game_plugin");

	if(is_ingame)
	{
		game_interface = (game_plugin_interface *)plugin_GetInterface(is_ingame, 1);
		game_interface->wakeupWithGameExit(url, 1);
	}
	else
	{
		vshmain_AE35CF2D(url, mode); // xmb_plugin->Function 23
	}
}

static bool IS(const char *a, const char *b)
{
	if(!a || !*a) return false;
	while(*a && (*a == *b)) a++,b++;
	return !(*a-*b); // compare two strings. returns true if they are identical
}

static bool _IS(const char *a, const char *b)
{
	if(!a || !*a) return false;
	return (strcasecmp(a, b) == 0);	// compare two strings. returns true if they are identical (case insensitive)
}

static void replace_char(char *text, char c, char r)
{
	if(!text) return;

	char *pos = strchr(text, c);
	while (pos)
	{
		*pos = r; pos = strchr(pos, c);
	}
}

#if defined(PS3MAPI) || defined(DEBUG_MEM)
static u64 faster_find(const char *find, int len, char *mask)
{
	if(!mask || !find) return 0;

	u64 faster = *(u64*)find; // first 64bits

	replace_char(mask, '*', '?'); // use single type of wildcard

	if(len < 8)
		faster = 0; // do not fast find if len < 8
	else
	{
		char s = mask[8]; mask[8] = '\0'; // truncate to search only first 8 characters
		if(strchr(mask, '?')) faster = 0; // do not fast find if using mask
		mask[8] = s; // restore mask
	}

	return faster;
}

static bool bcompare(const char *a, const char *b, u8 len, const char *mask)
{
	while(len && ((*a++ == *b++) || (*mask == '?'))) {mask++,len--;}
	return len;
}
#endif

static char *to_upper(char *text)
{
	char *upper = text;
	for( ; *text; text++) if(*text >= 'a' && *text <= 'z') *text ^= 0x20;
	return upper;
}

#ifndef LITE_EDITION
static bool _islike(const char *param, const char *text)
{
	if(!param || !text) return false;
	return strncasecmp(param, text, strlen(text)) == 0;
}
#endif

static bool islike(const char *param, const char *text)
{
	if(!param || !text) return false;
	while(*text && (*text == *param)) text++, param++;
	return !*text;
}

static char h2a(const char hex)
{
	char c = hex;
	if(c >= 0 && c <= 9)
		c += '0';
	else if(c >= 10 && c <= 15)
		c += 55; //A-F
	return c;
}

static void urldec(char *url, char *original)
{
	if(!url) return;
	if(strchr(url, '+') || strchr(url, '%'))
	{
		if(original) strcpy(original, url); // return original url

		u16 pos = 0; char c;
		for(u16 i = 0; url[i] >= ' '; i++, pos++)
		{
			if(url[i] == '+')
				url[pos] = ' ';
			else if(url[i] != '%')
				url[pos] = url[i];
			else
			{
				url[pos] = 0; u8 n = 2; if(url[i+1]=='u') {i++, n+=2;}
				while(n--)
				{
					url[pos] <<= 4, c = (url[++i] | 0x20);
					if(c >= '0' && c <= '9') url[pos] += (c - '0'); else
					if(c >= 'a' && c <= 'f') url[pos] += (c - 'W'); // <= c - 'a' + 10
				}
			}
		}
		url[pos] = '\0';
	}
}

static bool urlenc_ex(char *dst, const char *src, bool gurl)
{
	if(!src || !dst) return false;

	size_t i, j = 0, pos = 0;

	if(islike(src, "http") && (src[4] == ':' || src[5] == ':') && (src[6] == '/') && src[7]) { for(i = 8; src[i]; i++) if(src[i] == '/') {pos = i; break;} }

#ifdef USE_NTFS
	if(islike(src + pos, "/dev_nt")) pos += 11;
#endif

	for(i = 0; src[i]; i++, j++)
	{
		if(j >= HTML_RECV_LAST) {j = HTML_RECV_LAST; break;}

		if((unsigned char)src[i] & 0x80)
		{
			dst[j++] = '%';
			dst[j++] = h2a((unsigned char)src[i]>>4);
			dst[j] = h2a(src[i] & 0xf);
		}
		else if((src[i]=='?' || ((src[i]==':') && (i >= pos))) && gurl)
		{
			dst[j++] = '%';
			dst[j++] = '3';
			dst[j] = (src[i] & 0xf) + '7'; // A | F
		}
		else if(src[i]==' ')
			dst[j] = '+';
		else if(src[i]=='\'' || src[i]=='"' || src[i]=='%' || src[i]=='&' || src[i]=='+' || src[i]=='#')
		{
			dst[j++] = '%';
			dst[j++] = '2';
			dst[j] = (src[i] == '+') ? 'B' : '0' + (src[i] & 0xf);
		}
		else
			dst[j] = src[i];
	}
	dst[j] = '\0';

	return (j > i); // true if dst != src
}

static bool urlenc(char *dst, const char *src)
{
	return urlenc_ex(dst, src, true);
}

static size_t htmlenc(char *dst, char *src, u8 cpy2src)
{
	if(!src || !dst) return 0;

	size_t j = 0;
	char tmp[10]; u8 t, c;
	for(size_t i = 0; src[i]; i++)
	{
		if((unsigned char)src[i] >= 0x7F || (unsigned char)src[i] < 0x20)
		{
			t = sprintf(tmp, "&#x%X;", (unsigned char)src[i]); c = 0;
			while(t--) {dst[j++] = tmp[c++];}
		}
		else dst[j++] = src[i];
	}

	j = MIN(j, MAX_LINE_LEN);
	dst[j] = '\0';

	if(cpy2src) strcpy(src, dst);
	return j;
}

static size_t utf8enc(char *dst, char *src, u8 cpy2src)
{
	if(!src || !dst) return 0;
	size_t j = 0; unsigned int c;
	for(size_t i = 0; src[i]; i++)
	{
		c = (unsigned char)src[i];
		if(c <= 0x7F)
			dst[j++] = c;
		else { if(c <= 0x07FF) {
			dst[j++] = 0xC0 | (c>>06);
		} else if(c <= 0xFFFF) {
			dst[j++] = 0xE0 | (0x0F & (c>>12));
			dst[j++] = 0x80 | (0x3F & (c>>06));
		} else if(c <= 0x1FFFFF) {
			dst[j++] = 0xF0 | (0x0F & (c>>18));
			dst[j++] = 0x80 | (0x3F & (c>>12));
			dst[j++] = 0x80 | (0x3F & (c>>06));
		} else if(c <= 0x3FFFFFF) {
			dst[j++] = 0xF8 | (0x0F & (c>>24));
			dst[j++] = 0x80 | (0x3F & (c>>18));
			dst[j++] = 0x80 | (0x3F & (c>>12));
			dst[j++] = 0x80 | (0x3F & (c>>06));
		} else if(c <= 0x7FFFFFFF) {
			dst[j++] = 0xFC | (0x0F & (c>>30));
			dst[j++] = 0x80 | (0x3F & (c>>24));
			dst[j++] = 0x80 | (0x3F & (c>>18));
			dst[j++] = 0x80 | (0x3F & (c>>12));
			dst[j++] = 0x80 | (0x3F & (c>>06));
		}	dst[j++] = 0x80 | (0x3F & c); }
	}

	j = MIN(j, MAX_LINE_LEN);
	dst[j] = '\0';

	if(cpy2src) strcpy(src, dst);
	return j;
}
/*
static size_t utf8dec(char *dst, char *src, u8 cpy2src)
{
	size_t j = 0;
	unsigned char c;
	for(size_t i = 0; src[i]; i++, j++)
	{
		c = (unsigned char)src[i];
		if( (c & 0x80) == 0 )
		{
			dst[j] = c;
		}
		else if( (c & 0xE0) == 0xC0 )
		{
			dst[j] = ((c & 0x1F)<<6) |
					 (src[i+1] & 0x3F);
			i+=1;
		}
		else if( (c & 0xF0) == 0xE0 )
		{
			dst[j]  = ((c & 0xF)<<12);
			dst[j] |= ((src[i+1] & 0x3F)<<6);
			dst[j] |= ((src[i+2] & 0x3F));
			i+=2;
		}
		else if ( (c & 0xF8) == 0xF0 )
		{
			dst[j]  = ((c & 0x7)<<18);
			dst[j] |= ((src[i+1] & 0x3F)<<12);
			dst[j] |= ((src[i+2] & 0x3F)<<06);
			dst[j] |= ((src[i+3] & 0x3F));
			i+=3;
		}
		else if ( (c & 0xFC) == 0xF8 )
		{
			dst[j]  = ((c & 0x3)<<24);
			dst[j] |= ((src[i+1] & 0x3F)<<18);
			dst[j] |= ((src[i+2] & 0x3F)<<12);
			dst[j] |= ((src[i+3] & 0x3F)<<06);
			dst[j] |= ((src[i+4] & 0x3F));
			i+=4;
		}
		else if ( (c & 0xFE) == 0xFC )
		{
			dst[j]  = ((c & 0x1)<<30);
			dst[j] |= ((src[i+1] & 0x3F)<<24);
			dst[j] |= ((src[i+2] & 0x3F)<<18);
			dst[j] |= ((src[i+3] & 0x3F)<<12);
			dst[j] |= ((src[i+4] & 0x3F)<<06);
			dst[j] |= ((src[i+5] & 0x3F));
			i+=5;
		}
	}

	j = MIN(j, MAX_LINE_LEN);
	dst[j] = '\0';

	if(cpy2src) strcpy(src, dst);
	return j;
}
*/
static void add_url(char *body, const char *prefix, const char *url, const char *sufix)
{
	if(!body) return;

	if(prefix) concat(body, prefix);
	if(url)    concat(body, url);
	if(sufix)  concat(body, sufix);
}

static void add_html(u8 id, int value, char *buffer, char *templn)
{
	if(!buffer || !templn) return;

	char res_file[40];
	sprintf(res_file, "%s/setup/setup%c.dat", WM_RES_PATH, id);
	read_file(res_file, templn, 1023, 0);
	char *pos = strstr(templn, "    ");
	if(pos)
	{
		if((id == 'a') || (id == 'v'))
			sprintf(res_file, "%04x", value);
		else
			sprintf(res_file, "%04i", value);
		memcpy(pos, res_file, 4);
	}
	concat(buffer, templn);
}

static size_t add_radio_button(const char *name, int value, const char *id, const char *label, const char *sufix, bool checked, char *buffer)
{
	if(!buffer || !label) return 0;

	char templn[MAX_LINE_LEN];
	sprintf(templn, "<label><input type=\"radio\" name=\"%s\" value=\"%i\" id=\"%s\"%s/> %s%s</label>", name, value, id, checked ? ITEM_CHECKED : "", label, (sufix) ? sufix : "<br>");
	return concat(buffer, templn);
}

static size_t add_check_box(const char *name, bool disabled, const char *label, const char *sufix, bool checked, char *buffer)
{
	if(!buffer || !label) return 0;

	char templn[MAX_LINE_LEN], clabel[MAX_LINE_LEN];
	strcpy(clabel, label);
	char *p = strstr(clabel, AUTOBOOT_PATH);
	if(p)
	{
		u8 pos = p - clabel;
		sprintf(p, HTML_INPUT("autop", "%s", "255", "40"), webman_config->autoboot_path);
		strcat(p, label + pos + strlen(AUTOBOOT_PATH));
	}
	sprintf(templn, "<label><input type=\"checkbox\" name=\"%s\" value=\"1\" %s%s/> %s</label>%s", name, disabled ? HTML_DISABLED_CHECKBOX : "", checked ? ITEM_CHECKED : "", clabel, (!sufix) ? "<br>" : sufix);
	return concat(buffer, templn);
}

static size_t add_checkbox(const char *name, const char *label, const char *sufix, bool checked, char *buffer)
{
	return add_check_box(name, false, label, sufix, checked, buffer);
}

static size_t _add_checkbox(const char *name, const char *label, bool checked, char *buffer)
{
	return add_checkbox(name, label, _BR_, checked, buffer);
}

static size_t add_option_item(int value, const char *label, bool selected, char *buffer)
{
	char templn[MAX_LINE_LEN], svalue[20];
	if(value >= 0x1000) sprintf(svalue, "0x%x", value); else sprintf(svalue, "%i", value);
	sprintf(templn, "<option value=\"%s\"%s/>%s", svalue, selected ? ITEM_SELECTED : "", label);
	return concat(buffer, templn);
}

#if defined(VIDEO_REC) || defined(USE_UACCOUNT)
//static size_t add_string_item(const char *value, const char *label, bool selected, char *buffer)
//{
//	char templn[MAX_LINE_LEN];
//	sprintf(templn, "<option value=\"%s\"%s/>%s", value, selected ? ITEM_SELECTED : "", label);
//	return concat(buffer, templn);
//}
#endif

static size_t prepare_header(char *buffer, const char *param, u8 is_binary)
{
	if(!buffer || !param) return 0;

	bool set_base_path = false;

	size_t slen = sprintf(buffer,	"HTTP/1.1 200 OK\r\n"
									"%s"
									"Content-Type: ",
									webman_config->bind ? "" : // disallow CORS if bind (remote access to FTP/WWW services) is disabled
									"Access-Control-Allow-Origin: *\r\n"); // default: allow CORS (Cross-Origin Resource Sharing)

	t_string header; _set(&header, buffer, slen);

	// get mime type
	if(is_binary == BINARY_FILE)
	{
		const char *ext = get_ext(param);

		if(_IS(ext, ".png"))
			_concat2(&header, "image/", "png");
		else
		if(_IS(ext, ".jpg") || _IS(ext, ".jpeg") || IS(ext, ".STH"))
			_concat2(&header, "image/", "jpeg");
		else
		if(_IS(ext, ".htm") || _IS(ext, ".html") || _IS(ext, ".shtm"))
			{_concat2(&header, "text/", "html;charset=UTF-8"); set_base_path = true;}
		else
		if(_IS(ext + 1, ".js"))
			_concat2(&header, "text/", "javascript");
		else
		if(_IS(ext, ".css"))
			_concat2(&header, "text/", "css");
		else
		if(_IS(ext, ".txt") || _IS(ext, ".log") || _IS(ext, ".ini") || _IS(ext, ".cfg") || IS(ext, ".HIP") || IS(ext, ".HIS") || IS(ext, ".HIP") || IS(ext, ".CNF"))
			_concat2(&header, "text/", "plain;charset=UTF-8");
		else
		if(_IS(ext, ".svg"))
			_concat2(&header, "image/", "svg+xml");
		#ifndef LITE_EDITION
		else
		if(_IS(ext, ".gif"))
			_concat2(&header, "image/", "gif");
		else
		if(_IS(ext, ".bmp"))
			_concat2(&header, "image/", "bmp");
		else
		if(_IS(ext, ".tif"))
			_concat2(&header, "image/", "tiff");
		else
		if(_IS(ext, ".avi"))
			_concat2(&header, "video/", "x-msvideo");
		else
		if(_IS(ext, ".mp4") || IS(ext, ".MTH"))
			_concat2(&header, "video/", "mp4");
		else
		if(_IS(ext, ".mkv"))
			_concat2(&header, "video/", "x-matroska");
		else
		if(_IS(ext, ".mpg") || _IS(ext, ".mp2") || strcasestr(ext, ".mpe"))
			_concat2(&header, "video/", "mpeg");
		else
		if(_IS(ext, ".vob"))
			_concat2(&header, "video/", "vob");
		else
		if(_IS(ext, ".wmv"))
			_concat2(&header, "video/", "x-ms-wmv");
		else
		if(_IS(ext, ".flv"))
			_concat2(&header, "video/", "x-flv");
		else
		if(_IS(ext, ".mov"))
			_concat2(&header, "video/", "quicktime");
		else
		if(_IS(ext, ".webm"))
			_concat2(&header, "video/", "webm");
		else
		if(_IS(ext, ".mp3"))
			_concat2(&header, "audio/", "mpeg");
		else
		if(_IS(ext, ".wav"))
			_concat2(&header, "audio/", "x-wav");
		else
		if(_IS(ext, ".wma"))
			_concat2(&header, "audio/", "x-ms-wma");
		else
		if(_IS(ext, ".mid") || _IS(ext, ".kar"))
			_concat2(&header, "audio/", "midi");
		else
		if(_IS(ext, ".mod"))
			_concat2(&header, "audio/", "mod");
		else
		if(_IS(ext, ".zip"))
			_concat2(&header, "application/", "zip");
		else
		if(_IS(ext, ".pdf"))
			_concat2(&header, "application/", "pdf");
		else
		if(_IS(ext, ".doc"))
			_concat2(&header, "application/", "msword");
		else
		if(_IS(ext, ".docx"))
			_concat2(&header, "application/", "vnd.openxmlformats-officedocument.wordprocessingml.document");
		else
		if(_IS(ext, ".xls"))
			_concat2(&header, "application/", "vnd.ms-excel");
		else
		if(_IS(ext, ".xlsx"))
			_concat2(&header, "application/", "vnd.openxmlformats-officedocument.spreadsheetml.sheet");
		else
		if(_IS(ext, ".ppt") || _IS(ext, ".pps"))
			_concat2(&header, "application/", "vnd.ms-powerpoint");
		else
		if(_IS(ext, ".swf"))
			_concat2(&header, "application/", "x-shockwave-flash");
		#endif
		else
			_concat2(&header, "application/", "octet-stream");
	}
	else
		{_concat2(&header, "text/", "html;charset=UTF-8"); set_base_path = true;}

	if(set_base_path && (is_binary != WEB_COMMAND) && param[0] == '/') {strcpy(html_base_path, param); if(!isDir(param)) remove_filename(html_base_path);}

	_concat(&header, "\r\n");

	return header.size;
}

static u64 convertH(const char *val); // peek_poke.h

static int oct(const char *c)
{
	if(!c) return 0;

	int value = 0;
	if(c)
		while(ISDIGIT(*c))
		{
			value <<= 3;
			value += (*c - '0'); c++;
		}
	return value;
}

static s64 val(const char *c)
{
	if(!c) return 0;

	if(islike(c, "0x"))
	{
		return convertH((char*)c);
	}

	s64 previous_result = 0, result = 0;
	s64 multiplier = 1;

	if(c && *c == '-')
	{
		multiplier = -1;
		c++;
	}

	while(*c)
	{
		if(!ISDIGIT(*c)) return result * multiplier;

		result *= 10;
		if(result < previous_result)
			return(0); // overflow
		else
			previous_result = result;

		result += (*c & 0x0F);
		if(result < previous_result)
			return(0); // overflow
		else
			previous_result = result;

		c++;
	}
	return(result * multiplier);
}

static u16 get_value(char *value, const char *url, u16 max_size)
{
	if(!value || !url || !max_size) return 0;

	u16 n;
	for(n = 0; n < max_size; n++)
	{
		if(url[n] == '&' || url[n] == 0) break;
		value[n] = url[n];
	}
	value[n] = '\0';
	return n;
}

static u16 get_param(const char *name, char *value, const char *url, u16 max_size)
{
	if(!name || !value || !url || !max_size) return 0;

	u8 name_len = strlen(name);

	if(name_len)
	{
		char *pos = strstr(url, name);
		if(pos)
		{
			if(name[name_len - 1] != '=') name_len++;
			return get_value(value, pos + name_len, max_size);
		}
	}

	*value = '\0';
	return 0;
}

static s64 get_valuen64(const char *param, const char *label)
{
	if(!param || !label) return false;

	char value[12], *pos = strstr(param, label);
	if(pos)
	{
		get_value(value, pos + strlen(label), 11);
		return val(value);
	}
	return 0;
}

static u32 get_valuen32(const char *param, const char *label)
{
	return (u32)get_valuen64(param, label);
}

static u16 get_port(const char *param, const char *label, u16 default_port)
{
	u16 port = (u16)get_valuen32(param, label);
	if(port)
		port = RANGE(port, 1, 65535);
	else
		port = default_port;
	return port;
}

static u8 get_valuen(const char *param, const char *label, u8 min_value, u8 max_value)
{
	u8 value = (u8)get_valuen64(param, label);
	return RANGE(value, min_value, max_value);
}

static u8 get_flag(const char *param, const char *label)
{
	if(!param || !label) return false;

	char *flag = strstr(param, label);
	if(flag)
	{
		*flag = NULL; return true;
	}
	return false;
}

#ifndef LITE_EDITION
static u8 parse_tags(char *text)
{
	if(!text) return 0;

	char *pos; u8 op = 0;
	pos = strstr(text, "@info");
	if(pos)
	{
		op = val(pos + 5) + 10;
		get_sys_info(pos, op % 100, (op >= 100));
	}
	return op;
}
#endif

#ifndef LITE_EDITION
static char *prepend(char *a, const char *b, int len)
{
	int i = strlen(a);
	int n = strlen(b);
	if(len <= 0) len = n;
	for(; i >= 0; i--)
		a[i + len] = a[i];
	memcpy(a, b, n);
	return a;
}

static u8 get_operator(char *equal_pos, bool nullfy)
{
	if(!equal_pos) return 0;
	if(*equal_pos != '=') equal_pos = strchr(equal_pos, '=');
	if(!equal_pos) return 0;

	char *prev = (equal_pos - 1);
	const char c = nullfy ? '\0' : *prev;
	if(*prev == '^') {*prev = c; return'^';} // field^=value
	if(*prev == '|') {*prev = c; return'|';} // field|=value
	if(*prev == '&') {*prev = c; return'&';} // field&=value
	return 0;
}

static u64 update_value(u64 old_valuen, u64 new_valuen, u8 oper)
{
	if(oper == '|')
		new_valuen |= old_valuen;
	if(oper == '&')
		new_valuen &= old_valuen;
	if(oper == '^')
		new_valuen ^= old_valuen;
	return new_valuen;
}
#endif
