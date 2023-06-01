#include <regex.h>

#include "parser.h"
void Grep(int argc, char *argv[], Fl *flags);
void GrepFile(regex_t *preg, FILE *file, Fl *flags, char *name, int countfiles);
void flag_o(char *line, regmatch_t match, regex_t preg);
int main(int argc, char *argv[]) {
  Fl flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL};
  int f = 1;
  Parser(&flags, argv, argc, &f);
  if (f == 1 && argc > 2) {
    Grep(argc, argv, &flags);
    // printf("%ld\n%s", strlen(flags.pattern), flags.pattern);
  }

  if (!f || argc == 1)
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n[-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n[--context[=num]] [--directories=action] "
            "[--label] [--line-buffered]\n[--null] [pattern] [file ...]\n");
  free(flags.pattern);
  return 0;
}

void Grep(int argc, char *argv[], Fl *flags) {
  int countfiles = NumFiles(argc, argv);
  char *pattern;
  int count = 2;
  pattern = argv[1];
  for (int i = 1; pattern[0] == '-' && !(flags->e || flags->f); i++) {
    pattern = argv[i];
    count = i + 1;
  }
  int a = 0;
  if (flags->e || flags->f) {
    a |= REG_EXTENDED;
    pattern = flags->pattern;
  } else
    countfiles = countfiles - 1;
  if (flags->i) a |= REG_ICASE;
  a |= REG_NEWLINE;
  regex_t preg;
  if (regcomp(&preg, pattern, a) != 0) {
    fprintf(stderr, "failed cimpile");
  } else {
    for (int i = count; i < argc; i++) {
      char *c = strchr(argv[i - 1], 'e');
      char *f = strchr(argv[i - 1], 'f');
      FILE *file = fopen(argv[i], "rb");
      if (file && !(argv[i - 1][0] == '-' && c != NULL && c[1] == '\0') &&
          !(argv[i - 1][0] == '-' && f != NULL && f[1] == '\0')) {
        GrepFile(&preg, file, flags, argv[i], countfiles);
        fclose(file);
      } else if (argv[i][0] != '-' && !flags->s &&
                 !(argv[i - 1][0] == '-' && c != NULL && c[1] == '\0') &&
                 !(argv[i - 1][0] == '-' && f != NULL && f[1] == '\0'))
        fprintf(stderr, "no such file or dir %s\n", argv[i]);
    }
    regfree(&preg);
  }
  
}

void GrepFile(regex_t *preg, FILE *file, Fl *flags, char *name,
              int countfiles) {
  char *str = NULL;
  size_t leng;
  regmatch_t match;
  int countmatch = 0;
  for (int countline = 1; getline(&str, &leng, file) > 0; countline++) {
    if ((!regexec(preg, str, 1, &match, 0) && !flags->v) ||
        (regexec(preg, str, 1, &match, 0) && flags->v)) {
      if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = '\0';
      if (!flags->l) {
        if (!flags->c) {
          if (!flags->h && countfiles > 1) printf("%s:", name);
          if (flags->n) printf("%d:", countline);
          if (flags->o)
            flag_o(str, match, *preg);
          else
            printf("%s\n", str);
        }
        countmatch++;
      } else {
        flags->l = 2;
        countmatch = 1;
      }
    }
  }
  
  if (flags->c) {
    if (countfiles > 1) printf("%s:", name);
    printf("%d\n", countmatch);
    }
  if (flags->l == 2) {
    printf("%s\n", name);
    flags->l = 1;
  }
  free(str);
}
void flag_o(char *line, regmatch_t match, regex_t preg) {
  char *buf_line = line;
  printf("%.*s\n", (int)(match.rm_eo - match.rm_so), buf_line + match.rm_so);
  while (!regexec(&preg, buf_line += match.rm_eo, 1, &match, REG_NOTEOL)) {
    printf("%.*s\n", (int)(match.rm_eo - match.rm_so), buf_line + match.rm_so);
  }
}