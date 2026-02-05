#pragma once

// this file will create a html file displaying all the scan
// info as well as a pre-made css file to make everything
// look *pretty*...hopefully. i dont do ui or anything
// so no promises
//
#include "../libs/shiba-core/shiba.h"
#include "../smap-options/smap-options.h"

#ifndef SMAP_HTML_TAGS
#define SMAP_HTML_TAGS

#define HTML_DOCTYPE fprintf(f, "<!DOCTYPE html>\n")
#define HTML_LANG(lang) fprintf(f, "<html lang=\"%s\">\n", lang)
#define HTML_CLOSE fprintf(f, "</html>\n")

#define HTML_HEAD_OPEN fprintf(f, "<head>\n")
#define HTML_HEAD_CLOSE fprintf(f, "</head>\n")

#define HTML_CHARSET fprintf(f, "<meta charset=\"UTF-8\">\n")
#define HTML_VIEWPORT fprintf(f, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n")

#define HTML_TITLE(title) fprintf(f, "<title>%s</title>\n", title)
#define HTML_SMAP_STYLE fprintf(f, "<link href=\"smap_style.css\" rel=\"stylesheet\">\n")

#define HTML_BODY_OPEN fprintf(f, "<body>\n\n")
#define HTML_BODY_CLOSE fprintf(f, "</body>\n")

#define HTML_SECTION(class) fprintf(f, "<section class=\"%s\">\n\n</section>\n", class)

#define HTML_H1_OPEN fprintf(f, "<h1>")
#define HTML_H1_CLOSE fprintf(f, "</h1>\n")
#define HTML_H2_OPEN fprintf(f, "<h2>")
#define HTML_H2_CLOSE fprintf(f, "</h2>\n")
#define HTML_H3_OPEN fprintf(f, "<h3>")
#define HTML_H3_CLOSE fprintf(f, "</h3>\n")

#define HTML_PARAGRAPH_OPEN(class) fprintf(f, "<p class=\"%s\">", class)
#define HTML_PARAGRAPH_CLOSE fprintf(f, "</p>\n")

#define HTML_UL_OPEN(class) fprintf(f, "<ul class=\"%s\">\n", class)
#define HTML_UL_CLOSE fprintf(f, "</ul>\n")

#define HTML_LIST_OPEN fprintf(f, "<li>")
#define HTML_LIST_CLOSE fprintf(f, "</li>\n")

#define HTML_STRONG_OPEN fprintf(f, "<strong>")
#define HTML_STRONG_CLOSE fprintf(f, "</strong>\n")

#define HTML_SPAN_OPEN(class) fprintf(f, "<span class=\"%s\">", class)
#define HTML_SPAN_CLOSE fprintf(f, "</span>\n")

#define HTML_BR fprintf(f, "<br>\n")

#define HTML_FOOTER_OPEN fprintf(f, "<footer>")
#define HTML_FOOTER_CLOSE fprintf(f, "</footer>\n")

#define HTML_MAIN_OPEN fprintf(f, "<main>\n")
#define HTML_MAIN_CLOSE fprintf(f, "</main>\n")

#endif
