#include "html.h"

int create_html_template(const char* file_name) {
  FILE* f;
  f = fopen(file_name, "w");
  if (!f) {
    return 1;
  }
  
  // see html.h for usable tags
  DOCTYPE;
  HTML_LANG("en");
  HTML_CLOSE;
  return 0;
}
