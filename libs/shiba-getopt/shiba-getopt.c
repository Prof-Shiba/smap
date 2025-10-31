#include "./shiba-getopt.h"

int opt_index = 1, opt_err = 1, opt_opt = 1;
char* opt_arg = 0;

int getopt_reset(void) {
  opt_index = 1; // remember where in argv we are
  opt_err = 1;
  opt_opt = 0; // curent option being processed (h in -h)
  opt_arg = 0; // optiona args (<filename> in -f)
  return 0;
}

static int _get_opt(int argc, char* argv[], const char* opts) {
  static int char_index = 0; // track our position within an argv element
  const char* s; // scan thru opts string
  char mode;
  int off = 0, opt = -1;
  opt_arg = 0;

  if(char_index) {
    opt_opt = argv[opt_index][char_index];

    for(s = opts + off; *s; s++) if(opt_opt == *s) {
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
    if(char_index && ! argv[opt_index][char_index]) {
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
             (argv[opt_index][1] == '\0')) {
    char* tmp;
    int i, j, k;

    if(mode == '+') opt = -1;
    else {
      for(i = j = opt_index; i < argc; i++) if((argv[i][0] == '-') &&
                                        (argv[i][1] != '\0')) {
        opt_index=i;
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
    opt = _get_opt(argc, argv, opts);
  }
  if (opt_index > argc) opt_index = argc;
  return opt;
}

int _get_opt_internal(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index, int long_only)
{
  char mode, colon_mode = *short_opts;
  int short_off = 0, opt = -1;
  opt_arg = 0;

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
    // cool shuffling alg. rearranges the order to move processed options
    // before non-option arguments, so you can pass stuff in in any order
    for (i = j = opt_index; i < argc; i++)
      if ((argv[i][0] == '-') && 
      (argv[i][1] != '\0')) {
        opt_index = i;
        opt = _get_opt_internal(argc, argv, short_opts, long_opts, long_index, long_only);

        while (i > j) {
          temp = argv[--i];
          for (k = i; k + 1 < opt_index; k++)
            argv[k] = argv[k+1];
          argv[--opt_index] = temp;
        }
        break;
      }
  }
  else if ((!long_only) && (argv[opt_index][1] != '-'))
    opt = _get_opt(argc, argv, short_opts);
  else {
   int char_index, offset;
    int found = 0, ind, hits = 0;

    if(((opt_opt = argv[opt_index][1]) != '-') && ! argv[opt_index][2]) {
      int c;

      ind = short_off;
      while((c = short_opts[ind++])) {
        if(((short_opts[ind] == ':') ||
            ((c == 'W') && (short_opts[ind] == ';'))) &&
           (short_opts[++ind] == ':'))
          ind ++;
        if(opt_opt == c) return _get_opt(argc, argv, short_opts);
      }
    }
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

    if(!hits) for(ind = 0; long_opts[ind].name; ind++)
      if(strncmp(long_opts[ind].name,
                 argv[opt_index] + offset, char_index - offset) == 0)
        found = ind, hits++;
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
      } else if(long_opts[found].has_arg == 1) {
        if(++opt_index >= argc) {
          opt = (colon_mode == ':') ? ':' : '?';

          if(opt_err) fprintf(stderr,
                             "%s: option `--%s' requires an argument\n",
                             argv[0], long_opts[found].name);
        } else opt_arg = argv[opt_index];
      }
      if(!opt) {
        if (long_index) *long_index = found;
        if(!long_opts[found].flag) opt = long_opts[found].val;
        else *(long_opts[found].flag) = long_opts[found].val;
      }
      opt_index++;
    } else if(!hits) {
      if(offset == 1) opt = _get_opt(argc, argv, short_opts);
      else {
        opt = '?';
        if(opt_err) fprintf(stderr,
                           "%s: unrecognized option `%s'\n",
                           argv[0], argv[opt_index++]);
      }
    } else {
      opt = '?';
      if(opt_err) fprintf(stderr,
                         "%s: option `%s' is ambiguous\n",
                         argv[0], argv[opt_index++]);
    }
  }
  if (opt_index > argc) opt_index = argc;
  return opt;
}

int getopt_long(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index)
{
  return _get_opt_internal(argc, argv, short_opts, long_opts, long_index, 0);
}

int getopt_long_only(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index)
{
  return _get_opt_internal(argc, argv, short_opts, long_opts, long_index, 1);
}
