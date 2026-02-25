#include "../libs/shiba-core/shiba.h"
#include "../smap-options/smap-options.h"
#include "../output/print_time.h"
#include "./html.h"

extern void handle_report(scan_info_t* s, const f32 cpu_time);
extern void print_report(scan_info_t* s, FILE* f, const f32 cpu_time);
extern void print_html_report(scan_info_t* s, FILE* f, const f32 cpu_time);
extern void print_greppable_report(scan_info_t* s, FILE* f, const f32 cpu_time);
extern void print_smap_file_report(scan_info_t* s, FILE* f, const f32 cpu_time);
extern void print_css_file();
