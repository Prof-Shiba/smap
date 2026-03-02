#include "./shiba-getopt.h"

int opt_index = 1, opt_err = 1, opt_opt = 1;
char* opt_arg = 0;

int getopt_reset(void) {
  opt_index = 1; // remember where in argv we are. start at 1
  opt_err = 1; // whether we print error messages or not
  opt_opt = 0; // curent option being processed (h in -h)
  opt_arg = 0; // options arg
  return 0;
}

// handles short options (like -v, -h etc)
static int _get_opt(int argc, char* argv[], const char* opts) {
  static int char_index = 0;
  const char* s;
  char mode = opts[0];
  int off = 0, opt = -1;
  opt_arg = 0;

  if(char_index) {
    opt_opt = argv[opt_index][char_index];

    for(s = opts + off; *s; s++)
      if(opt_opt == *s) {
        char_index++;

        if(*(++s) == ':') {
          if(argv[opt_index][char_index]) {
            opt_arg = &(argv[opt_index++][char_index]);
            char_index = 0;
          }
          else if(*(++s) != ':') {
            char_index = 0;
            if(++opt_index >= argc) {
              if(opt_err) fprintf(stderr,
                                  "%s: option needs an argument -- %c\n",
                                  argv[0], opt_opt);
              opt = '?';
              goto my_get_opt_ok;
            }
          opt_arg = argv[opt_index++];
        }
      }
      opt = opt_opt;
      goto my_get_opt_ok;
    }
    if(opt_err) fprintf(stderr,
                        "%s: invalid option -- %c\n",
                        argv[0], opt_opt);
    opt = '?';
    if(argv[opt_index][++char_index] == '\0') {
      opt_index++;
      char_index = 0;
    }

  my_get_opt_ok:
    if(char_index && !argv[opt_index][char_index]) {
      opt_index++;
      char_index = 0;
    }
  }
  else if((opt_index >= argc) ||
             ((argv[opt_index][0] == '-') &&
              (argv[opt_index][1] == '-') &&
              (argv[opt_index][2] == '\0'))) {
    opt_index++;
    opt = -1;
  }
  else if((argv[opt_index][0] != '-') ||
             (argv[opt_index][1] == '\0')) { // char is '-', like stdin/out indicator
    char* tmp;
    int i, j, k;

    if(mode == '+') opt = -1;
    else {
      // rotation alg to put options anywhere in the cmd line
      for(i = j = opt_index; i < argc; i++)
        if((argv[i][0] == '-') && (argv[i][1] != '\0')) {
        opt_index = i;
        opt = _get_opt(argc, argv, opts);

        while(i > j) {
          tmp = argv[--i];

          for(k = i; k + 1 < opt_index; k++)
              argv[k] = argv[k + 1];

          argv[--opt_index] = tmp;
        }
        break;
      }
      if(i == argc) opt = -1;
    }
  } else {
    char_index++;
    opt = _get_opt(argc, argv, opts); // at argv ele starting with '-' and has more chars, recursively parse it
  }
  if (opt_index > argc) opt_index = argc;
  return opt;
}

