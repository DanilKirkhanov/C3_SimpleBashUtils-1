#ifndef PARSER_H
#define PARSER_H
typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Fl;
void ShortFlag(char flag[], Fl *flags);
void LongFlag(char flag[], Fl *flags);
void Parser(Fl *flags, char *argv[], int argc, int *f);

#endif
