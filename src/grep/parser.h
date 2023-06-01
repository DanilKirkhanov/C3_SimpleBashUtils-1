#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE
typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int f;
  int n;
  int h;
  int s;
  int o;
  char *pattern;
} Fl;
int ShortFlag(char const flag[], Fl *flags);
void Parser(Fl *flags, char *argv[], int argc, int *f);
int NumFiles(int argc, char *argv[]);
char *Patterns(int argc, char **argv, int *err);
void CatPattern(char *pattern, int *err, char **ptrs, size_t *countmem);
#endif
