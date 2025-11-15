#include "./shiba-getopt.h"

// This is the implementation for the cross platform getopt
// functions, where I added comments to hopefully explain
// most of whats going on, as its pretty cool imo.

int opt_index = 1, opt_err = 1, opt_opt = 1;
char* opt_arg = 0;

int getopt_reset(void) {
  opt_index = 1; // remember where in argv we are. start at 1
  opt_err = 1; // whether we print error messages or not
  opt_opt = 0; // curent option being processed (h in -h)
  opt_arg = 0; // options arg (<filename> in -f)
  return 0;
}

// handles short options (like -v, -h etc)
static int _get_opt(int argc, char* argv[], const char* opts) {
  static int char_index = 0; // track our position within an argv element. resets to 0 when 1 ele is done
  const char* s; // scan thru opts string
  char mode = opts[0];
  int off = 0, opt = -1; // off = offset into opts str, opt = return value (opt char or -1 if done)
  opt_arg = 0; // clear any prev args

  if(char_index) { // we're in the middle of parsing -abc for example
    opt_opt = argv[opt_index][char_index]; // get current character we are processing

    for(s = opts + off; *s; s++) // go thru opts to see if char is valid opt. like "hv:f::"
      if(opt_opt == *s) {
        char_index++;

        if(*(++s) == ':') { // check ahead to see if it takes an arg
          if(argv[opt_index][char_index]) { //if not null, -ffilename.txt, arg is stuck onto option
            opt_arg = &(argv[opt_index++][char_index]); // point opt_arg there
            char_index = 0; // reset bc we finished that argv element
          }
          else if(*(++s) != ':') { // if the next char isnt a ':', its a required arg
            char_index = 0;
            if(++opt_index >= argc) { // opt requires arg, none provided though
              if(opt_err) fprintf(stderr,
                                  "%s: option needs an argument -- %c\n",
                                  argv[0], opt_opt);
              opt = '?';
              goto my_get_opt_ok; // cleanup and dip
            }
          opt_arg = argv[opt_index++]; // we found next arg, advance
        }
      }
      opt = opt_opt;
      goto my_get_opt_ok;
    }
    // if we finished the loop and found no option in opts, return '?'
    if(opt_err) fprintf(stderr,
                        "%s: invalid option -- %c\n",
                        argv[0], opt_opt);
    opt = '?';
    // invalid, but advance past it. if we reached end of argv ele (null term), move to next ele
    if(argv[opt_index][++char_index] == '\0') {
      opt_index++;
      char_index = 0;
    }

  my_get_opt_ok:
    // if char_index is set, (middle of -abc), but char is NULL, we are done
    // this is for if we just finished processing c in -abc
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
  else if((argv[opt_index][0] != '-') || // is a positional arg like filename.txt
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
      if(i == argc) opt = -1; // at the end with no extra options, done
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
  char colon_mode = *short_opts; // If first char is ':', return ':' instead of '?' for missing args
  int short_off = 0; // Offset for skipping mode characters in short_opts
  int opt = -1;
  opt_arg = 0;

  // adjust short_off if short_opts starts with special mode characters
  if (mode == '+' || mode == '-' || mode == ':') {
    short_off = 1;
    if (short_opts[1] == ':') short_off = 2;  // Handle "::" at start
  }

  //check for end-of-options (same as _get_opt)
  if ((opt_index >= argc) ||
     ((argv[opt_index][0] == '-') &&
     (argv[opt_index][1] == '-') &&
     (argv[opt_index][2] == '\0'))) {
      opt_index++;
      opt = -1;
  }
  
  // non-option argument - use permutation alg
  else if ((argv[opt_index][0] != '-') || 
          (argv[opt_index][1] == '\0')) {
    char* temp;
    int i, j, k;
    opt = -1;

    if (mode == '+') // stop at first non-option
      return -1;
    else if (mode == '-') { // treat non-options as options with value 1
      opt_arg = argv[opt_index++];
      return 1;
    }
    
    // same rotation algorithm as _get_opt, but calls _get_opt_internal recursively
    for (i = j = opt_index; i < argc; i++)
      if ((argv[i][0] == '-') && 
      (argv[i][1] != '\0')) {
        opt_index = i;
        // recursive call to handle the option we found
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
    opt = _get_opt(argc, argv, short_opts);  // Delegate to short option parser
  
  // parsing long options (or long_only single-dash options)
  else {
    int char_index, offset;
    int found = 0, ind, hits = 0; // found = index of matched option, hits = number of matches

    // in long_only mode with single char like -h, check if it's a short option first
    if(((opt_opt = argv[opt_index][1]) != '-') && !argv[opt_index][2]) {
      int c;

      ind = short_off; // start after any mode characters
      while((c = short_opts[ind++])) { // scan through short_opts
        // skip over ':' markers
        if (short_opts[ind] == ':') {
          ind++;
          if (short_opts[ind] == ':') // optional args (::)
            ind++;
        }
        // if this single char matches a short option, call _get_opt instead
        if(opt_opt == c) return _get_opt(argc, argv, short_opts);
      }
    }
    
    // offset into argv[opt_index] to skip past dashes
    // For --help: offset = 2
    // For -help (in long_only mode): offset = 1
    offset = 2 - (argv[opt_index][1] != '-');
    
    // find where the option name ends (either at '=' or \0)
    // For --output=file.txt, char_index will point at the '='
    for(char_index = offset;
        (argv[opt_index][char_index] != '\0') &&
          (argv[opt_index][char_index] != '=');
        char_index++);

    // find exact matches
    for(ind = 0; long_opts[ind].name && !hits; ind++) // !hits ensures we stop after first exact match
      if((strlen(long_opts[ind].name) == (size_t) (char_index - offset)) &&
         (strncmp(long_opts[ind].name,
                  argv[opt_index] + offset, char_index - offset) == 0))
        found = ind, hits++;

    // no exact match, look for prefix matches
    // allows abbreviations like --he for --help (if unambiguous)
    if(!hits) for(ind = 0; long_opts[ind].name; ind++)
      if(strncmp(long_opts[ind].name,
                 argv[opt_index] + offset, char_index - offset) == 0)
        found = ind, hits++; // Count all matches to detect ambiguity
    
    if(hits == 1) { // exactly one match
      opt = 0; // initialize to 0 (success), may be overwritten

      // check if user provided an argument with = syntax
      if(argv[opt_index][char_index] == '=') {
        if(long_opts[found].has_arg == 0) { // option doesn't take an argument!
          opt = '?';
          if(opt_err) fprintf(stderr,
                             "%s: option `--%s' doesn't allow an argument\n",
                             argv[0], long_opts[found].name);
        } else {
          opt_arg = argv[opt_index] + ++char_index; // point past the '='
          char_index = 0; // we're done with this element
        }
      }
      // option requires an argument but none provided with =
      else if(long_opts[found].has_arg == 1) { // required_argument
        if(++opt_index >= argc) { // try next argv element
          opt = (colon_mode == ':') ? ':' : '?';
          if(opt_err) fprintf(stderr,
                             "%s: option `--%s' requires an argument\n",
                             argv[0], long_opts[found].name);
        } else
            opt_arg = argv[opt_index];
      }
      
      if(!opt) { // If no error occurred
        if (long_index) *long_index = found; // tell caller which option matched
        
        if(!long_opts[found].flag) // normal mode: return val
          opt = long_opts[found].val;
        else // flag mode: set *flag = val, return 0
          *(long_opts[found].flag) = long_opts[found].val;
      }
      opt_index++;// Advance past this option
    }
    
    else if(!hits) { // No matches found
      // In long_only mode with single dash, might still be a short option
      if(offset == 1) opt = _get_opt(argc, argv, short_opts);
      else { // Definitely not a valid option
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
