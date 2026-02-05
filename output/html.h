#pragma once

// this file will create a html file displaying all the scan
// info as well as a pre-made css file to make everything
// look *pretty*...hopefully. i dont do ui or anything
// so no promises
//
#include "../libs/shiba-core/shiba.h"
#include "../output/scan_report.h"

#ifndef SMAP_HTML_TAGS
#define SMAP_HTML_TAGS

// TODO: possibly add more if needed and finished implementation file
#define DOCTYPE fprintf(f, "<!DOCTYPE html>")
#define HTML_LANG(lang) fprintf(f, "<html lang=\"%s\">", lang)
#define HTML_CLOSE fprintf(f, "</html>")
#define HEAD fprintf(f, "<head>\n\n</head>")
#define CHARSET fprintf(f, "<meta charset=\"UTF-8\">")
#define VIEWPORT fprintf(f, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">")
#define TITLE fprintf(f, "<title>%s</title>", title)
#define SMAP_STYLE fprintf(f, "<link href=\"smap_style.css\" rel=\"stylesheet\">")
#define BODY fprintf(f, "<body>\n\n</body>")

#endif

// creates html5 boilerplate.
// returns 0 on success (was able to write to a file)
// 1 on failure
int create_html_template(const char* file_name);
