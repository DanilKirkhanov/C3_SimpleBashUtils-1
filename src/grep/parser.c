#include "parser.h"

int ShortFlag(char const flag[], Fl *flags) {
  char n = flag[1];
  int i = 1;
  int fe = 1;
  while (n != '\0' && fe == 1) {
    switch (n) {
      case 'e':
        flags->e = 1;
        fe = 2;
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'f':
        flags->f = 1;
        fe = 2;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        fprintf(stderr, "s21_grep: invalid option --%c\n", n);
        fe = 0;
    }
    i++;
    n = flag[i];
  }
  return fe == 2 ? 1 : fe;
}

void Parser(Fl *flags, char *argv[], int argc, int *f) {
  flags->pattern = Patterns(argc, argv, f);
  for (int i = 1; i < argc && *f == 1; i++) {
    if (argv[i][0] == '-') {
      *f = ShortFlag(argv[i], flags);
    }
  }
}

int NumFiles(int argc, char *argv[]) {
  int flag = 0;
  for (int i = 1; i < argc; i++) {
    char *f = NULL, *c = NULL;
    c = strchr(argv[i - 1], 'e');
    f = strchr(argv[i - 1], 'f');
    if (argv[i][0] != '-' &&
        !(argv[i - 1][0] == '-' && c != NULL && c[1] == '\0') &&
        !(argv[i - 1][0] == '-' && f != NULL && f[1] == '\0'))
      flag++;
  }
  return flag;
}

char *Patterns(int argc, char **argv, int *err) {
  char *c = NULL, *ff = NULL;
  char *ptrs = NULL;
  size_t countmem = 0;
  char *str = NULL;
  size_t leng;
  for (int i = 1; i < argc && *err; i++) {
    c = strchr(argv[i], 'e');
    ff = strchr(argv[i], 'f');
    if (c != NULL && argv[i][0] == '-' &&
        (ff == NULL || strlen(ff) < strlen(c))) {
      if (c[1] == '\0') {
        if (argv[i + 1] != NULL) {
          CatPattern(argv[i + 1], err, &ptrs, &countmem);
        } else {
          fprintf(stderr, "grep: option requires an argument -- e\n");
          *err = 0;
        }
      } else {
        CatPattern(c + 1, err, &ptrs, &countmem);
      }
    } else {
      if (ff != NULL && argv[i][0] == '-') {
        FILE *f = NULL;
        if (ff[1] == '\0') {
          if (argv[i + 1] != NULL) {
            f = fopen(argv[i + 1], "rb");
          } else {
            fprintf(stderr, "grep: option requires an argument -- f\n");
            *err = 0;
          }
        } else {
          f = fopen(ff + 1, "rb");
        }
        if (f != NULL) {
          while (getline(&str, &leng, f) > 0) {
            if (str[strlen(str) - 1] == '\n' && strlen(str) > 1)
              str[strlen(str) - 1] = '\0';
            CatPattern(str, err, &ptrs, &countmem);
          }
          free(str);
          fclose(f);
        } else {
          fprintf(stderr, "grep: %s: No such file or directory\n",
                  ff[1] == '\0' ? argv[i + 1] : ff + 1);
          *err = 2;
        }
      }
    }
  }
  return ptrs;
}

void CatPattern(char *pattern, int *err, char **ptrs, size_t *countmem) {
  if (*ptrs == NULL) {
    *ptrs = (char *)malloc(1024);
    *countmem = 1;
    if (*ptrs != NULL) {
      strcpy(*ptrs, pattern);
    } else {
      *err = 0;
      fprintf(stderr, "NO MEM");
    }
  } else {
    if (strlen(*ptrs) + strlen(pattern) + 2 > *countmem * 1024) {
      *countmem = *countmem + 1;
      *ptrs = realloc(*ptrs, *countmem * 1024 + 1024);
    }
    if (*ptrs != NULL) {
      strcat(*ptrs, "|");
      if (strcmp(pattern, "\n") == 0)
        strcat(*ptrs, ".");
      else
        strcat(*ptrs, pattern);
    } else {
      *err = 0;
      fprintf(stderr, "NO MEM");
    }
  }
}