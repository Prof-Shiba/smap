#include "html.h"

int create_html_template(const char* file_name) {
  FILE* f;
  char* lang = "en";
  f = fopen(file_name, "w");
  if (!f) {
    return 1;
  }
  
  // see html.h for usable tags
  DOCTYPE;
  HTML_LANG(lang);
  HTML_CLOSE;
  return 0;
}
