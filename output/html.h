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

#define HTML_DOCTYPE fprintf(stream, "<!DOCTYPE html>\n")
#define HTML_LANG(lang) fprintf(stream, "<html lang=\"%s\">\n", lang)
#define HTML_CLOSE fprintf(stream, "</html>\n")

#define HTML_HEAD_OPEN fprintf(stream, "<head>\n")
#define HTML_HEAD_CLOSE fprintf(stream, "</head>\n")

#define HTML_CHARSET fprintf(stream, "<meta charset=\"UTF-8\">\n")
#define HTML_VIEWPORT fprintf(stream, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n")

#define HTML_TITLE(title) fprintf(stream, "<title>%s</title>\n", title)
#define HTML_SMAP_STYLE fprintf(stream, "<link href=\"smap_style.css\" rel=\"stylesheet\">\n")

#define HTML_BODY_OPEN fprintf(stream, "<body>\n\n")
#define HTML_BODY_CLOSE fprintf(stream, "</body>\n")

#define HTML_SECTION(class) fprintf(stream, "<section class=\"%s\">\n\n</section>\n", class)

#define HTML_H1_OPEN fprintf(stream, "<h1>")
#define HTML_H1_CLOSE fprintf(stream, "</h1>\n")
#define HTML_H2_OPEN fprintf(stream, "<h2>")
#define HTML_H2_CLOSE fprintf(stream, "</h2>\n")
#define HTML_H3_OPEN fprintf(stream, "<h3>")
#define HTML_H3_CLOSE fprintf(stream, "</h3>\n")

#define HTML_PARAGRAPH_OPEN(class) fprintf(stream, "<p class=\"%s\">", class)
#define HTML_PARAGRAPH_CLOSE fprintf(stream, "</p>\n")

#define HTML_UL_OPEN(class) fprintf(stream, "<ul class=\"%s\">\n", class)
#define HTML_UL_CLOSE fprintf(stream, "</ul>\n")

#define HTML_LIST_OPEN fprintf(stream, "<li>")
#define HTML_LIST_CLOSE fprintf(stream, "</li>\n")

#define HTML_STRONG_OPEN fprintf(stream, "<strong>")
#define HTML_STRONG_CLOSE fprintf(stream, "</strong>\n")

#define HTML_SPAN_OPEN(class) fprintf(stream, "<span class=\"%s\">", class)
#define HTML_SPAN_CLOSE fprintf(stream, "</span>\n")

#define HTML_BR fprintf(stream, "<br>\n")

#define HTML_FOOTER_OPEN fprintf(stream, "<footer>")
#define HTML_FOOTER_CLOSE fprintf(stream, "</footer>\n")

#define HTML_MAIN_OPEN fprintf(stream, "<main>\n")
#define HTML_MAIN_CLOSE fprintf(stream, "</main>\n")

#endif