// handles long options (--help) and short options (-h)
int _get_opt_internal(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index, int long_only)
{
  char mode = short_opts[0];
  char colon_mode = *short_opts;
  int short_off = 0; 
  int opt = -1;
  opt_arg = 0;

  if (mode == '+' || mode == '-' || mode == ':') {
    short_off = 1;
    if (short_opts[1] == ':') short_off = 2;
  }

  if ((opt_index >= argc) ||
     ((argv[opt_index][0] == '-') &&
     (argv[opt_index][1] == '-') &&
     (argv[opt_index][2] == '\0'))) {
      opt_index++;
      opt = -1;
  }
  
  else if ((argv[opt_index][0] != '-') || 
          (argv[opt_index][1] == '\0')) {
    char* temp;
    int i, j, k;
    opt = -1;

    if (mode == '+')
      return -1;
    else if (mode == '-') {
      opt_arg = argv[opt_index++];
      return 1;
    }
    
    // same rotation algorithm as _get_opt, but calls _get_opt_internal recursively
    for (i = j = opt_index; i < argc; i++)
      if ((argv[i][0] == '-') && 
      (argv[i][1] != '\0')) {
        opt_index = i;
        opt = _get_opt_internal(argc, argv, short_opts, long_opts, long_index, long_only);

        // rotate array to move non-options after processed option
        while (i > j) {
          temp = argv[--i];
          for (k = i; k + 1 < opt_index; k++)
            argv[k] = argv[k+1];
          argv[--opt_index] = temp;
        }
        break;
      }
  }
  
  // in getopt_long, -h is always short. in getopt_long_only, -h might be long
  else if ((!long_only) && (argv[opt_index][1] != '-'))
    opt = _get_opt(argc, argv, short_opts);
  
  else {
    int char_index, offset;
    int found = 0, ind, hits = 0; 

    if(((opt_opt = argv[opt_index][1]) != '-') && !argv[opt_index][2]) {
      int c;

      ind = short_off;
      while((c = short_opts[ind++])) { 
        if (short_opts[ind] == ':') {
          ind++;
          if (short_opts[ind] == ':') 
            ind++;
        }
        if(opt_opt == c) return _get_opt(argc, argv, short_opts);
      }
    }
    
    // offset into argv[opt_index] to skip past dashes
    // For --help: offset = 2
    // For -help (in long_only mode): offset = 1
    offset = 2 - (argv[opt_index][1] != '-');
    
    for(char_index = offset;
        (argv[opt_index][char_index] != '\0') &&
          (argv[opt_index][char_index] != '=');
        char_index++);

    for(ind = 0; long_opts[ind].name && !hits; ind++) 
      if((strlen(long_opts[ind].name) == (size_t) (char_index - offset)) &&
         (strncmp(long_opts[ind].name,
                  argv[opt_index] + offset, char_index - offset) == 0))
        found = ind, hits++;

    // allows abbreviations like --he for --help (if unambiguous)
    if(!hits) for(ind = 0; long_opts[ind].name; ind++)
      if(strncmp(long_opts[ind].name,
                 argv[opt_index] + offset, char_index - offset) == 0)
        found = ind, hits++; // Count all matches to detect ambiguity
    
    if(hits == 1) {
      opt = 0;

      if(argv[opt_index][char_index] == '=') {
        if(long_opts[found].has_arg == 0) {
          opt = '?';
          if(opt_err) fprintf(stderr,
                             "%s: option `--%s' doesn't allow an argument\n",
                             argv[0], long_opts[found].name);
        } else {
          opt_arg = argv[opt_index] + ++char_index; 
          char_index = 0; 
        }
      }
      else if(long_opts[found].has_arg == 1) { 
        if(++opt_index >= argc) { 
          opt = (colon_mode == ':') ? ':' : '?';
          if(opt_err) fprintf(stderr,
                             "%s: option `--%s' requires an argument\n",
                             argv[0], long_opts[found].name);
        } else
            opt_arg = argv[opt_index];
      }
      
      if(!opt) { 
        if (long_index) *long_index = found; 
        
        if(!long_opts[found].flag)
          opt = long_opts[found].val;
        else // flag mode: set *flag = val, return 0
          *(long_opts[found].flag) = long_opts[found].val;
      }
      opt_index++;
    }
    
    else if(!hits) { 
      if(offset == 1) opt = _get_opt(argc, argv, short_opts);
      else {
        opt = '?';
        if(opt_err) fprintf(stderr,
                           "%s: unrecognized option `%s'\n",
                           argv[0], argv[opt_index++]);
      }
    }
    
    else { // multiple matches, cant tell what it is
      opt = '?';
      if(opt_err) fprintf(stderr,
                         "%s: option `%s' is ambiguous\n",
                         argv[0], argv[opt_index++]);
    }
  }
  
  if (opt_index > argc) opt_index = argc;// clamp
  return opt;
}

// normal: --option for long, -o for short
int getopt_long(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index)
{
  return _get_opt_internal(argc, argv, short_opts, long_opts, long_index, 0);
}

// more forgiving: -option and --option both work for long options
int getopt_long_only(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index)
{
  return _get_opt_internal(argc, argv, short_opts, long_opts, long_index, 1);
}
