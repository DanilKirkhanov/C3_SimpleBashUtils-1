#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ShortFlag(char flag[], Fl *flags) {
  char n = flag[1];
  int i = 1;
  while (n != '\0') {
    switch (n) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->v = 1;
        flags->e = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      default:
        fprintf(stderr, "s21_cat: illegal option --%c", n);
        exit(0);
    }
    i++;
    n = flag[i];
  }
}

void LongFlag(char flag[], Fl *flags) {
  if (strcmp(flag, "--number-nonblank") == 0)
    flags->b = 1;
  else if (strcmp(flag, "--number") == 0)
    flags->n = 1;
  else if (strcmp(flag, "--squeeze-blank") == 0)
    flags->s = 1;
  else if (strcmp(flag, "--") == 0)
    ;
  else {
    fprintf(stderr, "s21_cat: illegal option %s", flag);
    exit(0);
  }
}

void Parser(Fl *flags, char *argv[], int argc, int *f) {
  if (argc > 1) {
    for (int i = 0; i < argc; i++) {
      if (argc == 1) *f = 0;
      if (argv[i][0] == '-' && argv[1][1] == '-') {
        LongFlag(argv[i], flags);
        *f = *f + 1;
      } else if (argv[i][0] == '-') {
        ShortFlag(argv[i], flags);
        *f = *f + 1;
      }
    }
  }
  if (flags->b) flags->n = 0;
}